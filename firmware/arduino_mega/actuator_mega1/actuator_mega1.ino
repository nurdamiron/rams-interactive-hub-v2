/**
 * RAMS Kinetic Table — Arduino Mega #1 Firmware
 * Blocks 1–8 (Outer Ring 1–7 + Inner Block 8)
 * Serial1 (RX=19, TX=18) ← ESP32
 * Pins 22–53: 16 actuators (2 per block), each H-Bridge uses 2 pins
 * Total: 8 blocks × 2 actuators × 2 pins = 32 pins
 *
 * PROTOCOL: JSON commands from ESP32
 * Format: {"block":5,"action":"up","duration":12000}
 */

#include <ArduinoJson.h>

#define MOVE_DURATION 12000  // Время движения по умолчанию (12 секунд)
#define MAX_ACTIVE_BLOCKS 2  // Максимум 2 блока одновременно

// Таймауты для автоматической остановки
unsigned long blockTimers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int blockDurations[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Маппинг блоков на пины
struct Block {
  int pin1, pin2, pin3, pin4;
};

Block blocks[8] = {
  {22, 23, 24, 25},  // Блок 1
  {26, 27, 28, 29},  // Блок 2
  {30, 31, 32, 33},  // Блок 3
  {34, 35, 36, 37},  // Блок 4
  {38, 39, 40, 41},  // Блок 5
  {50, 51, 52, 53},  // Блок 6 (ФИЗИЧЕСКИЕ ПИНЫ)
  {42, 43, 44, 45},  // Блок 7
  {46, 47, 48, 49}   // Блок 8
};

// Подсчет активных блоков
int countActiveBlocks() {
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (blockTimers[i] > 0) {
      count++;
    }
  }
  return count;
}

void setup() {
  // Serial для дебага (USB)
  Serial.begin(115200);

  // Serial1 для связи с ESP32 (TX1=18, RX1=19)
  Serial1.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  RAMS MEGA #1 ACTUATOR CONTROLLER");
  Serial.println("  Blocks 1-8 (16 Actuators)");
  Serial.println("  Arduino Mega - Pins 22-53");
  Serial.println("  INVERSE LOGIC (LOW=ON, HIGH=OFF)");
  Serial.println("  Serial1 (TX1=18, RX1=19) ↔ ESP32");
  Serial.println("  JSON PROTOCOL");
  Serial.println("  MAX 2 BLOCKS ACTIVE");
  Serial.println("========================================\n");

  // Настройка всех пинов от 22 до 53
  for (int pin = 22; pin <= 53; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH); // Все выключены (HIGH = OFF)
  }

  Serial.println("[INIT] 32 pins (22-53) initialized (all OFF)");
  Serial.println("[INIT] 8 blocks ready");
  Serial.println("[INIT] Serial1 ready for ESP32 JSON commands");
  Serial.println("\n[READY] Waiting for commands from ESP32...");
  Serial.println("========================================\n");

  // Отправляем подтверждение ESP32
  Serial1.println("{\"status\":\"ready\",\"mega\":1,\"blocks\":8}");
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

  // Проверка диапазона блока (1-8)
  if (blockNum < 1 || blockNum > 8) {
    Serial.print("[ERROR] Invalid block number: ");
    Serial.println(blockNum);
    return false;
  }

  int index = blockNum - 1; // Индекс массива (0-7)
  Block b = blocks[index];

  // Проверка ограничения на количество активных блоков
  if (action == "up" || action == "down") {
    int activeCount = countActiveBlocks();
    bool isThisBlockActive = (blockTimers[index] > 0);

    if (!isThisBlockActive && activeCount >= MAX_ACTIVE_BLOCKS) {
      Serial.print("[ERROR] Max active blocks limit (");
      Serial.print(MAX_ACTIVE_BLOCKS);
      Serial.print("), active: ");
      Serial.println(activeCount);

      Serial1.print("{\"status\":\"error\",\"message\":\"Max ");
      Serial1.print(MAX_ACTIVE_BLOCKS);
      Serial1.println(" blocks limit\"}");
      return false;
    }
  }

  // UP - движение вверх
  if (action == "up") {
    Serial.print("[BLOCK ");
    Serial.print(blockNum);
    Serial.print("] → UP (");
    Serial.print(duration);
    Serial.println("ms, 2 actuators)");

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
    Serial.println("ms, 2 actuators)");

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

  for (int i = 0; i < 8; i++) {
    // Если таймер активен и время вышло
    if (blockTimers[i] > 0 && (now - blockTimers[i] >= blockDurations[i])) {
      Serial.print("[AUTO-STOP] Block ");
      Serial.println(i + 1);

      Block* b = &blocks[i];
      blockStopByPins(b);

      // Сбросить таймер
      blockTimers[i] = 0;
      blockDurations[i] = 0;

      // Уведомить ESP32
      Serial1.print("{\"status\":\"stopped\",\"block\":");
      Serial1.print(i + 1);
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
}

// Движение блока ВНИЗ (по пинам)
void blockDownByPins(Block* b) {
  digitalWrite(b->pin1, HIGH);
  digitalWrite(b->pin2, LOW);   // Актуатор 1 вниз
  digitalWrite(b->pin3, HIGH);
  digitalWrite(b->pin4, LOW);   // Актуатор 2 вниз
}

// Остановка блока по пинам
void blockStopByPins(Block* b) {
  digitalWrite(b->pin1, HIGH);
  digitalWrite(b->pin2, HIGH);
  digitalWrite(b->pin3, HIGH);
  digitalWrite(b->pin4, HIGH);
}

// Остановка всех блоков
void stopAllBlocks() {
  for (int i = 0; i < 8; i++) {
    Block* b = &blocks[i];
    blockStopByPins(b);
    blockTimers[i] = 0;
    blockDurations[i] = 0;
  }
  Serial.println("[SYSTEM] All blocks stopped");
}
