/**
 * Arduino Mega #2 - Test Sequence (Blocks 9-15)
 * Этап 1: Block 9 UP
 * Этап 2: Block 9 DOWN + Block 10 UP (одновременно)
 * Этап 3: Block 10 DOWN + Block 11 UP (одновременно)
 * ...
 * Этап 7: Block 14 DOWN + Block 15 UP (одновременно)
 * Этап 8: Block 15 DOWN
 * Каждый этап = 10 секунд
 */

#define TOTAL_BLOCKS 7

struct BlockPins {
  uint8_t blockNum;
  uint8_t act1Up;
  uint8_t act1Down;
  uint8_t act2Up;
  uint8_t act2Down;
};

// Маппинг блоков 9-15
BlockPins blocks[TOTAL_BLOCKS] = {
  {9,  22, 23, 24, 25},   // Block 9
  {10, 26, 27, 28, 29},   // Block 10
  {11, 30, 31, 32, 33},   // Block 11
  {12, 34, 35, 36, 37},   // Block 12
  {13, 38, 39, 40, 41},   // Block 13
  {14, 42, 43, 44, 45},   // Block 14
  {15, 46, 47, 48, 49}    // Block 15
};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  MEGA #2 - BLOCKS 9-15 TEST");
  Serial.println("  One UP, One DOWN simultaneously");
  Serial.println("========================================");
  Serial.println("Logic: LOW = ON, HIGH = OFF\n");

  // Инициализация всех пинов
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    pinMode(blocks[i].act1Up, OUTPUT);
    pinMode(blocks[i].act1Down, OUTPUT);
    pinMode(blocks[i].act2Up, OUTPUT);
    pinMode(blocks[i].act2Down, OUTPUT);

    // Все HIGH = все OFF (инверсная логика)
    digitalWrite(blocks[i].act1Up, HIGH);
    digitalWrite(blocks[i].act1Down, HIGH);
    digitalWrite(blocks[i].act2Up, HIGH);
    digitalWrite(blocks[i].act2Down, HIGH);
  }

  Serial.println("[INIT] All blocks ready\n");
  delay(3000);
}

void blockUp(int index) {
  // Порядок: сначала UP = LOW, потом DOWN = HIGH
  digitalWrite(blocks[index].act1Up, LOW);     // Act1 UP
  digitalWrite(blocks[index].act2Up, LOW);     // Act2 UP
  digitalWrite(blocks[index].act1Down, HIGH);  // Act1 DOWN off
  digitalWrite(blocks[index].act2Down, HIGH);  // Act2 DOWN off
}

void blockDown(int index) {
  // Порядок: сначала DOWN = LOW, потом UP = HIGH
  digitalWrite(blocks[index].act1Down, LOW);   // Act1 DOWN
  digitalWrite(blocks[index].act2Down, LOW);   // Act2 DOWN
  digitalWrite(blocks[index].act1Up, HIGH);    // Act1 UP off
  digitalWrite(blocks[index].act2Up, HIGH);    // Act2 UP off
}

void blockStop(int index) {
  digitalWrite(blocks[index].act1Up, HIGH);
  digitalWrite(blocks[index].act1Down, HIGH);
  digitalWrite(blocks[index].act2Up, HIGH);
  digitalWrite(blocks[index].act2Down, HIGH);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("STARTING SEQUENCE (70 seconds total)");
  Serial.println("========================================\n");

  // ЭТАП 1: Block 9 UP (10 сек)
  Serial.println("STAGE 1: Block 9 UP (10s)");
  blockUp(0);
  delay(10000);

  // ЭТАПЫ 2-7: Предыдущий DOWN + Следующий UP одновременно
  for (int i = 0; i < 6; i++) {
    Serial.print("STAGE ");
    Serial.print(i + 2);
    Serial.print(": Block ");
    Serial.print(blocks[i].blockNum);
    Serial.print(" DOWN + Block ");
    Serial.print(blocks[i + 1].blockNum);
    Serial.println(" UP (10s)");

    blockDown(i);     // Предыдущий блок DOWN
    blockUp(i + 1);   // Следующий блок UP
    delay(10000);
  }

  // Остановить ВСЕ блоки перед STAGE 8
  Serial.println("STOP - All blocks off");
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    blockStop(i);
  }
  delay(500);

  // ЭТАП 8: Block 15 DOWN (10 сек)
  Serial.println("STAGE 8: Block 15 DOWN (10s)");
  blockDown(6);
  delay(10000);

  // Остановить все
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    blockStop(i);
  }

  Serial.println("\n========================================");
  Serial.println("SEQUENCE COMPLETE! Restarting...\n");
  Serial.println("========================================\n");
  delay(2000);
}
