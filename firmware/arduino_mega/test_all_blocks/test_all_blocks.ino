/**
 * Arduino Mega - Test All 8 Blocks
 * Тестирует все блоки по очереди: UP 5сек → DOWN 5сек
 */

#define TOTAL_BLOCKS 8

struct BlockPins {
  uint8_t blockNum;
  uint8_t upPin1;
  uint8_t upPin2;
  uint8_t downPin1;
  uint8_t downPin2;
};

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
  Serial.println("  TESTING ALL 8 BLOCKS");
  Serial.println("  Each block: UP 5s → DOWN 5s");
  Serial.println("========================================\n");

  // Инициализация всех пинов
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    pinMode(blocks[i].upPin1, OUTPUT);
    pinMode(blocks[i].upPin2, OUTPUT);
    pinMode(blocks[i].downPin1, OUTPUT);
    pinMode(blocks[i].downPin2, OUTPUT);

    digitalWrite(blocks[i].upPin1, HIGH);
    digitalWrite(blocks[i].upPin2, HIGH);
    digitalWrite(blocks[i].downPin1, HIGH);
    digitalWrite(blocks[i].downPin2, HIGH);
  }

  Serial.println("[INIT] All blocks ready\n");
  delay(2000);
}

void loop() {
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    Serial.print("\n=== BLOCK ");
    Serial.print(blocks[i].blockNum);
    Serial.println(" ===");

    // UP - ИНВЕРСНАЯ ЛОГИКА: LOW = ON
    Serial.println("UP (5s)...");
    digitalWrite(blocks[i].upPin1, LOW);
    digitalWrite(blocks[i].upPin2, LOW);
    digitalWrite(blocks[i].downPin1, HIGH);
    digitalWrite(blocks[i].downPin2, HIGH);
    delay(5000);

    // STOP - все HIGH = OFF
    digitalWrite(blocks[i].upPin1, HIGH);
    digitalWrite(blocks[i].upPin2, HIGH);
    Serial.println("STOP");
    delay(500);

    // DOWN - ИНВЕРСНАЯ ЛОГИКА: LOW = ON
    Serial.println("DOWN (5s)...");
    digitalWrite(blocks[i].upPin1, HIGH);
    digitalWrite(blocks[i].upPin2, HIGH);
    digitalWrite(blocks[i].downPin1, LOW);
    digitalWrite(blocks[i].downPin2, LOW);
    delay(5000);

    // STOP - все HIGH = OFF
    digitalWrite(blocks[i].downPin1, HIGH);
    digitalWrite(blocks[i].downPin2, HIGH);
    Serial.println("STOP\n");
    delay(1000);
  }

  Serial.println("\n========================================");
  Serial.println("All 8 blocks tested - repeating...");
  Serial.println("========================================\n");
  delay(3000);
}
