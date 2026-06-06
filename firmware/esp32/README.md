# RAMS Interactive Hub - ESP32 Firmware

Прошивка для ESP32 контроллера интерактивных макетов.

## Возможности

- ✅ WiFi HTTP Server для приема команд
- ✅ Управление WS2812B LED лентами (до 600+ LED)
- ✅ Управление реле (12В нагрузка)
- ✅ Управление линейными актуаторами (вверх/вниз)
- ✅ Эффекты: fade, pulse, rainbow, sparkle
- ✅ Debouncing и очередь команд
- ✅ Статический IP адрес
- ✅ Health check endpoint (/ping)

---

## Требования

### Железо
- ESP32 DevKit (любая версия с WiFi)
- WS2812B LED ленты (5V)
- Реле модули (5V логика, 12V коммутация)
- Линейные актуаторы 12V (опционально)
- Блоки питания:
  - 5V/10A для LED
  - 12V/5A для актуаторов
  - USB 5V для ESP32

### Программное обеспечение
- **Arduino IDE** 2.x+ или **PlatformIO**
- USB драйвер для ESP32 (CP210x или CH340)

---

## Установка

### Вариант 1: Arduino IDE

1. **Установить Arduino IDE**: https://www.arduino.cc/en/software

2. **Добавить ESP32 Board Manager**:
   - File → Preferences
   - Additional Board Manager URLs:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager → Найти "esp32" → Install

3. **Установить библиотеки**:
   - Tools → Manage Libraries
   - Установить:
     - `FastLED` by Daniel Garcia
     - `ESPAsyncWebServer` by me-no-dev
     - `AsyncTCP` by me-no-dev
     - `ArduinoJson` by Benoit Blanchon

4. **Открыть прошивку**:
   - File → Open → `rams_controller.ino`

5. **Настроить параметры**:
   - Tools → Board → ESP32 Dev Module
   - Tools → Upload Speed → 921600
   - Tools → Port → (выбрать COM порт ESP32)

6. **Загрузить на ESP32**:
   - Нажать кнопку Upload (→)

### Вариант 2: PlatformIO (Рекомендуется)

1. **Установить PlatformIO**:
   - VS Code → Extensions → Искать "PlatformIO IDE" → Install
   - Или CLI: `pip install platformio`

2. **Открыть проект**:
   ```bash
   cd firmware/esp32/rams_controller
   ```

3. **Собрать проект**:
   ```bash
   pio run
   ```

4. **Загрузить на ESP32**:
   ```bash
   pio run --target upload
   ```

5. **Открыть Serial Monitor**:
   ```bash
   pio device monitor
   ```

---

## Конфигурация

### WiFi настройки

Отредактировать в `rams_controller.ino`:

```cpp
// WiFi Settings
const char* WIFI_SSID = "RAMS_Hub";              // Название сети
const char* WIFI_PASSWORD = "RamsInteractive2026";  // Пароль
IPAddress local_IP(192, 168, 1, 100);            // Статический IP
```

### LED конфигурация

```cpp
#define LED_PIN           16        // GPIO pin для Data line
#define NUM_LEDS          600       // Общее количество LED
#define BRIGHTNESS        200       // Яркость (0-255)
```

### Пины GPIO

```cpp
#define RELAY_1_PIN       17        // Реле #1
#define RELAY_2_PIN       18        // Реле #2
#define RELAY_3_PIN       19        // Реле #3
#define ACTUATOR_UP_PIN   21        // Актуатор вверх
#define ACTUATOR_DOWN_PIN 22        // Актуатор вниз
```

### Маппинг проектов

Настроить в функции `setupProjects()`:

```cpp
// Пример: RAMS GARDEN (id=9)
projects[9].ledStart = 200;   // Начало с LED #200
projects[9].ledCount = 60;    // 60 LED
projects[9].color = CRGB::Green;  // Зеленый цвет
```

---

## Подключение

### Схема подключения

```
┌─────────────────────────────────────────────────┐
│                    ESP32                         │
│                                                  │
│  GPIO 16 ──────────────► WS2812B Data In        │
│  GPIO 17 ──────────────► Relay #1 IN            │
│  GPIO 18 ──────────────► Relay #2 IN            │
│  GPIO 19 ──────────────► Relay #3 IN            │
│  GPIO 21 ──────────────► Actuator UP            │
│  GPIO 22 ──────────────► Actuator DOWN          │
│  GPIO 2  ──────────────► Status LED             │
│                                                  │
│  5V ─────────────────────► VIN                   │
│  GND ────────────────────► GND                   │
└─────────────────────────────────────────────────┘

            │                │              │
            │                │              │
            ▼                ▼              ▼
    ┌──────────────┐ ┌─────────────┐ ┌──────────┐
    │  WS2812B     │ │   Relays    │ │Actuators │
    │  LED Strip   │ │   12V Load  │ │   12V    │
    └──────────────┘ └─────────────┘ └──────────┘
         5V               12V            12V
```

### Подключение WS2812B

1. **Data Line**: GPIO 16 → WS2812B DIN
   - Рекомендуется резистор 470Ω на линии данных
   - Или Level Shifter 3.3V → 5V

2. **Питание**:
   - 5V БП → WS2812B 5V
   - GND БП → WS2812B GND и ESP32 GND (общая земля!)

3. **Защита**:
   - Конденсатор 1000μF на питании LED
   - Предохранитель на линии 5V

### Подключение реле

1. **Управление**:
   - GPIO 17/18/19 → Relay Module IN1/IN2/IN3
   - 5V ESP32 → Relay VCC
   - GND → Relay GND

2. **Нагрузка 12V**:
   - 12V БП (+) → Relay COM
   - Relay NO → Нагрузка (+)
   - Нагрузка (-) → 12V БП (-)

### Подключение актуаторов

1. **Через H-Bridge** (рекомендуется):
   - GPIO 21 → H-Bridge IN1 (вперед)
   - GPIO 22 → H-Bridge IN2 (назад)
   - 12V БП → H-Bridge VCC
   - Actuator → Motor A/B

2. **Через реле** (простой вариант):
   - Relay 1: Управление направлением (полярность)
   - Relay 2: Управление питанием (вкл/выкл)

---

## API Endpoints

### GET /ping

Health check endpoint.

**Response**:
```json
{
  "status": "ok",
  "uptime": 123456
}
```

### GET /status

Получить статус системы.

**Response**:
```json
{
  "connected": true,
  "ip": "192.168.1.100",
  "uptime": 123456,
  "mode": "highlight",
  "activeProject": 9,
  "ledCount": 600,
  "brightness": 200
}
```

### POST /command

Отправить команду на хардвер.

**Request Body**:
```json
{
  "action": "highlight",
  "projectId": 9,
  "data": {
    "brightness": 200,
    "color": "#00FF00",
    "effect": "fade"
  }
}
```

**Response**:
```json
{
  "success": true
}
```

#### Доступные команды

1. **highlight** - Подсветить проект
2. **off** - Выключить проект/все
3. **reset** - Сбросить всё
4. **screensaver** - Запустить заставку
5. **actuator** - Управление актуатором
6. **toggle_element** - Переключить реле

---

## Примеры команд

### cURL

```bash
# Ping
curl http://192.168.1.100/ping

# Status
curl http://192.168.1.100/status

# Highlight project #9
curl -X POST http://192.168.1.100/command \
  -H "Content-Type: application/json" \
  -d '{
    "action": "highlight",
    "projectId": 9,
    "data": {
      "brightness": 200,
      "color": "#FFD700",
      "effect": "fade"
    }
  }'

# Turn off all
curl -X POST http://192.168.1.100/command \
  -H "Content-Type: application/json" \
  -d '{"action": "reset"}'

# Control actuator
curl -X POST http://192.168.1.100/command \
  -H "Content-Type: application/json" \
  -d '{
    "action": "actuator",
    "target": "roof",
    "state": "up"
  }'
```

---

## Отладка

### Serial Monitor

При загрузке прошивки откройте Serial Monitor (115200 baud):

```
========================================
  RAMS Interactive Hub - ESP32
  Hardware Controller v1.0
========================================
[SETUP] Configuring GPIO pins...
[SETUP] GPIO pins configured ✓
[SETUP] Initializing FastLED...
[SETUP] FastLED initialized: 600 LEDs ✓
[SETUP] Configuring projects...
[SETUP] Configured 28 projects ✓
[WIFI] Connecting to WiFi...
[WIFI] SSID: RAMS_Hub
...........
[WIFI] Connected successfully! ✓
[WIFI] IP Address: 192.168.1.100
[WIFI] Signal Strength: -45 dBm
[SERVER] Starting web server...
[SERVER] Web server started on port 80 ✓

[READY] System initialized successfully!
========================================
```

### Индикация статуса

- **Быстрое мигание**: Подключение к WiFi
- **Медленное мигание**: Ожидание команд
- **Постоянно горит**: Команда выполняется

### Логи команд

```
[CMD] Action: highlight | Project: 9
[PROJECT] Highlighted project #9 with color #FFD700

[CMD] Action: actuator
[ACTUATOR] Moving UP: roof
[ACTUATOR] Stopped: roof

[CMD] Action: reset
[SYSTEM] Resetting all hardware...
```

---

## Troubleshooting

### ESP32 не подключается к WiFi

1. Проверить SSID и пароль
2. Проверить доступность сети
3. Попробовать другой роутер
4. Сбросить настройки WiFi:
   ```cpp
   WiFi.disconnect(true);
   ```

### LED не загораются

1. Проверить подключение Data line
2. Проверить питание 5V
3. Добавить level shifter 3.3V → 5V
4. Уменьшить количество LED в конфигурации
5. Проверить общую землю (GND)

### Реле не срабатывают

1. Проверить питание реле (5V)
2. Проверить тип реле (активное LOW/HIGH)
3. Измерить напряжение на GPIO
4. Проверить ток потребления

### Нестабильная работа

1. Добавить конденсаторы на питание
2. Использовать отдельные БП для LED и актуаторов
3. Уменьшить яркость LED
4. Добавить задержки в коде

---

## Производительность

- **LED Refresh Rate**: 120 FPS
- **WiFi Response Time**: < 50ms
- **Command Processing**: < 10ms
- **Max Projects**: 28
- **Max LEDs**: 1000+ (ограничено памятью)

---

## Безопасность

1. **WiFi**: Использовать WPA2 шифрование
2. **Питание**: Отдельные БП для разных компонентов
3. **Предохранители**: На каждой линии 12V
4. **Защита**: Диоды на актуаторах
5. **Заземление**: Общая земля для всех устройств

---

## Дальнейшее развитие

- [ ] OTA обновления (Over-The-Air)
- [ ] Web интерфейс для конфигурации
- [ ] MQTT поддержка
- [ ] Больше эффектов (chaser, theater, etc)
- [ ] Сохранение конфигурации в EEPROM
- [ ] Датчики (температура, движение)
- [ ] Голосовое управление (интеграция с Alexa/Google Home)

---

## Лицензия

RAMS Global © 2026

---

## Контакты

- **Email**: support@rams.global
- **GitHub**: https://github.com/rams-global
- **Документация**: `/docs/HARDWARE-ARCHITECTURE.md`
