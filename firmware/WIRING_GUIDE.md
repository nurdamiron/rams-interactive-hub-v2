# Схема подключения ESP32 + 2x Arduino Mega

## Общая схема системы

```
                    ┌─────────────────┐
                    │    ESP32-WROOM  │
                    │   DevKit v1     │
                    └────────┬─┬──────┘
                            │ │
            ┌───────────────┘ └────────────────┐
            │                                   │
      Serial2 (TX2/RX2)                   Serial1 (TX1/RX1)
            │                                   │
    ┌───────▼──────────┐              ┌────────▼─────────┐
    │ Arduino Mega #1  │              │ Arduino Mega #2  │
    │  (Blocks 1-8)    │              │  (Blocks 9-15)   │
    │  16 actuators    │              │  15 actuators    │
    └──────────────────┘              └──────────────────┘
```

---

## 🔌 ESP32 → Arduino Mega #1 (блоки 1-8)

### Соединение через UART Serial2

| ESP32 Pin | Mega #1 Pin | Сигнал | Описание |
|-----------|-------------|--------|----------|
| **GPIO 17** | **Pin 19 (RX1)** | TX → RX | ESP32 передаёт команды в Mega #1 |
| **GPIO 16** | **Pin 18 (TX1)** | RX ← TX | ESP32 получает ответы от Mega #1 |
| **GND** | **GND** | Ground | Общий GND (обязательно!) |

### Как паять:

1. **ESP32 GPIO 17** → провод → **Mega #1 Pin 19 (RX1)**
2. **ESP32 GPIO 16** → провод → **Mega #1 Pin 18 (TX1)**
3. **ESP32 GND** → провод → **Mega #1 GND**

```
ESP32                    Arduino Mega #1
────────────────────────────────────────
GPIO 17 (TX2) ─────────► Pin 19 (RX1)
GPIO 16 (RX2) ◄───────── Pin 18 (TX1)
GND           ─────────── GND
```

⚠️ **Важно:**
- Используй **TX1/RX1** на Mega (pins 18/19), **НЕ** TX0/RX0!
- TX0/RX0 заняты для USB Serial Monitor
- Бод-рейт: **115200**

---

## 🔌 ESP32 → Arduino Mega #2 (блоки 9-15)

### Соединение через UART Serial1

| ESP32 Pin | Mega #2 Pin | Сигнал | Описание |
|-----------|-------------|--------|----------|
| **GPIO 25** | **Pin 19 (RX1)** | TX → RX | ESP32 передаёт команды в Mega #2 |
| **GPIO 26** | **Pin 18 (TX1)** | RX ← TX | ESP32 получает ответы от Mega #2 |
| **GND** | **GND** | Ground | Общий GND (обязательно!) |

### Как паять:

1. **ESP32 GPIO 25** → провод → **Mega #2 Pin 19 (RX1)**
2. **ESP32 GPIO 26** → провод → **Mega #2 Pin 18 (TX1)**
3. **ESP32 GND** → провод → **Mega #2 GND**

```
ESP32                    Arduino Mega #2
────────────────────────────────────────
GPIO 25 (TX1) ─────────► Pin 19 (RX1)
GPIO 26 (RX1) ◄───────── Pin 18 (TX1)
GND           ─────────── GND
```

---

## 📝 Полная таблица соединений

| Компонент | Pin | → | Компонент | Pin | Назначение |
|-----------|-----|---|-----------|-----|------------|
| ESP32 | GPIO 17 | → | Mega #1 | Pin 19 (RX1) | ESP32 → Mega #1 команды |
| ESP32 | GPIO 16 | ← | Mega #1 | Pin 18 (TX1) | ESP32 ← Mega #1 ответы |
| ESP32 | GND | ─ | Mega #1 | GND | Общий GND |
| ESP32 | GPIO 25 | → | Mega #2 | Pin 19 (RX1) | ESP32 → Mega #2 команды |
| ESP32 | GPIO 26 | ← | Mega #2 | Pin 18 (TX1) | ESP32 ← Mega #2 ответы |
| ESP32 | GND | ─ | Mega #2 | GND | Общий GND |

---

## 🔧 Необходимые материалы

### Провода:
- **6 проводов** (лучше разноцветные для удобства)
  - 2x красный/жёлтый (TX сигналы)
  - 2x зелёный/синий (RX сигналы)
  - 2x чёрный (GND)

### Рекомендация:
- Длина: **20-30 см** (в зависимости от расположения плат)
- Тип: **Dupont провода (папа-мама)** или паять напрямую
- Сечение: **0.25-0.5 мм²** (тонкие сигнальные провода)

---

## 🛠️ Инструкция по пайке

### Вариант 1: Dupont кабели (без пайки)
1. Купи Dupont кабели папа-папа или папа-мама
2. Подключи согласно таблице выше
3. Готово!

### Вариант 2: Пайка проводов напрямую
1. **Зачисти концы проводов** (~5 мм)
2. **ESP32 сторона:**
   - Припаяй провода к пинам GPIO 16, 17, 25, 26, GND
3. **Mega #1 сторона:**
   - Припаяй к pins 18, 19, GND
4. **Mega #2 сторона:**
   - Припаяй к pins 18, 19, GND
5. **Используй термоусадку** для изоляции соединений

---

## ⚡ Питание

### Важно: Общий GND обязателен!
- Если все платы питаются от разных источников → **обязательно** соедини GND между ними
- Если все от одного источника (например USB hub) → GND уже общий, но дополнительный провод не помешает

### Рекомендуемая схема питания:

```
USB Hub / Блок питания
    │
    ├──► ESP32 (5V + GND)
    ├──► Mega #1 (5V + GND)
    └──► Mega #2 (5V + GND)
```

Или:
- **ESP32:** USB от компьютера
- **Mega #1:** отдельный USB адаптер
- **Mega #2:** отдельный USB адаптер
- **GND:** соединить между всеми тремя платами!

---

## 🧪 Проверка подключения

### 1. Загрузи прошивки:
- **Mega #1:** `firmware/arduino_mega/actuator_mega1/actuator_mega1.ino`
- **Mega #2:** `firmware/arduino_mega/actuator_mega2/actuator_mega2.ino`
- **ESP32:** `firmware/esp32/unified_controller/unified_controller.ino`

### 2. Открой Serial Monitor ESP32 (115200 baud)

Ты должен увидеть:
```
========================================
  RAMS UNIFIED CONTROLLER v2.0
  ESP32 + 2x Arduino Mega + WS2812B
========================================
[LED] Initializing FastLED...
[ZONES] Configuring block → LED zones mapping...
[MEGA] Mega #1 ready on GPIO16/17 ✓
[MEGA] Mega #2 ready on GPIO26/25 ✓
[WIFI] AP Started: RAMS_Controller
[WIFI] IP: 192.168.4.1
[READY] System initialized!
```

### 3. Протестируй команду:

**HTTP POST** на `http://192.168.4.1/api/block`:
```json
{
  "block": 1,
  "action": "up",
  "duration": 5000
}
```

В Serial Monitor должен появиться:
```
[QUEUE] Added: Block 1 up (5000ms) [Queue: 1]
[QUEUE] Started: Block 1 up (5000ms) [Remaining: 0]
[MEGA #1] → {"block":1,"action":"up","duration":5000}
[LED] Block 1 zone ON (strip 1)
```

И на Mega #1 Serial Monitor:
```
[CMD] Received: {"block":1,"action":"up","duration":5000}
[BLOCK 1] → UP (5000ms, 2 actuators)
[AUTO-STOP] Block 1
```

---

## 📌 Схема пинов ESP32 DevKit v1

```
                 ┌─────────────────┐
                 │  ESP32 DevKit   │
                 │                 │
   3V3 ──────────┤ 3V3         GND ├──────── GND (общий!)
                 ├                 │
                 ├ GPIO 16     GND │
                 │   RX2 ◄─────────┼─ от Mega #1 TX1
                 ├ GPIO 17         │
                 │   TX2 ──────────┼─► в Mega #1 RX1
                 │                 │
                 ├ GPIO 25         │
                 │   TX1 ──────────┼─► в Mega #2 RX1
                 ├ GPIO 26         │
                 │   RX1 ◄─────────┼─ от Mega #2 TX1
                 │                 │
   GND (общий) ──┤ GND         GND ├─ GND (к Mega #1, #2)
                 └─────────────────┘
```

---

## ⚠️ Типичные ошибки

### ❌ Ошибка: Нет связи между ESP32 и Mega
**Причина:**
- Перепутаны TX/RX провода
- Не подключён общий GND
- Разные baud rates

**Решение:**
- TX ESP32 → RX Mega (обязательно!)
- RX ESP32 ← TX Mega (обязательно!)
- Соедини GND между всеми платами
- Проверь: везде 115200 baud

---

### ❌ Ошибка: Команды не доходят до Mega
**Причина:**
- Использованы pins TX0/RX0 вместо TX1/RX1 на Mega
- Serial Monitor открыт на Mega → блокирует Serial1

**Решение:**
- Используй **TX1 (pin 18)** и **RX1 (pin 19)** на Mega
- Закрой Serial Monitor на Mega при работе через ESP32

---

## ✅ Итоговый чеклист

- [ ] ESP32 GPIO 17 → Mega #1 Pin 19 (RX1)
- [ ] ESP32 GPIO 16 ← Mega #1 Pin 18 (TX1)
- [ ] ESP32 GPIO 25 → Mega #2 Pin 19 (RX1)
- [ ] ESP32 GPIO 26 ← Mega #2 Pin 18 (TX1)
- [ ] GND соединён между ESP32, Mega #1, Mega #2
- [ ] Прошивки загружены на все 3 платы
- [ ] Baud rate везде 115200
- [ ] ESP32 создаёт WiFi сеть "RAMS_Controller"
- [ ] Тест команды через API работает

---

## 🚀 После подключения

Когда всё подключено и работает:
1. Подключись к WiFi **RAMS_Controller** (пароль: `rams2026interactive`)
2. Открой браузер: **http://192.168.4.1**
3. Используй API для управления блоками
4. LED зоны будут подсвечиваться синхронно с актуаторами!

**Готово! Теперь у тебя единая система ESP32 + 2 Mega + LEDs! 🔥**
