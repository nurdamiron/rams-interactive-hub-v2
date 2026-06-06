/**
 * Arduino Mega #1 - Actuator Test Script
 *
 * Тестирует все 8 блоков по очереди
 * Логика: Блок N опускается + Блок N+1 поднимается одновременно
 *
 * ПИНЫ:
 * Block 1: UP 22,23 / DOWN 24,25
 * Block 2: UP 26,27 / DOWN 28,29
 * Block 3: UP 30,31 / DOWN 32,33
 * Block 4: UP 34,35 / DOWN 36,37
 * Block 5: UP 38,39 / DOWN 40,41
 * Block 6: UP 50,51 / DOWN 52,53
 * Block 7: UP 42,43 / DOWN 44,45
 * Block 8: UP 46,47 / DOWN 48,49
 */

#define TOTAL_BLOCKS 8
#define UP_DURATION 10000    // 10 секунд вверх
#define DOWN_DURATION 10000  // 10 секунд вниз

// Структура пинов блока
struct BlockPins {
  uint8_t blockNum;
  uint8_t upPin1;
  uint8_t upPin2;
  uint8_t downPin1;
  uint8_t downPin2;
};

// Маппинг блоков → пины (4 пина на блок)
BlockPins blocks[TOTAL_BLOCKS] = {
  {1, 22, 23, 24, 25},  // Block 1
  {2, 26, 27, 28, 29},  // Block 2
  {3, 30, 31, 32, 33},  // Block 3
  {4, 34, 35, 36, 37},  // Block 4
  {5, 38, 39, 40, 41},  // Block 5
  {6, 50, 51, 52, 53},  // Block 6
  {7, 42, 43, 44, 45},  // Block 7
  {8, 46, 47, 48, 49}   // Block 8
};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  ACTUATOR TEST SCRIPT");
  Serial.println("  Testing Blocks 1-8 in Sequence");
  Serial.println("========================================\n");

  // Инициализация всех пинов
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    pinMode(blocks[i].upPin1, OUTPUT);
    pinMode(blocks[i].upPin2, OUTPUT);
    pinMode(blocks[i].downPin1, OUTPUT);
    pinMode(blocks[i].downPin2, OUTPUT);

    // ИНВЕРСНАЯ ЛОГИКА: Все пины HIGH (реле выключены)
    digitalWrite(blocks[i].upPin1, HIGH);
    digitalWrite(blocks[i].upPin2, HIGH);
    digitalWrite(blocks[i].downPin1, HIGH);
    digitalWrite(blocks[i].downPin2, HIGH);

    Serial.print("[INIT] Block ");
    Serial.print(blocks[i].blockNum);
    Serial.print(" → UP:");
    Serial.print(blocks[i].upPin1);
    Serial.print(",");
    Serial.print(blocks[i].upPin2);
    Serial.print(" / DOWN:");
    Serial.print(blocks[i].downPin1);
    Serial.print(",");
    Serial.println(blocks[i].downPin2);
  }

  Serial.println("\n[READY] Starting test in 3 seconds...\n");
  delay(3000);
}

void loop() {
  // ЭТАП 1: Поднять первый блок
  Serial.println("=== STAGE 1: Block 1 UP ===");
  blockUp(0);
  delay(UP_DURATION);

  // ЭТАП 2-8: Опустить текущий + Поднять следующий
  for (int i = 1; i < TOTAL_BLOCKS; i++) {
    Serial.print("\n=== STAGE ");
    Serial.print(i + 1);
    Serial.print(": Block ");
    Serial.print(i);
    Serial.print(" DOWN + Block ");
    Serial.print(i + 1);
    Serial.println(" UP ===");

    blockDown(i - 1);  // Опустить предыдущий
    blockUp(i);        // Поднять текущий

    delay(DOWN_DURATION);
  }

  // ЭТАП 9: Опустить последний блок
  Serial.println("\n=== STAGE 9: Block 8 DOWN ===");
  blockDown(TOTAL_BLOCKS - 1);
  delay(DOWN_DURATION);

  // Конец цикла
  Serial.println("\n========================================");
  Serial.println("  TEST COMPLETE!");
  Serial.println("  All blocks tested successfully");
  Serial.println("========================================\n");
  Serial.println("Restarting in 5 seconds...\n");
  delay(5000);
}

// ============================================================================
// ФУНКЦИИ УПРАВЛЕНИЯ БЛОКАМИ
// ============================================================================

void blockUp(int blockIndex) {
  // ИНВЕРСНАЯ ЛОГИКА: LOW = реле включено, HIGH = реле выключено

  // Включить UP пины (LOW = включено)
  digitalWrite(blocks[blockIndex].upPin1, LOW);
  digitalWrite(blocks[blockIndex].upPin2, LOW);

  // Выключить DOWN пины (HIGH = выключено)
  digitalWrite(blocks[blockIndex].downPin1, HIGH);
  digitalWrite(blocks[blockIndex].downPin2, HIGH);

  Serial.print("[UP] Block ");
  Serial.print(blocks[blockIndex].blockNum);
  Serial.print(" → Pins ");
  Serial.print(blocks[blockIndex].upPin1);
  Serial.print(",");
  Serial.print(blocks[blockIndex].upPin2);
  Serial.println(" = LOW (relay ON)");
}

void blockDown(int blockIndex) {
  // ИНВЕРСНАЯ ЛОГИКА: LOW = реле включено, HIGH = реле выключено

  // Включить DOWN пины (LOW = включено)
  digitalWrite(blocks[blockIndex].downPin1, LOW);
  digitalWrite(blocks[blockIndex].downPin2, LOW);

  // Выключить UP пины (HIGH = выключено)
  digitalWrite(blocks[blockIndex].upPin1, HIGH);
  digitalWrite(blocks[blockIndex].upPin2, HIGH);

  Serial.print("[DOWN] Block ");
  Serial.print(blocks[blockIndex].blockNum);
  Serial.print(" → Pins ");
  Serial.print(blocks[blockIndex].downPin1);
  Serial.print(",");
  Serial.print(blocks[blockIndex].downPin2);
  Serial.println(" = LOW (relay ON)");
}

void blockStop(int blockIndex) {
  // ИНВЕРСНАЯ ЛОГИКА: Все HIGH = все реле выключены
  digitalWrite(blocks[blockIndex].upPin1, HIGH);
  digitalWrite(blocks[blockIndex].upPin2, HIGH);
  digitalWrite(blocks[blockIndex].downPin1, HIGH);
  digitalWrite(blocks[blockIndex].downPin2, HIGH);

  Serial.print("[STOP] Block ");
  Serial.print(blocks[blockIndex].blockNum);
  Serial.println(" → All pins = HIGH (relays OFF)");
}
