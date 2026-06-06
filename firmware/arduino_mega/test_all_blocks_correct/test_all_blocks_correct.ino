/**
 * Arduino Mega - Test All 8 Blocks (CORRECT MAPPING)
 * Маппинг: Act1(UP=even, DOWN=even+1) + Act2(UP=even+2, DOWN=even+3)
 */

#define TOTAL_BLOCKS 8

struct BlockPins {
  uint8_t blockNum;
  uint8_t act1Up;    // Актуатор 1 UP
  uint8_t act1Down;  // Актуатор 1 DOWN
  uint8_t act2Up;    // Актуатор 2 UP
  uint8_t act2Down;  // Актуатор 2 DOWN
};

BlockPins blocks[TOTAL_BLOCKS] = {
  {1, 22, 23, 24, 25},  // Block 1: Act1(22,23) Act2(24,25)
  {2, 26, 27, 28, 29},  // Block 2: Act1(26,27) Act2(28,29)
  {3, 30, 31, 32, 33},  // Block 3: Act1(30,31) Act2(32,33)
  {4, 34, 35, 36, 37},  // Block 4: Act1(34,35) Act2(36,37)
  {5, 38, 39, 40, 41},  // Block 5: Act1(38,39) Act2(40,41)
  {6, 42, 43, 44, 45},  // Block 6: Act1(42,43) Act2(44,45)
  {7, 46, 47, 48, 49},  // Block 7: Act1(46,47) Act2(48,49)
  {8, 50, 51, 52, 53}   // Block 8: Act1(50,51) Act2(52,53)
};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TESTING ALL 8 BLOCKS - CORRECT MAPPING");
  Serial.println("========================================");
  Serial.println("Logic: LOW = Relay ON, HIGH = Relay OFF\n");

  // Инициализация пинов
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    pinMode(blocks[i].act1Up, OUTPUT);
    pinMode(blocks[i].act1Down, OUTPUT);
    pinMode(blocks[i].act2Up, OUTPUT);
    pinMode(blocks[i].act2Down, OUTPUT);

    // Все HIGH = все реле OFF (инверсная логика)
    digitalWrite(blocks[i].act1Up, HIGH);
    digitalWrite(blocks[i].act1Down, HIGH);
    digitalWrite(blocks[i].act2Up, HIGH);
    digitalWrite(blocks[i].act2Down, HIGH);
  }

  Serial.println("[INIT] All blocks ready\n");
  delay(2000);
}

void loop() {
  // Тестируем каждый блок по очереди
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    Serial.print("\n=== BLOCK ");
    Serial.print(blocks[i].blockNum);
    Serial.println(" ===");

    // UP - оба актуатора вверх (LOW = включено)
    Serial.println("UP (5s)...");
    digitalWrite(blocks[i].act1Up, LOW);     // Act1 UP
    digitalWrite(blocks[i].act2Up, LOW);     // Act2 UP
    digitalWrite(blocks[i].act1Down, HIGH);  // Act1 DOWN off
    digitalWrite(blocks[i].act2Down, HIGH);  // Act2 DOWN off
    delay(5000);

    // STOP - все HIGH = все выключены
    digitalWrite(blocks[i].act1Up, HIGH);
    digitalWrite(blocks[i].act2Up, HIGH);
    Serial.println("STOP");
    delay(500);

    // DOWN - оба актуатора вниз (LOW = включено)
    Serial.println("DOWN (5s)...");
    digitalWrite(blocks[i].act1Up, HIGH);    // Act1 UP off
    digitalWrite(blocks[i].act2Up, HIGH);    // Act2 UP off
    digitalWrite(blocks[i].act1Down, LOW);   // Act1 DOWN
    digitalWrite(blocks[i].act2Down, LOW);   // Act2 DOWN
    delay(5000);

    // STOP - все HIGH = все выключены
    digitalWrite(blocks[i].act1Down, HIGH);
    digitalWrite(blocks[i].act2Down, HIGH);
    Serial.println("STOP\n");
    delay(1000);
  }

  Serial.println("\n========================================");
  Serial.println("All blocks tested! Repeating...\n");
  delay(2000);
}
