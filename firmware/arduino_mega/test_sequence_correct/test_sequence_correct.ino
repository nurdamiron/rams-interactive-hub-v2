/**
 * Arduino Mega - Correct Sequence Test
 * Этап 1: Block 1 UP
 * Этап 2: Block 1 DOWN + Block 2 UP (одновременно)
 * Этап 3: Block 2 DOWN + Block 3 UP (одновременно)
 * ...
 * Этап 8: Block 7 DOWN + Block 8 UP (одновременно)
 * Этап 9: Block 8 DOWN
 * Каждый этап = 10 секунд
 */

#define TOTAL_BLOCKS 8

struct BlockPins {
  uint8_t blockNum;
  uint8_t act1Up;
  uint8_t act1Down;
  uint8_t act2Up;
  uint8_t act2Down;
};

// ИСПРАВЛЕННЫЙ МАППИНГ (блоки 6,7,8 переназначены)
BlockPins blocks[TOTAL_BLOCKS] = {
  {1, 22, 23, 24, 25},  // Block 1
  {2, 26, 27, 28, 29},  // Block 2
  {3, 30, 31, 32, 33},  // Block 3
  {4, 34, 35, 36, 37},  // Block 4
  {5, 38, 39, 40, 41},  // Block 5
  {6, 50, 51, 52, 53},  // Block 6 (ИСПРАВЛЕНО: было 42-45)
  {7, 42, 43, 44, 45},  // Block 7 (ИСПРАВЛЕНО: было 46-49)
  {8, 46, 47, 48, 49}   // Block 8 (ИСПРАВЛЕНО: было 50-53)
};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  CORRECT SEQUENCE TEST");
  Serial.println("  One UP, One DOWN simultaneously");
  Serial.println("========================================");
  Serial.println("Logic: LOW = ON, HIGH = OFF\n");

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
  // Порядок как в рабочем тесте: сначала DOWN = LOW, потом UP = HIGH
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
  Serial.println("STARTING SEQUENCE (90 seconds total)");
  Serial.println("========================================\n");

  // ЭТАП 1: Block 1 UP (10 сек)
  Serial.println("STAGE 1: Block 1 UP (10s)");
  blockUp(0);
  delay(10000);

  // ЭТАПЫ 2-8: Предыдущий DOWN + Следующий UP одновременно
  for (int i = 0; i < 7; i++) {
    Serial.print("STAGE ");
    Serial.print(i + 2);
    Serial.print(": Block ");
    Serial.print(i + 1);
    Serial.print(" DOWN + Block ");
    Serial.print(i + 2);
    Serial.println(" UP (10s)");

    blockDown(i);     // Предыдущий блок DOWN
    blockUp(i + 1);   // Следующий блок UP
    delay(10000);
  }

  // Остановить ВСЕ блоки перед STAGE 9
  Serial.println("STOP - All blocks off");
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    blockStop(i);
  }
  delay(500);

  // ЭТАП 9: Block 8 DOWN (10 сек)
  Serial.println("STAGE 9: Block 8 DOWN (10s)");
  blockDown(7);
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
