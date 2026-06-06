# 🚀 Быстрый старт: ESP32 + Arduino Mega

## Как это работает

```
Приложение → ESP32 (WiFi) → Arduino Mega (Serial) → Актуаторы
```

---

## 📥 Подключение

```
ESP32 Pin 17 (TX) ──→ Mega Pin 19 (RX1)
ESP32 Pin 16 (RX) ←── Mega Pin 18 (TX1)
ESP32 GND ─────────── Mega GND
```

---

## 💾 Загрузка прошивок

### 1. Arduino Mega
```
Файл: firmware/arduino_mega/actuator_test/actuator_test.ino
Board: Arduino Mega
Baud: 115200
```

### 2. ESP32
```
Файл: firmware/esp32/rams_controller/rams_controller.ino
Board: ESP32 Dev Module
Baud: 115200
```

---

## 🧪 Тест 1: Проверка Mega (без ESP32)

1. **Подключи Mega к USB**
2. **Открой Serial Monitor** (115200 baud)
3. **Отправь команду:**
   ```json
   {"block":1,"action":"up","duration":5000}
   ```
4. **Блок 1 должен подняться на 5 секунд и остановиться**

---

## 🧪 Тест 2: Проверка ESP32 → Mega

1. **Подключи провода** (TX-RX, RX-TX, GND-GND)
2. **Загрузи прошивку на ESP32**
3. **Подожди подключения к WiFi** (IP: 192.168.1.100)
4. **Отправь команду через WiFi:**
   ```bash
   curl -X POST http://192.168.1.100/command \
     -H "Content-Type: application/json" \
     -d '{"action":"actuator","target":"block_1","state":"up","data":{"duration":5000}}'
   ```
5. **Блок 1 должен подняться**

---

## 🎯 Команды

### Из приложения (TypeScript)

```typescript
// Поднять блок 1 на 12 секунд
await hardwareService.sendCommand({
  action: 'actuator',
  target: 'block_1',
  state: 'up',
  data: { duration: 12000 }
});

// Опустить блок 3
await hardwareService.sendCommand({
  action: 'actuator',
  target: 'block_3',
  state: 'down',
  data: { duration: 12000 }
});

// Остановить блок 5
await hardwareService.sendCommand({
  action: 'actuator',
  target: 'block_5',
  state: 'stop'
});

// Остановить ВСЕ блоки
await hardwareService.sendCommand({
  action: 'actuator',
  target: 'block_0',
  state: 'stop'
});
```

### Через curl (для тестирования)

```bash
# Блок 1 вверх
curl -X POST http://192.168.1.100/command \
  -H "Content-Type: application/json" \
  -d '{"action":"actuator","target":"block_1","state":"up","data":{"duration":12000}}'

# Блок 2 вниз
curl -X POST http://192.168.1.100/command \
  -H "Content-Type: application/json" \
  -d '{"action":"actuator","target":"block_2","state":"down","data":{"duration":12000}}'

# Остановить все
curl -X POST http://192.168.1.100/command \
  -H "Content-Type: application/json" \
  -d '{"action":"actuator","target":"block_0","state":"stop"}'
```

### Напрямую на Mega (через Serial Monitor)

```json
{"block":1,"action":"up","duration":12000}
{"block":2,"action":"down","duration":10000}
{"block":0,"action":"stop"}
```

---

## 🔧 Если не работает

### Mega не получает команды
- ✅ Проверь провода (TX→RX, RX→TX, GND→GND)
- ✅ Проверь в Serial Monitor: `[INIT] Serial1 ready for ESP32 commands`
- ✅ Baud rate = 115200 на обоих

### Актуаторы не двигаются
- ✅ Проверь питание (12V для актуаторов)
- ✅ Проверь пины (Блок 1 = 22,23,24,25)
- ✅ Инверсная логика: LOW=ON, HIGH=OFF

### ESP32 не подключается к WiFi
- ✅ SSID: `RAMS_Hub`
- ✅ Пароль: `RamsInteractive2026`
- ✅ IP: `192.168.1.100`

---

## 📋 Блоки и пины

| Блок | Пины         |
|------|--------------|
| 1    | 22,23,24,25  |
| 2    | 26,27,28,29  |
| 3    | 30,31,32,33  |
| 4    | 34,35,36,37  |
| 5    | 38,39,40,41  |
| 6    | 50,51,52,53  |
| 7    | 42,43,44,45  |
| 8    | 46,47,48,49  |

---

## ✅ Чек-лист запуска

- [ ] Загружена прошивка на Arduino Mega
- [ ] Загружена прошивка на ESP32
- [ ] Подключены провода (TX-RX, RX-TX, GND-GND)
- [ ] ESP32 подключился к WiFi (192.168.1.100)
- [ ] Протестирована команда через curl
- [ ] Актуаторы движутся и останавливаются
- [ ] Приложение отправляет команды успешно

---

**Готово! 🎉**

Теперь твоё приложение управляет актуаторами через ESP32 и Mega!
