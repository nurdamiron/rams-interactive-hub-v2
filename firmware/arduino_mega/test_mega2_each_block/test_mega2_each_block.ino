/**
 * Arduino Mega #2 - Test Each Block Separately
 * Тестирует каждый блок по очереди: UP 5сек → DOWN 5сек
 */

#define TOTAL_BLOCKS 7

struct BlockPins {
  uint8_t blockNum;
  uint8_t act1Up;
  uint8_t act1Down;
  uint8_t act2Up;
  uint8_t act2Down;
};

// Текущий маппинг (будем проверять)
BlockPins blocks[TOTAL_BLOCKS] = {
  {9,  22, 23, 24, 25},   // Block 9
  {10, 26, 27, 28, 29},   // Block 10
  {11, 30, 31, 32, 33},   // Block 11
  {12, 34, 35, 36, 37},   // Block 12
  {13, 38, 39, 40, 41},   // Block 13
  {14, 42, 43, 44, 45},   // Block 14
  {15, 46, 47, 48, 49}    // Block 15 (ВРЕМЕННО 2 актуатора)
};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  MEGA #2 - TEST EACH BLOCK");
  Serial.println("========================================\n");

  // Инициализация всех пинов
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    pinMode(blocks[i].act1Up, OUTPUT);
    pinMode(blocks[i].act1Down, OUTPUT);
    pinMode(blocks[i].act2Up, OUTPUT);
    pinMode(blocks[i].act2Down, OUTPUT);

    // Все HIGH = все OFF
    digitalWrite(blocks[i].act1Up, HIGH);
    digitalWrite(blocks[i].act1Down, HIGH);
    digitalWrite(blocks[i].act2Up, HIGH);
    digitalWrite(blocks[i].act2Down, HIGH);
  }

  Serial.println("[INIT] All blocks ready\n");
  delay(2000);
}

void blockUp(int index) {
  digitalWrite(blocks[index].act1Up, LOW);
  digitalWrite(blocks[index].act2Up, LOW);
  digitalWrite(blocks[index].act1Down, HIGH);
  digitalWrite(blocks[index].act2Down, HIGH);
}

void blockDown(int index) {
  digitalWrite(blocks[index].act1Down, LOW);
  digitalWrite(blocks[index].act2Down, LOW);
  digitalWrite(blocks[index].act1Up, HIGH);
  digitalWrite(blocks[index].act2Up, HIGH);
}

void blockStop(int index) {
  digitalWrite(blocks[index].act1Up, HIGH);
  digitalWrite(blocks[index].act1Down, HIGH);
  digitalWrite(blocks[index].act2Up, HIGH);
  digitalWrite(blocks[index].act2Down, HIGH);
}

void loop() {
  // Тестируем каждый блок по очереди
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    Serial.print("\n=== BLOCK ");
    Serial.print(blocks[i].blockNum);
    Serial.println(" ===");

    // UP
    Serial.println("UP (5s)...");
    blockUp(i);
    delay(5000);

    // STOP
    blockStop(i);
    Serial.println("STOP");
    delay(500);

    // DOWN
    Serial.println("DOWN (5s)...");
    blockDown(i);
    delay(5000);

    // STOP
    blockStop(i);
    Serial.println("STOP\n");
    delay(1000);
  }

  Serial.println("\n========================================");
  Serial.println("All blocks tested! Repeating...\n");
  delay(2000);
}
