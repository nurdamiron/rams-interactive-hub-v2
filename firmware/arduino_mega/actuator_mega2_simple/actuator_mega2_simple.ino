/**
 * Arduino Mega #2 - Simple Actuator Controller
 *
 * Управляет блоками 9-15
 * Простой текстовый протокол (как DroneControl.ino)
 *
 * Принимает команды от ESP32 через Serial1:
 * - BLOCK:9:UP:10000\n
 * - BLOCK:12:DOWN:10000\n
 * - BLOCK:15:STOP\n
 * - ALL:STOP\n
 * - PING\n
 *
 * Отправляет ответы:
 * - ACK:9:UP\n
 * - PONG\n
 * - DONE:9\n
 *
 * @version 1.0
 * @author RAMS Global Team
 */

// ============================================================================
// КОНФИГУРАЦИЯ БЛОКОВ
// ============================================================================
#define TOTAL_BLOCKS 7   // Блоки 9-15 = 7 блоков
#define FIRST_BLOCK 9    // Первый блок = 9
#define LAST_BLOCK 15    // Последний блок = 15

// Пины актуаторов (по 2 пина на блок: UP и DOWN)
// Блок 9: pins 22, 23
// Блок 10: pins 24, 25
// ...
struct ActuatorPins {
  uint8_t upPin;
  uint8_t downPin;
};

// Маппинг блоков → пины
// Индекс массива = номер блока - FIRST_BLOCK
ActuatorPins blockPins[TOTAL_BLOCKS] = {
  {22, 23},    // Блок 9 (индекс 0)
  {24, 25},    // Блок 10 (индекс 1)
  {26, 27},    // Блок 11 (индекс 2)
  {28, 29},    // Блок 12 (индекс 3)
  {30, 31},    // Блок 13 (индекс 4)
  {32, 33},    // Блок 14 (индекс 5)
  {34, 35}     // Блок 15 (индекс 6)
};

// ============================================================================
// СОСТОЯНИЕ БЛОКОВ
// ============================================================================
enum BlockState {
  STATE_IDLE = 0,
  STATE_UP = 1,
  STATE_DOWN = -1
};

struct Block {
  BlockState state;
  unsigned long startTime;
  int duration;
};

Block blocks[TOTAL_BLOCKS];

// ============================================================================
// НАСТРОЙКИ SERIAL
// ============================================================================
#define SERIAL_BAUD 115200
#define ESP32_SERIAL Serial1  // RX1 (pin 19), TX1 (pin 18)

// ============================================================================
// HELPER: Получить индекс блока в массиве
// ============================================================================
inline int getBlockIndex(int blockNum) {
  return blockNum - FIRST_BLOCK;
}

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  // Debug serial
  Serial.begin(SERIAL_BAUD);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  MEGA #2 ACTUATOR CONTROLLER");
  Serial.println("  Blocks 9-15 | Simple Protocol");
  Serial.println("========================================");

  // Инициализация пинов актуаторов
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    int blockNum = FIRST_BLOCK + i;

    pinMode(blockPins[i].upPin, OUTPUT);
    pinMode(blockPins[i].downPin, OUTPUT);
    digitalWrite(blockPins[i].upPin, LOW);
    digitalWrite(blockPins[i].downPin, LOW);

    blocks[i].state = STATE_IDLE;
    blocks[i].startTime = 0;
    blocks[i].duration = 0;

    Serial.print("[INIT] Block ");
    Serial.print(blockNum);
    Serial.print(" → UP:");
    Serial.print(blockPins[i].upPin);
    Serial.print(", DOWN:");
    Serial.println(blockPins[i].downPin);
  }

  // ESP32 serial
  ESP32_SERIAL.begin(SERIAL_BAUD);
  delay(100);

  Serial.println("[SERIAL] ESP32 communication ready");
  Serial.println("[READY] System initialized!\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop() {
  // Чтение команд от ESP32
  readCommands();

  // Обработка таймаутов блоков
  processBlocks();

  delay(1);
}

// ============================================================================
// ЧТЕНИЕ КОМАНД ОТ ESP32
// ============================================================================
void readCommands() {
  if (!ESP32_SERIAL.available()) return;

  String cmd = ESP32_SERIAL.readStringUntil('\n');
  cmd.trim();

  if (cmd.length() == 0) return;

  Serial.print("[RX] ");
  Serial.println(cmd);

  // Парсинг команды
  if (cmd.startsWith("BLOCK:")) {
    handleBlockCommand(cmd);
  }
  else if (cmd == "ALL:STOP") {
    handleStopAll();
  }
  else if (cmd == "PING") {
    handlePing();
  }
  else {
    Serial.print("[ERROR] Unknown command: ");
    Serial.println(cmd);
  }
}

// ============================================================================
// ОБРАБОТКА КОМАНДЫ БЛОКА
// ============================================================================
void handleBlockCommand(String cmd) {
  // Формат: BLOCK:9:UP:10000
  int block = 0;
  char action[10];
  int duration = 0;

  // Парсинг через sscanf
  int parsed = sscanf(cmd.c_str(), "BLOCK:%d:%[^:]:%d", &block, action, &duration);

  if (parsed < 2) {
    Serial.println("[ERROR] Invalid command format");
    return;
  }

  // Проверка номера блока
  if (block < FIRST_BLOCK || block > LAST_BLOCK) {
    Serial.print("[ERROR] Invalid block: ");
    Serial.print(block);
    Serial.print(" (expected ");
    Serial.print(FIRST_BLOCK);
    Serial.print("-");
    Serial.print(LAST_BLOCK);
    Serial.println(")");
    return;
  }

  // Выполнение действия
  if (strcmp(action, "UP") == 0) {
    if (duration == 0) duration = 10000;  // По умолчанию 10 сек
    blockUp(block, duration);
    sendAck(block, "UP");
  }
  else if (strcmp(action, "DOWN") == 0) {
    if (duration == 0) duration = 10000;
    blockDown(block, duration);
    sendAck(block, "DOWN");
  }
  else if (strcmp(action, "STOP") == 0) {
    blockStop(block);
    sendAck(block, "STOP");
  }
  else {
    Serial.print("[ERROR] Invalid action: ");
    Serial.println(action);
  }
}

// ============================================================================
// КОМАНДЫ УПРАВЛЕНИЯ БЛОКАМИ
// ============================================================================
void blockUp(int block, int duration) {
  int idx = getBlockIndex(block);

  // Остановить текущее движение
  digitalWrite(blockPins[idx].upPin, LOW);
  digitalWrite(blockPins[idx].downPin, LOW);

  // Начать движение вверх
  digitalWrite(blockPins[idx].upPin, HIGH);

  blocks[idx].state = STATE_UP;
  blocks[idx].startTime = millis();
  blocks[idx].duration = duration;

  Serial.print("[BLOCK] ");
  Serial.print(block);
  Serial.print(" UP (");
  Serial.print(duration);
  Serial.println("ms)");
}

void blockDown(int block, int duration) {
  int idx = getBlockIndex(block);

  // Остановить текущее движение
  digitalWrite(blockPins[idx].upPin, LOW);
  digitalWrite(blockPins[idx].downPin, LOW);

  // Начать движение вниз
  digitalWrite(blockPins[idx].downPin, HIGH);

  blocks[idx].state = STATE_DOWN;
  blocks[idx].startTime = millis();
  blocks[idx].duration = duration;

  Serial.print("[BLOCK] ");
  Serial.print(block);
  Serial.print(" DOWN (");
  Serial.print(duration);
  Serial.println("ms)");
}

void blockStop(int block) {
  int idx = getBlockIndex(block);

  digitalWrite(blockPins[idx].upPin, LOW);
  digitalWrite(blockPins[idx].downPin, LOW);

  blocks[idx].state = STATE_IDLE;

  Serial.print("[BLOCK] ");
  Serial.print(block);
  Serial.println(" STOP");
}

// ============================================================================
// ОБРАБОТКА ALL:STOP
// ============================================================================
void handleStopAll() {
  Serial.println("[ALL] STOP ALL BLOCKS");

  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    digitalWrite(blockPins[i].upPin, LOW);
    digitalWrite(blockPins[i].downPin, LOW);
    blocks[i].state = STATE_IDLE;
  }

  sendAck(0, "STOP");
}

// ============================================================================
// ОБРАБОТКА PING
// ============================================================================
void handlePing() {
  ESP32_SERIAL.println("PONG");
  Serial.println("[TX] PONG");
}

// ============================================================================
// ОТПРАВКА ACK
// ============================================================================
void sendAck(int block, const char* action) {
  ESP32_SERIAL.print("ACK:");
  ESP32_SERIAL.print(block);
  ESP32_SERIAL.print(":");
  ESP32_SERIAL.println(action);

  Serial.print("[TX] ACK:");
  Serial.print(block);
  Serial.print(":");
  Serial.println(action);
}

// ============================================================================
// ОБРАБОТКА ТАЙМАУТОВ БЛОКОВ
// ============================================================================
void processBlocks() {
  unsigned long now = millis();

  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    if (blocks[i].state == STATE_IDLE) continue;

    unsigned long elapsed = now - blocks[i].startTime;

    // Проверка таймаута
    if (elapsed >= blocks[i].duration) {
      int blockNum = FIRST_BLOCK + i;
      blockStop(blockNum);

      // Отправляем DONE
      ESP32_SERIAL.print("DONE:");
      ESP32_SERIAL.println(blockNum);

      Serial.print("[DONE] Block ");
      Serial.print(blockNum);
      Serial.println(" timeout");
    }
  }
}
