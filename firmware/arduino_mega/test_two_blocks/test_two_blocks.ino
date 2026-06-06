/**
 * Arduino Mega - Two Blocks Test
 * Тестирует Block 1 и Block 2 одновременно
 */

// Block 1 пины
#define BLOCK1_UP1   22
#define BLOCK1_UP2   23
#define BLOCK1_DOWN1 24
#define BLOCK1_DOWN2 25

// Block 2 пины
#define BLOCK2_UP1   26
#define BLOCK2_UP2   27
#define BLOCK2_DOWN1 28
#define BLOCK2_DOWN2 29

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TWO BLOCKS TEST");
  Serial.println("  Block 1 + Block 2 simultaneously");
  Serial.println("========================================\n");

  // Инициализация Block 1
  pinMode(BLOCK1_UP1, OUTPUT);
  pinMode(BLOCK1_UP2, OUTPUT);
  pinMode(BLOCK1_DOWN1, OUTPUT);
  pinMode(BLOCK1_DOWN2, OUTPUT);

  // Инициализация Block 2
  pinMode(BLOCK2_UP1, OUTPUT);
  pinMode(BLOCK2_UP2, OUTPUT);
  pinMode(BLOCK2_DOWN1, OUTPUT);
  pinMode(BLOCK2_DOWN2, OUTPUT);

  // Начальное состояние - всё LOW
  digitalWrite(BLOCK1_UP1, LOW);
  digitalWrite(BLOCK1_UP2, LOW);
  digitalWrite(BLOCK1_DOWN1, LOW);
  digitalWrite(BLOCK1_DOWN2, LOW);

  digitalWrite(BLOCK2_UP1, LOW);
  digitalWrite(BLOCK2_UP2, LOW);
  digitalWrite(BLOCK2_DOWN1, LOW);
  digitalWrite(BLOCK2_DOWN2, LOW);

  Serial.println("[INIT] Both blocks ready");
  delay(2000);
}

void loop() {
  // ============ TEST 1: HIGH = ON ============
  Serial.println("\n=== TEST 1: Both UP with HIGH ===");
  Serial.println("Block 1: pins 22,23 = HIGH");
  Serial.println("Block 2: pins 26,27 = HIGH");

  digitalWrite(BLOCK1_UP1, HIGH);
  digitalWrite(BLOCK1_UP2, HIGH);
  digitalWrite(BLOCK1_DOWN1, LOW);
  digitalWrite(BLOCK1_DOWN2, LOW);

  digitalWrite(BLOCK2_UP1, HIGH);
  digitalWrite(BLOCK2_UP2, HIGH);
  digitalWrite(BLOCK2_DOWN1, LOW);
  digitalWrite(BLOCK2_DOWN2, LOW);

  Serial.println("⏳ Waiting 8 seconds - CHECK BOTH MOTORS!");
  delay(8000);

  Serial.println("Stopping both...\n");
  digitalWrite(BLOCK1_UP1, LOW);
  digitalWrite(BLOCK1_UP2, LOW);
  digitalWrite(BLOCK2_UP1, LOW);
  digitalWrite(BLOCK2_UP2, LOW);
  delay(3000);

  // ============ TEST 2: LOW = ON ============
  Serial.println("=== TEST 2: Both UP with LOW ===");
  Serial.println("Block 1: pins 22,23 = LOW");
  Serial.println("Block 2: pins 26,27 = LOW");

  digitalWrite(BLOCK1_UP1, LOW);
  digitalWrite(BLOCK1_UP2, LOW);
  digitalWrite(BLOCK1_DOWN1, HIGH);
  digitalWrite(BLOCK1_DOWN2, HIGH);

  digitalWrite(BLOCK2_UP1, LOW);
  digitalWrite(BLOCK2_UP2, LOW);
  digitalWrite(BLOCK2_DOWN1, HIGH);
  digitalWrite(BLOCK2_DOWN2, HIGH);

  Serial.println("⏳ Waiting 8 seconds - CHECK BOTH MOTORS!");
  delay(8000);

  Serial.println("Stopping both...\n");
  digitalWrite(BLOCK1_UP1, HIGH);
  digitalWrite(BLOCK1_UP2, HIGH);
  digitalWrite(BLOCK1_DOWN1, HIGH);
  digitalWrite(BLOCK1_DOWN2, HIGH);

  digitalWrite(BLOCK2_UP1, HIGH);
  digitalWrite(BLOCK2_UP2, HIGH);
  digitalWrite(BLOCK2_DOWN1, HIGH);
  digitalWrite(BLOCK2_DOWN2, HIGH);

  delay(3000);

  Serial.println("========================================");
  Serial.println("TESTS COMPLETE - Repeating...");
  Serial.println("========================================\n");
  delay(3000);
}
