# Настройка рабочего места для прошивки (чистый Mac)

## 1. Установить Homebrew (если нету)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

После установки выполнить команду, которую Homebrew покажет в конце (добавить в PATH).

## 2. Установить Python и pipx

```bash
brew install python pipx
pipx ensurepath
```

Закрыть и открыть терминал заново.

## 3. Установить PlatformIO CLI

```bash
pipx install platformio
```

Проверить:
```bash
pio --version
```

## 4. Установить драйверы USB (если ESP32/Mega не определяются)

### Для ESP32 с чипом CP2102:
```bash
brew install --cask silicon-labs-vcp-driver
```

### Для ESP32/Mega с чипом CH340:
```bash
brew install --cask wch-ch34x-usb-serial-driver
```

> Как узнать какой чип: посмотреть на плату рядом с USB разъёмом.
> CP2102 — маленький чип с надписью "CP2102" или "SILABS".
> CH340 — чип с надписью "CH340G" или "CH340C".

После установки драйвера — **перезагрузить Mac**.

## 5. Склонировать проект

```bash
cd ~/Desktop
git clone <URL_ВАШЕГО_РЕПОЗИТОРИЯ> rams-screen
```

Или скопировать папку `firmware/` на флешке.

## 6. Установить зависимости (платформы и тулчейны)

```bash
# ESP32
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/esp32_master
pio pkg install

# Mega #1
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega1_relay
pio pkg install

# Mega #2
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega2_relay
pio pkg install
```

Это скачает ~500MB тулчейнов (компиляторы для ESP32 и AVR).

## 7. Настроить WiFi

Открыть файл `firmware/esp32_master/src/main.cpp` и заменить:

```cpp
const char* WIFI_SSID     = "YOUR_WIFI_SSID";      // ← Имя WiFi сети
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";   // ← Пароль WiFi
```

## 8. Проверить что реле Active LOW или HIGH

Открыть файл `firmware/shared/protocol.h` и проверить:

```cpp
#define RELAY_ACTIVE_LOW true   // true = синие модули с оптопарой (LOW = ON)
                                 // false = простые реле (HIGH = ON)
```

> Как проверить: подключить реле модуль к 5V и GND.
> Подать LOW на IN1 — если реле щёлкнуло → Active LOW (true).
> Подать HIGH на IN1 — если реле щёлкнуло → Active HIGH (false).

## 9. Собрать прошивки (проверка)

```bash
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/esp32_master && pio run
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega1_relay && pio run
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega2_relay && pio run
```

Все 3 должны показать `[SUCCESS]`.

## 10. Прошить устройства

### Подключить ESP32 по USB:

```bash
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/esp32_master
pio run -t upload
```

> Если не определяет порт: `pio device list` покажет доступные порты.
> Указать порт вручную: `pio run -t upload --upload-port /dev/cu.usbserial-XXXX`

> ESP32 при прошивке нужно перевести в boot mode:
> Зажать кнопку BOOT → нажать и отпустить EN → отпустить BOOT.
> Некоторые платы делают это автоматически.

### Подключить Arduino Mega #1 по USB:

```bash
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega1_relay
pio run -t upload
```

### Подключить Arduino Mega #2 по USB:

```bash
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega2_relay
pio run -t upload
```

## 11. Мониторинг (Serial Monitor)

Смотреть логи с устройства:

```bash
# ESP32
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/esp32_master
pio device monitor

# Mega #1
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega1_relay
pio device monitor

# Mega #2
cd ~/Desktop/rams-screen/rams-interactive-hub/firmware/mega2_relay
pio device monitor
```

Выход из монитора: `Ctrl+C`

## 12. Тестирование без физического железа

Можно отправить UDP команды с Mac для проверки ESP32:

```bash
# Узнать IP ESP32 (смотреть в Serial Monitor при загрузке)
# Отправить команду:
echo -n "PING" | nc -u -w1 <ESP32_IP> 4210
echo -n "BLOCK:1:UP" | nc -u -w1 <ESP32_IP> 4210
echo -n "ALL:STOP" | nc -u -w1 <ESP32_IP> 4210
echo -n "STATUS" | nc -u -w1 <ESP32_IP> 4210
```

## Troubleshooting

| Проблема | Решение |
|----------|---------|
| `pio: command not found` | Закрыть и открыть терминал, или `pipx ensurepath` |
| `No device found` | Установить драйвер (шаг 4), проверить кабель (data cable, не зарядный) |
| ESP32 не прошивается | Зажать BOOT, нажать EN, отпустить BOOT |
| `Permission denied /dev/cu.*` | `sudo chmod 666 /dev/cu.usbserial-*` |
| Mega не определяется | Попробовать другой USB порт, проверить кабель |
| Build failed | `pio pkg install` в папке проекта |
