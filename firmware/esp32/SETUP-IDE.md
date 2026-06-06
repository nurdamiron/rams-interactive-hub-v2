# Настройка IDE для ESP32 - Пошаговая инструкция

## 🎯 Выбор IDE

Есть два варианта:
1. **PlatformIO + VS Code** (рекомендуется) - профессиональная среда
2. **Arduino IDE** (проще) - для начинающих

---

## ✨ ВАРИАНТ 1: PlatformIO + VS Code (Рекомендуется)

### Преимущества PlatformIO:
- ✅ Автоматическая установка библиотек
- ✅ Лучший редактор кода (VS Code)
- ✅ Встроенный Serial Monitor
- ✅ Управление версиями библиотек
- ✅ Поддержка множества платформ

### Шаг 1: Установить VS Code

#### Mac:
```bash
# Через Homebrew
brew install --cask visual-studio-code

# Или скачать с https://code.visualstudio.com/
```

#### Windows:
1. Скачать: https://code.visualstudio.com/
2. Запустить установщик
3. Установить с настройками по умолчанию

#### Linux:
```bash
# Ubuntu/Debian
sudo snap install code --classic

# Или через apt
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/trusted.gpg.d/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" > /etc/apt/sources.list.d/vscode.list'
sudo apt update
sudo apt install code
```

### Шаг 2: Установить PlatformIO Extension

1. **Открыть VS Code**

2. **Открыть Extensions** (или нажать `Cmd+Shift+X` на Mac, `Ctrl+Shift+X` на Windows):
   - Слева появится панель Extensions

3. **Найти PlatformIO IDE**:
   - В поиске ввести: `PlatformIO IDE`
   - Должно быть от автора "PlatformIO"
   - Нажать **Install**

4. **Дождаться установки** (2-5 минут):
   - Установится PlatformIO Core
   - Установятся зависимости
   - Появится иконка "дома" (PlatformIO Home) слева

5. **Перезагрузить VS Code**:
   - Нажать `Cmd+Q` (Mac) или закрыть окно
   - Открыть снова

### Шаг 3: Открыть проект ESP32

1. **Открыть папку проекта**:
   ```
   File → Open Folder...
   → Выбрать: /Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub/firmware/esp32/rams_controller
   ```

2. **PlatformIO распознает проект**:
   - Увидит файл `platformio.ini`
   - Автоматически загрузит платформу ESP32
   - Установит библиотеки (FastLED, ESPAsyncWebServer, ArduinoJson)

3. **Дождаться завершения**:
   - В нижней панели появится статус
   - "PlatformIO: Installing dependencies..."
   - Может занять 5-10 минут при первом запуске

### Шаг 4: Подключить ESP32

1. **Подключить ESP32 к компьютеру через USB**

2. **Проверить порт**:

   **Mac**:
   ```bash
   ls /dev/tty.*
   # Должно быть: /dev/tty.usbserial-xxx или /dev/tty.SLAB_USBtoUART
   ```

   **Windows**:
   - Диспетчер устройств → Порты (COM и LPT)
   - Должно быть: COM3, COM4, и т.д.

   **Linux**:
   ```bash
   ls /dev/ttyUSB*
   # Должно быть: /dev/ttyUSB0
   ```

3. **Если не видит ESP32** - установить драйвер:

   **Mac**:
   ```bash
   # CP210x драйвер (для большинства ESP32)
   brew install --cask silicon-labs-vcp-driver
   ```

   **Windows**:
   - Скачать: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
   - Установить

   **Linux**:
   ```bash
   # Обычно драйвер уже есть
   # Добавить права:
   sudo usermod -a -G dialout $USER
   # Перезайти в систему
   ```

### Шаг 5: Настроить platformio.ini

Откройте файл `platformio.ini` и проверьте настройки:

```ini
[platformio]
default_envs = esp32dev

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

monitor_speed = 115200
upload_speed = 921600

lib_deps =
    fastled/FastLED @ ^3.6.0
    me-no-dev/ESPAsyncWebServer @ ^1.2.3
    me-no-dev/AsyncTCP @ ^1.1.1
    bblanchon/ArduinoJson @ ^6.21.5
```

**Если нужно изменить порт** (опционально):
```ini
upload_port = /dev/tty.usbserial-0001  ; Mac
# upload_port = COM3  ; Windows
# upload_port = /dev/ttyUSB0  ; Linux

monitor_port = /dev/tty.usbserial-0001  ; Mac
```

### Шаг 6: Компиляция и загрузка

#### В PlatformIO есть несколько кнопок внизу:

| Иконка | Действие | Горячая клавиша |
|--------|----------|-----------------|
| ✓ | Compile (собрать) | `Cmd+Alt+B` |
| → | Upload (загрузить) | `Cmd+Alt+U` |
| 🔌 | Serial Monitor | `Cmd+Alt+S` |
| 🗑️ | Clean | `Cmd+Alt+C` |

#### Пошаговая компиляция:

1. **Нажать кнопку ✓ (Compile)**:
   ```
   Processing esp32dev (platform: espressif32; board: esp32dev; framework: arduino)
   ...
   Building .pio/build/esp32dev/firmware.bin
   ...
   SUCCESS
   ```

2. **Если ошибки компиляции**:
   - Проверить что все библиотеки установлены
   - PlatformIO → Rebuild (очистить и пересобрать)

3. **Загрузить на ESP32** - нажать кнопку → (Upload):
   ```
   Configuring upload protocol...
   Uploading .pio/build/esp32dev/firmware.bin
   ...
   Leaving... Hard resetting via RTS pin...
   SUCCESS
   ```

4. **Открыть Serial Monitor** - нажать кнопку 🔌:
   ```
   --- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
   --- More details at http://bit.ly/pio-monitor-filters
   --- Miniterm on /dev/tty.usbserial-0001  115200,8,N,1 ---
   --- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---

   ========================================
     RAMS Interactive Hub - ESP32
     Hardware Controller v1.0
   ========================================
   [SETUP] Configuring GPIO pins...
   [SETUP] GPIO pins configured ✓
   ...
   ```

✅ **Готово! PlatformIO настроен!**

---

## 🔧 ВАРИАНТ 2: Arduino IDE

### Преимущества Arduino IDE:
- ✅ Проще для начинающих
- ✅ Легкий интерфейс
- ✅ Быстрая установка

### Шаг 1: Установить Arduino IDE

1. **Скачать Arduino IDE 2.x**:
   - https://www.arduino.cc/en/software
   - Выбрать версию для вашей ОС

2. **Установить**:
   - Mac: Перетащить в Applications
   - Windows: Запустить установщик
   - Linux: Распаковать и запустить

### Шаг 2: Добавить поддержку ESP32

1. **Открыть настройки**:
   - Mac: `Arduino IDE → Settings`
   - Windows/Linux: `File → Preferences`

2. **Добавить URL для ESP32**:
   - Найти поле "Additional Boards Manager URLs"
   - Вставить:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
   - Нажать OK

3. **Установить ESP32 платформу**:
   - `Tools → Board → Boards Manager...`
   - В поиске ввести: `esp32`
   - Найти "esp32 by Espressif Systems"
   - Нажать **Install** (будет загружено ~500MB)
   - Дождаться установки (5-10 минут)

### Шаг 3: Установить библиотеки

1. **Открыть Library Manager**:
   - `Tools → Manage Libraries...`

2. **Установить библиотеки по очереди**:

   #### FastLED:
   - Ввести в поиск: `FastLED`
   - Найти "FastLED by Daniel Garcia"
   - Нажать **Install**

   #### ArduinoJson:
   - Ввести: `ArduinoJson`
   - Найти "ArduinoJson by Benoit Blanchon"
   - Установить **версию 6.x** (не 7.x!)

   #### ESPAsyncWebServer:
   - **ВАЖНО**: Эту библиотеку нужно установить вручную!

3. **Установить ESPAsyncWebServer вручную**:

   **Вариант A: Через Git (если установлен)**:
   ```bash
   cd ~/Documents/Arduino/libraries
   git clone https://github.com/me-no-dev/ESPAsyncWebServer.git
   git clone https://github.com/me-no-dev/AsyncTCP.git
   ```

   **Вариант B: Скачать ZIP**:
   - Скачать:
     - https://github.com/me-no-dev/ESPAsyncWebServer/archive/refs/heads/master.zip
     - https://github.com/me-no-dev/AsyncTCP/archive/refs/heads/master.zip
   - Arduino IDE: `Sketch → Include Library → Add .ZIP Library...`
   - Выбрать скачанные ZIP файлы

4. **Перезапустить Arduino IDE**

### Шаг 4: Открыть проект

1. **Открыть файл**:
   ```
   File → Open...
   → /Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub/firmware/esp32/rams_controller/rams_controller.ino
   ```

2. **Должен открыться код прошивки**

### Шаг 5: Настроить плату и порт

1. **Выбрать плату**:
   ```
   Tools → Board → esp32 → ESP32 Dev Module
   ```

2. **Настроить параметры**:
   ```
   Tools → Upload Speed → 921600
   Tools → CPU Frequency → 240MHz
   Tools → Flash Frequency → 80MHz
   Tools → Flash Mode → QIO
   Tools → Flash Size → 4MB (32Mb)
   Tools → Partition Scheme → Default 4MB with spiffs
   Tools → Core Debug Level → None
   ```

3. **Выбрать порт**:
   - Подключить ESP32 к USB
   - `Tools → Port → /dev/tty.usbserial-xxx` (Mac)
   - `Tools → Port → COM3` (Windows)
   - `Tools → Port → /dev/ttyUSB0` (Linux)

### Шаг 6: Компиляция и загрузка

1. **Проверить код** (кнопка ✓):
   - Нажать кнопку с галочкой вверху слева
   - Дождаться "Done compiling"
   - Проверить что нет ошибок

2. **Загрузить на ESP32** (кнопка →):
   - Нажать кнопку со стрелкой
   - Дождаться "Done uploading"

3. **Открыть Serial Monitor**:
   - `Tools → Serial Monitor`
   - Выбрать скорость: **115200 baud**
   - Должны увидеть логи:
   ```
   ========================================
     RAMS Interactive Hub - ESP32
   ========================================
   ```

✅ **Готово! Arduino IDE настроен!**

---

## 🎨 Настройка конфигурации

### Изменить WiFi настройки

Откройте `rams_controller.ino` и найдите:

```cpp
// WiFi Settings
const char* WIFI_SSID = "RAMS_Hub";              // ← Изменить на ваш WiFi
const char* WIFI_PASSWORD = "RamsInteractive2026"; // ← Изменить пароль
IPAddress local_IP(192, 168, 1, 100);            // ← Изменить IP если нужно
```

### Изменить количество LED

```cpp
// LED Configuration
#define NUM_LEDS          200       // ← Количество АДРЕСОВ (не физических LED!)
                                    // Для 12V: физические LED ÷ 3
```

### Изменить GPIO пины

```cpp
// Pin Configuration
#define LED_PIN           16        // ← Data line для LED
#define RELAY_1_PIN       17        // ← Реле #1
#define RELAY_2_PIN       18        // ← Реле #2
#define RELAY_3_PIN       19        // ← Реле #3
#define ACTUATOR_UP_PIN   21        // ← Актуатор вверх
#define ACTUATOR_DOWN_PIN 22        // ← Актуатор вниз
```

---

## 🐛 Troubleshooting

### Ошибка: "Board not found"

**Arduino IDE**:
- Убедитесь что ESP32 платформа установлена
- Перезапустите Arduino IDE
- Проверьте что URL для ESP32 добавлен в Preferences

**PlatformIO**:
- Удалить папку `.pio` в проекте
- PlatformIO → Clean
- PlatformIO → Build (пересоберет)

### Ошибка: "Library not found"

**Arduino IDE**:
- Установить библиотеку через Library Manager
- Для ESPAsyncWebServer - установить вручную (см. выше)

**PlatformIO**:
- Проверить `platformio.ini` → секция `lib_deps`
- PlatformIO → Clean
- PlatformIO → Build

### Ошибка: "Serial port not found"

1. **Проверить что ESP32 подключен**:
   ```bash
   # Mac
   ls /dev/tty.*

   # Linux
   ls /dev/ttyUSB*
   ```

2. **Установить драйвер**:
   - CP210x или CH340 (зависит от платы)

3. **Дать права** (Linux):
   ```bash
   sudo usermod -a -G dialout $USER
   # Перезайти
   ```

### Ошибка компиляции

1. **Проверить версии библиотек**:
   - ArduinoJson должен быть **6.x** (не 7.x)
   - FastLED должен быть 3.6.0+

2. **Очистить и пересобрать**:
   - Arduino IDE: `Sketch → Clean Build`
   - PlatformIO: кнопка 🗑️ Clean

3. **Проверить код**:
   - Нет опечаток в конфигурации
   - Все `#include` на месте

---

## 📝 Полезные команды

### PlatformIO CLI (опционально)

Если хотите использовать терминал:

```bash
# Перейти в папку проекта
cd /Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub/firmware/esp32/rams_controller

# Собрать проект
pio run

# Загрузить на ESP32
pio run --target upload

# Открыть Serial Monitor
pio device monitor

# Очистить
pio run --target clean

# Показать порты
pio device list
```

### Arduino CLI (опционально)

```bash
# Установить Arduino CLI
brew install arduino-cli  # Mac
# или скачать: https://arduino.github.io/arduino-cli/

# Установить ESP32 платформу
arduino-cli core install esp32:esp32

# Собрать
arduino-cli compile --fqbn esp32:esp32:esp32 rams_controller.ino

# Загрузить
arduino-cli upload -p /dev/tty.usbserial-0001 --fqbn esp32:esp32:esp32 rams_controller.ino

# Serial Monitor
arduino-cli monitor -p /dev/tty.usbserial-0001 -b 115200
```

---

## 🎉 Готово!

Теперь у вас настроена IDE для разработки на ESP32!

**Следующий шаг**:
1. Изменить WiFi настройки в коде
2. Скомпилировать (кнопка ✓)
3. Загрузить на ESP32 (кнопка →)
4. Открыть Serial Monitor и проверить логи

**См. также**:
- `STEP-BY-STEP-INSTALL.md` - Полная установка хардвера
- `README.md` - Документация прошивки

---

**Дата**: 2026-01-27
**Версия**: 1.0
