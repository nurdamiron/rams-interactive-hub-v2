/**
 * RAMS Screen - MEGA #2 ACTUATOR CONTROLLER
 *
 * УПРАВЛЕНИЕ БЛОКАМИ 9-15 (15 АКТУАТОРОВ)
 *
 * 7 блоков: 6 блоков по 2 актуатора + 1 блок (15-й) с 3 актуаторами
 * Всего 15 актуаторов, 30 пинов (22-53, исключая 48-49)
 * ИНВЕРСНАЯ ЛОГИКА: LOW = включено, HIGH = выключено
 *
 * БЛОК 9:  пины 22, 23, 24, 25 (2 актуатора)
 * БЛОК 10: пины 26, 27, 28, 29 (2 актуатора)
 * БЛОК 11: пины 30, 31, 32, 33 (2 актуатора)
 * БЛОК 12: пины 34, 35, 36, 37 (2 актуатора)
 * БЛОК 13: пины 38, 39, 40, 41 (2 актуатора)
 * БЛОК 14: пины 50, 51, 52, 53 (2 актуатора - ФИЗИЧЕСКИ ЗАПАЯН)
 * БЛОК 15: пины 42, 43, 44, 45, 46, 47 (3 актуатора - 6 пинов)
 *
 * СВЯЗЬ С ESP32:
 * - Serial1 (TX1=18, RX1=19) → ESP32
 * - Формат команды: JSON строка с \n
 *   {"block":9,"action":"up","duration":12000}
 *   {"block":15,"action":"down","duration":12000}
 *   {"block":0,"action":"stop"}  // 0 = все блоки
 */

#include <ArduinoJson.h>

#define MOVE_DURATION 12000  // Время движения по умолчанию (12 секунд)

// Таймауты для автоматической остановки (блоки 9-15 = индексы 0-6)
unsigned long blockTimers[7] = {0, 0, 0, 0, 0, 0, 0};
int blockDurations[7] = {0, 0, 0, 0, 0, 0, 0};

// Маппинг блоков на пины
struct Block {
  int blockNum;  // Номер блока (9-15)
  int pin1, pin2, pin3, pin4, pin5, pin6;
  bool hasThirdActuator; // true только для блока 15
};

Block blocks[7] = {
  {9,  22, 23, 24, 25, -1, -1, false},  // Блок 9
  {10, 26, 27, 28, 29, -1, -1, false},  // Блок 10
  {11, 30, 31, 32, 33, -1, -1, false},  // Блок 11
  {12, 34, 35, 36, 37, -1, -1, false},  // Блок 12
  {13, 38, 39, 40, 41, -1, -1, false},  // Блок 13
  {14, 50, 51, 52, 53, -1, -1, false},  // Блок 14 (ФИЗИЧЕСКИ ЗАПАЯН)
  {15, 42, 43, 44, 45, 46, 47, true}    // Блок 15 (3 актуатора)
};

void setup() {
  // Serial для дебага (USB)
  Serial.begin(115200);

  // Serial1 для связи с ESP32 (TX1=18, RX1=19)
  Serial1.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  RAMS MEGA #2 ACTUATOR CONTROLLER");
  Serial.println("  Blocks 9-15 (15 Actuators)");
  Serial.println("  Arduino Mega - Pins 22-47,50-53");
  Serial.println("  INVERSE LOGIC (LOW=ON, HIGH=OFF)");
  Serial.println("  Serial1 (TX1=18, RX1=19) ↔ ESP32");
  Serial.println("========================================\n");

  // Настройка пинов 22-47
  for (int pin = 22; pin <= 47; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH); // Все выключены (HIGH = OFF)
  }

  // Настройка пинов 50-53 (блок 14)
  for (int pin = 50; pin <= 53; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH); // Все выключены (HIGH = OFF)
  }

  Serial.println("[INIT] 30 pins (22-47,50-53) initialized (all OFF)");
  Serial.println("[INIT] 7 blocks ready (Block 15 has 3 actuators)");
  Serial.println("[INIT] Serial1 ready for ESP32 commands");
  Serial.println("\n[READY] Waiting for commands from ESP32...");
  Serial.println("========================================\n");

  // Отправляем подтверждение ESP32
  Serial1.println("{\"status\":\"ready\",\"mega\":2,\"blocks\":7}");
}

void loop() {
  // Проверяем наличие команды от ESP32 через Serial1
  if (Serial1.available()) {
    String command = Serial1.readStringUntil('\n');
    command.trim();

    if (command.length() > 0) {
      handleCommand(command);
    }
  }

  // Проверяем таймауты для автоматической остановки блоков
  checkBlockTimers();

  delay(10); // Небольшая задержка для стабильности
}

// ============================================================================
// ОБРАБОТКА КОМАНДЫ ОТ ESP32
// ============================================================================
void handleCommand(String jsonString) {
  Serial.print("[CMD] Received: ");
  Serial.println(jsonString);

  // Парсинг JSON
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print("[ERROR] JSON parse failed: ");
    Serial.println(error.c_str());

    // Отправляем ошибку ESP32
    Serial1.print("{\"status\":\"error\",\"message\":\"");
    Serial1.print(error.c_str());
    Serial1.println("\"}");
    return;
  }

  // Извлекаем параметры
  int blockNum = doc["block"] | 0;
  String action = doc["action"] | "";
  int duration = doc["duration"] | MOVE_DURATION;

  // Выполняем команду
  bool success = executeBlockCommand(blockNum, action, duration);

  // Отправляем подтверждение ESP32
  if (success) {
    Serial1.print("{\"status\":\"ok\",\"block\":");
    Serial1.print(blockNum);
    Serial1.print(",\"action\":\"");
    Serial1.print(action);
    Serial1.println("\"}");
  } else {
    Serial1.println("{\"status\":\"error\",\"message\":\"Invalid command\"}");
  }
}

// ============================================================================
// ВЫПОЛНЕНИЕ КОМАНДЫ
// ============================================================================
bool executeBlockCommand(int blockNum, String action, int duration) {

  // STOP ALL - остановить все блоки
  if (blockNum == 0 && action == "stop") {
    Serial.println("[CMD] STOP ALL BLOCKS");
    stopAllBlocks();
    return true;
  }

  // Проверка диапазона блока (9-15)
  if (blockNum < 9 || blockNum > 15) {
    Serial.print("[ERROR] Invalid block number for Mega #2: ");
    Serial.println(blockNum);
    return false;
  }

  int index = blockNum - 9; // Индекс массива (0-6 для блоков 9-15)
  Block b = blocks[index];

  // UP - движение вверх
  if (action == "up") {
    Serial.print("[BLOCK ");
    Serial.print(blockNum);
    Serial.print("] → UP (");
    Serial.print(duration);
    Serial.print("ms, ");
    Serial.print(b.hasThirdActuator ? 3 : 2);
    Serial.println(" actuators)");

    blockUpByPins(&b);

    // Установить таймер автоматической остановки
    blockTimers[index] = millis();
    blockDurations[index] = duration;

    return true;
  }

  // DOWN - движение вниз
  else if (action == "down") {
    Serial.print("[BLOCK ");
    Serial.print(blockNum);
    Serial.print("] → DOWN (");
    Serial.print(duration);
    Serial.print("ms, ");
    Serial.print(b.hasThirdActuator ? 3 : 2);
    Serial.println(" actuators)");

    blockDownByPins(&b);

    // Установить таймер автоматической остановки
    blockTimers[index] = millis();
    blockDurations[index] = duration;

    return true;
  }

  // STOP - остановка блока
  else if (action == "stop") {
    Serial.print("[BLOCK ");
    Serial.print(blockNum);
    Serial.println("] → STOP");

    blockStopByPins(&b);

    // Сбросить таймер
    blockTimers[index] = 0;
    blockDurations[index] = 0;

    return true;
  }

  else {
    Serial.print("[ERROR] Unknown action: ");
    Serial.println(action);
    return false;
  }
}

// ============================================================================
// ПРОВЕРКА ТАЙМЕРОВ (автоостановка после duration)
// ============================================================================
void checkBlockTimers() {
  unsigned long now = millis();

  for (int i = 0; i < 7; i++) {
    // Если таймер активен и время вышло
    if (blockTimers[i] > 0 && (now - blockTimers[i] >= blockDurations[i])) {
      Serial.print("[AUTO-STOP] Block ");
      Serial.println(i + 9);

      Block* b = &blocks[i];
      blockStopByPins(b);

      // Сбросить таймер
      blockTimers[i] = 0;
      blockDurations[i] = 0;

      // Уведомить ESP32
      Serial1.print("{\"status\":\"stopped\",\"block\":");
      Serial1.print(i + 9);
      Serial1.println(",\"reason\":\"timeout\"}");
    }
  }
}

// ============================================================================
// УПРАВЛЕНИЕ АКТУАТОРАМИ (низкоуровневые функции)
// ============================================================================

// Движение блока ВВЕРХ (по пинам)
void blockUpByPins(Block* b) {
  digitalWrite(b->pin1, LOW);   // Актуатор 1 вверх
  digitalWrite(b->pin2, HIGH);
  digitalWrite(b->pin3, LOW);   // Актуатор 2 вверх
  digitalWrite(b->pin4, HIGH);

  // Если есть 3-й актуатор (только блок 15)
  if (b->hasThirdActuator && b->pin5 != -1) {
    digitalWrite(b->pin5, LOW);   // Актуатор 3 вверх
    digitalWrite(b->pin6, HIGH);
  }
}

// Движение блока ВНИЗ (по пинам)
void blockDownByPins(Block* b) {
  digitalWrite(b->pin1, HIGH);
  digitalWrite(b->pin2, LOW);   // Актуатор 1 вниз
  digitalWrite(b->pin3, HIGH);
  digitalWrite(b->pin4, LOW);   // Актуатор 2 вниз

  // Если есть 3-й актуатор (только блок 15)
  if (b->hasThirdActuator && b->pin5 != -1) {
    digitalWrite(b->pin5, HIGH);
    digitalWrite(b->pin6, LOW);   // Актуатор 3 вниз
  }
}

// Остановка блока по пинам
void blockStopByPins(Block* b) {
  digitalWrite(b->pin1, HIGH);
  digitalWrite(b->pin2, HIGH);
  digitalWrite(b->pin3, HIGH);
  digitalWrite(b->pin4, HIGH);

  // Если есть 3-й актуатор (только блок 15)
  if (b->hasThirdActuator && b->pin5 != -1) {
    digitalWrite(b->pin5, HIGH);
    digitalWrite(b->pin6, HIGH);
  }
}

// Остановка всех блоков
void stopAllBlocks() {
  for (int i = 0; i < 7; i++) {
    Block* b = &blocks[i];
    blockStopByPins(b);
    blockTimers[i] = 0;
    blockDurations[i] = 0;
  }
  Serial.println("[SYSTEM] All blocks stopped");
}
