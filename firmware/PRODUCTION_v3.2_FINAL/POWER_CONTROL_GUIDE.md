# Power Control System - RAMS Controller

**Дата:** 2026-02-16
**Версия:** 1.0

---

## 🎯 Задача

Управлять всей системой одной кнопкой:
- Компьютер + Монитор
- Актуаторы + LED
- Последовательное включение с задержкой

---

## 🔌 Схема подключения

```
┌─────────────────────────────────────────────────────┐
│   220V ГЛАВНАЯ РОЗЕТКА                               │
└───────────┬─────────────────────────────────────────┘
            │
            ├──→ ESP32 БП (5V 2A) [ВСЕГДА ВКЛЮЧЕН]
            │     └─→ ESP32 Controller
            │          ├─ GPIO 19 → SSR Relay #1 (IN+)
            │          ├─ GPIO 18 → Relay #2 (IN)
            │          └─ GPIO 4  → Power Button (w/ 10kΩ pullup)
            │
            ├──→ SSR Relay #1 (40A) [Actuators Power]
            │     ├─ IN+ → GPIO 19 (ESP32)
            │     ├─ IN- → GND
            │     ├─ AC IN → 220V Main
            │     └─ AC OUT → 220V

→ Actuators Power Block
            │              ├─→ БП Mega #1 (12V)
            │              ├─→ БП Mega #2 (12V)
            │              ├─→ БП Actuators (24V)
            │              └─→ БП LED (12V)
            │
            └──→ Relay #2 (10A) [PC Power]
                  ├─ IN → GPIO 18 (ESP32)
                  ├─ COM → 220V Main
                  └─ NO → 220V → Удлинитель
                       ├─→ Windows PC
                       └─→ Монитор
```

---

## 📦 Компоненты

### 1. **SSR Relay 40A** (для актуаторов)
- **Модель:** Fotek SSR-40DA или аналог
- **Нагрузка:** 40A / 8000W
- **Управление:** 3-32V DC
- **Цена:** ~1500₽

**Подключение:**
```
ESP32 GPIO 19 → SSR (IN+)
ESP32 GND     → SSR (IN-)
220V Main     → SSR (AC IN 1)
220V Main     → SSR (AC IN 2)
SSR (AC OUT 1) → Actuators Power Block
SSR (AC OUT 2) → Actuators Power Block
```

### 2. **Relay Module 10A** (для компьютера)
- **Модель:** SRD-05VDC-SL-C (1-канальный модуль)
- **Нагрузка:** 10A / 2200W
- **Управление:** 5V DC
- **Цена:** ~150₽

**Подключение:**
```
ESP32 GPIO 18 → Relay (IN)
ESP32 GND     → Relay (GND)
ESP32 5V      → Relay (VCC)

220V Main → Relay (COM)
Relay (NO) → PC Power Strip
```

### 3. **ESP32 БП отдельный** (5V 2A)
- **Всегда включен** в розетку
- Не зависит от релюшек
- Питает только ESP32

### 4. **Физическая кнопка Power** (опционально)
- Большая кнопка с подсветкой
- Тип: Momentary push button
- **Подключение:**
  ```
  GPIO 4 ────┬──── Button ──── GND
             │
           10kΩ
             │
            3.3V
  ```

---

## 💻 Код для ESP32

### Добавь ПЕРЕД setup():

```cpp
// ============================================================================
// POWER CONTROL КОНФИГУРАЦИЯ
// ============================================================================
#define RELAY_ACTUATORS  19  // GPIO19 → SSR Relay 40A (актуаторы + LED)
#define RELAY_PC         18  // GPIO18 → Relay 10A (компьютер + монитор)
#define POWER_BUTTON     4   // GPIO4  → Физическая кнопка Power ON/OFF

bool actuatorsPowerOn = false;
bool pcPowerOn = false;
unsigned long pcStartTime = 0;
const unsigned long PC_BOOT_DELAY = 30000; // 30 секунд для загрузки Windows
```

### В setup() добавь ПОСЛЕ Serial.begin():

```cpp
// Power Control инициализация
pinMode(RELAY_ACTUATORS, OUTPUT);
pinMode(RELAY_PC, OUTPUT);
pinMode(POWER_BUTTON, INPUT_PULLUP);

digitalWrite(RELAY_ACTUATORS, LOW);  // Выключено
digitalWrite(RELAY_PC, LOW);         // Выключено

Serial.println("[POWER] Relay initialized");
Serial.println("[POWER] GPIO19 = Actuators/LED (OFF)");
Serial.println("[POWER] GPIO18 = PC/Monitor (OFF)");
Serial.println("[POWER] GPIO4  = Power Button (INPUT)");
```

### Добавь API endpoint ПОСЛЕ server.on("/api/stop"):

```cpp
// ===== POWER CONTROL API =====

// Включить систему (последовательно)
server.on("/api/power/on", HTTP_POST, []() {
  Serial.println("[POWER] System ON sequence started");

  // 1. Включить компьютер
  digitalWrite(RELAY_PC, HIGH);
  pcPowerOn = true;
  pcStartTime = millis();
  Serial.println("[POWER] PC/Monitor ON");

  // 2. Ждать 30 секунд (Windows загружается)
  server.send(200, "text/plain", "PC starting, actuators will power on in 30s");

  // Актуаторы включатся автоматически в loop()
});

// Выключить систему
server.on("/api/power/off", HTTP_POST, []() {
  Serial.println("[POWER] System OFF");

  // 1. Выключить актуаторы
  digitalWrite(RELAY_ACTUATORS, LOW);
  actuatorsPowerOn = false;

  delay(1000);

  // 2. Выключить компьютер (грубо, просто обрубить питание)
  digitalWrite(RELAY_PC, LOW);
  pcPowerOn = false;

  // 3. Остановить все блоки
  Mega1Serial.println("ALL:STOP");
  Mega2Serial.println("ALL:STOP");
  FastLED.clear(true);

  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    blockStates[i].isActive = false;
  }
  activeBlocksCount = 0;

  server.send(200, "text/plain", "System powered off");
});

// Статус питания
server.on("/api/power/status", HTTP_GET, []() {
  String json = "{";
  json += "\"pc\":" + String(pcPowerOn ? "true" : "false") + ",";
  json += "\"actuators\":" + String(actuatorsPowerOn ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
});
```

### В loop() добавь ПЕРЕД heartbeat:

```cpp
// ===== AUTO POWER ON ACTUATORS (после загрузки PC) =====
if (pcPowerOn && !actuatorsPowerOn) {
  unsigned long elapsed = now - pcStartTime;

  if (elapsed >= PC_BOOT_DELAY) {
    Serial.println("[POWER] PC boot delay complete, turning ON actuators");
    digitalWrite(RELAY_ACTUATORS, HIGH);
    actuatorsPowerOn = true;
  }
}

// ===== ФИЗИЧЕСКАЯ КНОПКА POWER =====
static bool lastButtonState = HIGH;
bool buttonState = digitalRead(POWER_BUTTON);

if (buttonState == LOW && lastButtonState == HIGH) {
  delay(50); // Debounce

  if (digitalRead(POWER_BUTTON) == LOW) {
    // Кнопка нажата - переключить питание
    if (!pcPowerOn) {
      Serial.println("[BUTTON] Power ON pressed");
      digitalWrite(RELAY_PC, HIGH);
      pcPowerOn = true;
      pcStartTime = now;
    } else {
      Serial.println("[BUTTON] Power OFF pressed");
      digitalWrite(RELAY_ACTUATORS, LOW);
      digitalWrite(RELAY_PC, LOW);
      actuatorsPowerOn = false;
      pcPowerOn = false;

      Mega1Serial.println("ALL:STOP");
      Mega2Serial.println("ALL:STOP");
      FastLED.clear(true);
    }
  }
}

lastButtonState = buttonState;
```

---

## 🎮 Управление из приложения

### В lib/esp32-client.ts добавь:

```typescript
/**
 * Включить всю систему
 */
async powerOn(): Promise<void> {
  const url = `/api/power/on`;
  console.log(`[ESP32Client] POST ${this.baseUrl}${url}`);
  const response = await this.fetchWithRetry(url, { method: "POST" });
  if (!response.ok) {
    throw new Error(`Failed to power on: ${response.statusText}`);
  }
  console.log(`[ESP32Client] ✅ System power ON initiated`);
}

/**
 * Выключить всю систему
 */
async powerOff(): Promise<void> {
  const url = `/api/power/off`;
  console.log(`[ESP32Client] POST ${this.baseUrl}${url}`);
  const response = await this.fetchWithRetry(url, { method: "POST" });
  if (!response.ok) {
    throw new Error(`Failed to power off: ${response.statusText}`);
  }
  console.log(`[ESP32Client] ✅ System power OFF`);
}

/**
 * Получить статус питания
 */
async getPowerStatus(): Promise<{ pc: boolean; actuators: boolean }> {
  const response = await this.fetchWithRetry("/api/power/status", { method: "GET" });
  if (!response.ok) {
    throw new Error(`Failed to get power status: ${response.statusText}`);
  }
  return response.json();
}
```

### В components/actuator-control.tsx добавь кнопки:

```typescript
<div className="flex gap-4 mb-6">
  <button
    onClick={async () => {
      await client.powerOn();
      // Показать сообщение "PC starting, wait 30s..."
    }}
    className="flex-1 bg-green-600 hover:bg-green-700 text-white px-6 py-4 rounded-lg font-bold text-lg"
  >
    🔋 POWER ON SYSTEM
  </button>

  <button
    onClick={async () => {
      if (confirm("Выключить всю систему?")) {
        await client.powerOff();
      }
    }}
    className="flex-1 bg-red-600 hover:bg-red-700 text-white px-6 py-4 rounded-lg font-bold text-lg"
  >
    ⚡ POWER OFF SYSTEM
  </button>
</div>
```

---

## 📋 Логика работы

### Включение (Power ON):

1. **Пользователь нажимает кнопку** (физическую или в приложении)
2. **ESP32 включает Relay #2** → Компьютер + Монитор получают питание
3. **Ждет 30 секунд** → Windows загружается
4. **ESP32 включает SSR Relay #1** → Актуаторы + LED получают питание
5. **Приложение автоматически стартует** (Electron в автозагрузке)
6. **Приложение подключается к ESP32** → Всё готово!

### Выключение (Power OFF):

1. **Пользователь нажимает кнопку**
2. **ESP32 останавливает все блоки** → Mega команды STOP
3. **ESP32 выключает SSR Relay #1** → Актуаторы отключаются
4. **Ждет 1 секунду**
5. **ESP32 выключает Relay #2** → Компьютер отключается (жестко)

---

## ⚠️ Важно!

### Windows должен быть настроен:

1. **BIOS:** "Restore AC Power Loss" = **ON** (автовключение при подаче питания)
2. **Windows:** Отключить спящий режим
3. **Electron:** Добавить в автозагрузку

### GPIO доступность:

ESP32 GPIO используемые:
- ✅ GPIO 19 - SSR Relay (Actuators Power) - СВОБОДЕН
- ✅ GPIO 18 - Relay (PC Power) - СВОБОДЕН
- ✅ GPIO 4 - Power Button - СВОБОДЕН

---

## 🎉 Готово!

Теперь вся система управляется **одной кнопкой**:
- Физической (на корпусе)
- Или в приложении

**Одним кликом включается:**
1. Компьютер + монитор
2. Ждет 30 сек
3. Актуаторы + LED
4. Приложение стартует автоматически
5. Всё готово к работе!
