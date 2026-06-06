/**
 * ТЕСТОВЫЙ СКРИПТ ДЛЯ MEGA #2
 *
 * Поочередное тестирование актуаторов блоков 9-15
 * - Каждый блок поднимается вверх на 3 секунды
 * - Затем опускается вниз на 3 секунды
 * - Пауза 1 секунда между блоками
 *
 * БЛОКИ:
 * Блок 9:  пины 22, 23, 24, 25 (2 актуатора)
 * Блок 10: пины 26, 27, 28, 29 (2 актуатора)
 * Блок 11: пины 30, 31, 32, 33 (2 актуатора)
 * Блок 12: пины 34, 35, 36, 37 (2 актуатора)
 * Блок 13: пины 38, 39, 40, 41 (2 актуатора)
 * Блок 14: пины 50, 51, 52, 53 (2 актуатора - ФИЗИЧЕСКИ ЗАПАЯН)
 * Блок 15: пины 42, 43, 44, 45, 46, 47 (3 актуатора)
 *
 * ИНВЕРСНАЯ ЛОГИКА: LOW = включено, HIGH = выключено
 */

#define TEST_DURATION 3000  // 3 секунды на движение
#define PAUSE_DURATION 1000 // 1 секунда пауза между блоками

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
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n========================================");
  Serial.println("  MEGA #2 ACTUATOR TEST SCRIPT");
  Serial.println("  Testing Blocks 9-15 (15 actuators)");
  Serial.println("  INVERSE LOGIC (LOW=ON, HIGH=OFF)");
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

  Serial.println("[INIT] All pins initialized (all OFF)");
  Serial.println("[INIT] Starting test sequence in 3 seconds...\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========== STARTING TEST CYCLE ==========\n");

  // Тестирование каждого блока по очереди
  for (int i = 0; i < 7; i++) {
    testBlock(&blocks[i]);
  }

  Serial.println("\n========== TEST CYCLE COMPLETED ==========");
  Serial.println("Waiting 5 seconds before next cycle...\n");
  delay(5000);
}

// ============================================================================
// ТЕСТИРОВАНИЕ ОДНОГО БЛОКА
// ============================================================================
void testBlock(Block* b) {
  int actuatorCount = b->hasThirdActuator ? 3 : 2;

  Serial.println("----------------------------------------");
  Serial.print("Testing BLOCK ");
  Serial.print(b->blockNum);
  Serial.print(" (");
  Serial.print(actuatorCount);
  Serial.println(" actuators)");
  Serial.println("----------------------------------------");

  // Движение ВВЕРХ
  Serial.print("  [UP] Pins: ");
  printBlockPins(b);
  blockUp(b);
  delay(TEST_DURATION);

  // Остановка
  Serial.println("  [STOP]");
  blockStop(b);
  delay(PAUSE_DURATION);

  // Движение ВНИЗ
  Serial.print("  [DOWN] Pins: ");
  printBlockPins(b);
  blockDown(b);
  delay(TEST_DURATION);

  // Остановка
  Serial.println("  [STOP]");
  blockStop(b);
  Serial.println();

  delay(PAUSE_DURATION);
}

// ============================================================================
// УПРАВЛЕНИЕ АКТУАТОРАМИ
// ============================================================================

// Движение блока ВВЕРХ
void blockUp(Block* b) {
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

// Движение блока ВНИЗ
void blockDown(Block* b) {
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

// Остановка блока
void blockStop(Block* b) {
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

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================
void printBlockPins(Block* b) {
  Serial.print(b->pin1);
  Serial.print(",");
  Serial.print(b->pin2);
  Serial.print(",");
  Serial.print(b->pin3);
  Serial.print(",");
  Serial.print(b->pin4);

  if (b->hasThirdActuator && b->pin5 != -1) {
    Serial.print(",");
    Serial.print(b->pin5);
    Serial.print(",");
    Serial.print(b->pin6);
  }

  Serial.println();
}
 