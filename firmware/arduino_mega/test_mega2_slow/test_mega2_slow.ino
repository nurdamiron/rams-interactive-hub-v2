/**
 * Arduino Mega #2 - Slow Test
 * Медленный тест каждого блока с паузами
 */

#define TOTAL_BLOCKS 7

struct BlockPins {
  uint8_t blockNum;
  uint8_t act1Up;
  uint8_t act1Down;
  uint8_t act2Up;
  uint8_t act2Down;
};

BlockPins blocks[TOTAL_BLOCKS] = {
  {9,  22, 23, 24, 25},
  {10, 26, 27, 28, 29},
  {11, 34, 35, 36, 37},  // ИСПРАВЛЕНО: было 30-33
  {12, 30, 31, 32, 33},  // ИСПРАВЛЕНО: было 34-37
  {13, 38, 39, 40, 41},
  {14, 42, 43, 44, 45},
  {15, 46, 47, 48, 49}
};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  MEGA #2 - SLOW TEST");
  Serial.println("  10 seconds per action");
  Serial.println("========================================\n");

  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    pinMode(blocks[i].act1Up, OUTPUT);
    pinMode(blocks[i].act1Down, OUTPUT);
    pinMode(blocks[i].act2Up, OUTPUT);
    pinMode(blocks[i].act2Down, OUTPUT);

    digitalWrite(blocks[i].act1Up, HIGH);
    digitalWrite(blocks[i].act1Down, HIGH);
    digitalWrite(blocks[i].act2Up, HIGH);
    digitalWrite(blocks[i].act2Down, HIGH);
  }

  Serial.println("[INIT] Ready\n");
  delay(3000);
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
  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    Serial.println("\n========================================");
    Serial.print(">>> TESTING BLOCK ");
    Serial.println(blocks[i].blockNum);
    Serial.println("========================================");

    Serial.println("\n[UP] 10 seconds - WATCH NOW!");
    Serial.print("Pins: ");
    Serial.print(blocks[i].act1Up);
    Serial.print(",");
    Serial.println(blocks[i].act2Up);
    blockUp(i);
    delay(10000);

    blockStop(i);
    Serial.println("\n[STOP] 3 seconds pause");
    delay(3000);

    Serial.println("\n[DOWN] 10 seconds - WATCH NOW!");
    Serial.print("Pins: ");
    Serial.print(blocks[i].act1Down);
    Serial.print(",");
    Serial.println(blocks[i].act2Down);
    blockDown(i);
    delay(10000);

    blockStop(i);
    Serial.println("\n[STOP] Done\n");
    delay(5000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL BLOCKS TESTED! Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
