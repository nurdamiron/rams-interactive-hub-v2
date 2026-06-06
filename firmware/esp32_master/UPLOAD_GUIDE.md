# ESP32 Firmware Upload Guide

## Способ 1: OTA (Over-The-Air) - через WiFi ✨

**Преимущества:** Удобно, не нужно подключать USB кабель

**Требования:**
- ESP32 подключен к WiFi (Rams_WIFI)
- Компьютер в той же сети

### Использование скрипта (рекомендуется):

```bash
cd firmware/esp32_master
./upload_ota.sh
```

Или с другим IP:
```bash
./upload_ota.sh 192.168.110.65
```

### Использование PlatformIO напрямую:

```bash
cd firmware/esp32_master
pio run -t upload
```

**Примечание:** OTA может быть нестабильным при больших размерах прошивки (>850KB) или слабом WiFi сигнале. Если OTA падает на 60-99%, используйте USB метод.

---

## Способ 2: USB - через кабель 🔌

**Преимущества:** 100% надежно, работает всегда

**Требования:**
- USB кабель подключен к ESP32
- Драйвер CH340/CP2102 установлен

### Шаги:

1. Подключите USB кабель к ESP32
2. Найдите порт:
```bash
ls /dev/cu.* | grep usb
```

3. Прошейте:
```bash
cd firmware/esp32_master
pio run -t upload --upload-port /dev/cu.usbserial-0001
```

Замените `/dev/cu.usbserial-0001` на ваш порт.

---

## Переключение между USB и OTA

### Для OTA в `platformio.ini`:
```ini
upload_protocol = espota
upload_port = 192.168.110.65
upload_flags =
    --auth=rams2024
    --timeout=60
```

### Для USB в `platformio.ini`:
Закомментируйте строки OTA:
```ini
; upload_protocol = espota
; upload_port = 192.168.110.65
; upload_flags =
;     --auth=rams2024
;     --timeout=60
```

Или просто укажите USB порт явно:
```bash
pio run -t upload --upload-port /dev/cu.usbserial-0001
```

---

## Troubleshooting

### OTA падает на 2-99%
- **Причина:** Слабый WiFi сигнал, ESP32 занят обработкой LED/блоков
- **Решение:** Используйте USB upload

### "Port already in use" / "Listen Failed"
- **Причина:** Старый процесс OTA еще работает
- **Решение:** Подождите 30 сек или перезагрузите ESP32

### "Host not found"
- **Причина:** ESP32 не в сети или неправильный IP
- **Решение:** Проверьте статус через `curl http://192.168.110.65/api/status`

### "Auth Failed"
- **Причина:** Неправильный пароль OTA
- **Решение:** Пароль должен быть `rams2024`

---

## Текущая конфигурация

- **ESP32 IP (STA):** 192.168.110.65
- **ESP32 IP (AP):** 192.168.4.1
- **OTA Password:** rams2024
- **SSID:** Rams_WIFI
- **WiFi Password:** Rams2021
- **Firmware Size:** ~853KB
- **USB Port (обычно):** /dev/cu.usbserial-0001

---

## Последние изменения в прошивке

✅ **Исправлена LED подсветка блоков** (main.cpp:479-486, 655-672):
- UP блоки больше НЕ подсвечиваются (LED эффекты продолжают работать)
- DOWN блоки показывают мягкий оранжевый blend (70% эффект + 30% оранжевый)
- При OTA прошивке LED анимации автоматически останавливаются для стабильности

✅ **Правильная конфигурация пинов**:
- Mega #1: GPIO 25 (TX) / GPIO 26 (RX)
- Mega #2: GPIO 16 (TX) / GPIO 17 (RX)

✅ **Все 8 LED эффектов работают**:
- Static, Pulse, Rainbow, Chase, Sparkle, Wave, Fire, Meteor
