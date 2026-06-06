/**
 * Arduino Mega #1 - Simple Actuator Controller
 *
 * Управляет блоками 1-8
 * Простой текстовый протокол (как DroneControl.ino)
 *
 * Принимает команды от ESP32 через Serial1:
 * - BLOCK:5:UP:10000\n
 * - BLOCK:3:DOWN:10000\n
 * - BLOCK:7:STOP\n
 * - ALL:STOP\n
 * - PING\n
 *
 * Отправляет ответы:
 * - ACK:5:UP\n
 * - PONG\n
 * - DONE:5\n
 *
 * @version 1.0
 * @author RAMS Global Team
 */

// ============================================================================
// КОНФИГУРАЦИЯ БЛОКОВ
// ============================================================================
#define TOTAL_BLOCKS 8  // Блоки 1-8 для Mega #1
#define FIRST_BLOCK 1   // Первый блок = 1

// Пины актуаторов (по 2 пина на блок: UP и DOWN)
// Блок 1: pins 22, 23
// Блок 2: pins 24, 25
// ...
struct ActuatorPins {
  uint8_t upPin;
  uint8_t downPin;
};

// Маппинг блоков → пины
ActuatorPins blockPins[TOTAL_BLOCKS + 1] = {
  {0, 0},      // Блок 0 не используется
  {22, 23},    // Блок 1
  {24, 25},    // Блок 2
  {26, 27},    // Блок 3
  {28, 29},    // Блок 4
  {30, 31},    // Блок 5
  {32, 33},    // Блок 6
  {34, 35},    // Блок 7
  {36, 37}     // Блок 8
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

Block blocks[TOTAL_BLOCKS + 1];

// ============================================================================
// НАСТРОЙКИ SERIAL
// ============================================================================
#define SERIAL_BAUD 115200
#define ESP32_SERIAL Serial1  // RX1 (pin 19), TX1 (pin 18)

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  // Debug serial
  Serial.begin(SERIAL_BAUD);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  MEGA #1 ACTUATOR CONTROLLER");
  Serial.println("  Blocks 1-8 | Simple Protocol");
  Serial.println("========================================");

  // Инициализация пинов актуаторов
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    pinMode(blockPins[i].upPin, OUTPUT);
    pinMode(blockPins[i].downPin, OUTPUT);
    digitalWrite(blockPins[i].upPin, LOW);
    digitalWrite(blockPins[i].downPin, LOW);

    blocks[i].state = STATE_IDLE;
    blocks[i].startTime = 0;
    blocks[i].duration = 0;

    Serial.print("[INIT] Block ");
    Serial.print(i);
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
  // Формат: BLOCK:5:UP:10000
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
  if (block < FIRST_BLOCK || block > TOTAL_BLOCKS) {
    Serial.print("[ERROR] Invalid block: ");
    Serial.println(block);
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
  // Остановить текущее движение
  digitalWrite(blockPins[block].upPin, LOW);
  digitalWrite(blockPins[block].downPin, LOW);

  // Начать движение вверх
  digitalWrite(blockPins[block].upPin, HIGH);

  blocks[block].state = STATE_UP;
  blocks[block].startTime = millis();
  blocks[block].duration = duration;

  Serial.print("[BLOCK] ");
  Serial.print(block);
  Serial.print(" UP (");
  Serial.print(duration);
  Serial.println("ms)");
}

void blockDown(int block, int duration) {
  // Остановить текущее движение
  digitalWrite(blockPins[block].upPin, LOW);
  digitalWrite(blockPins[block].downPin, LOW);

  // Начать движение вниз
  digitalWrite(blockPins[block].downPin, HIGH);

  blocks[block].state = STATE_DOWN;
  blocks[block].startTime = millis();
  blocks[block].duration = duration;

  Serial.print("[BLOCK] ");
  Serial.print(block);
  Serial.print(" DOWN (");
  Serial.print(duration);
  Serial.println("ms)");
}

void blockStop(int block) {
  digitalWrite(blockPins[block].upPin, LOW);
  digitalWrite(blockPins[block].downPin, LOW);

  blocks[block].state = STATE_IDLE;

  Serial.print("[BLOCK] ");
  Serial.print(block);
  Serial.println(" STOP");
}

// ============================================================================
// ОБРАБОТКА ALL:STOP
// ============================================================================
void handleStopAll() {
  Serial.println("[ALL] STOP ALL BLOCKS");

  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
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

  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (blocks[i].state == STATE_IDLE) continue;

    unsigned long elapsed = now - blocks[i].startTime;

    // Проверка таймаута
    if (elapsed >= blocks[i].duration) {
      blockStop(i);

      // Отправляем DONE
      ESP32_SERIAL.print("DONE:");
      ESP32_SERIAL.println(i);

      Serial.print("[DONE] Block ");
      Serial.print(i);
      Serial.println(" timeout");
    }
  }
}
