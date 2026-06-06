/**
 * Arduino Mega - Single Pin Test
 * Тестирует только PIN 22 - включить/выключить
 */

#define TEST_PIN 22

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  SINGLE PIN TEST - Pin 22");
  Serial.println("========================================\n");

  pinMode(TEST_PIN, OUTPUT);
  digitalWrite(TEST_PIN, LOW);

  Serial.println("[INIT] Pin 22 = LOW");
  delay(2000);
}

void loop() {
  // TEST 1: HIGH
  Serial.println("\n=== Pin 22 = HIGH ===");
  digitalWrite(TEST_PIN, HIGH);
  Serial.println("⏳ 5 seconds - CHECK RELAY & MOTOR!");
  delay(5000);

  // OFF
  Serial.println("Pin 22 = LOW (OFF)");
  digitalWrite(TEST_PIN, LOW);
  delay(2000);

  // TEST 2: LOW (inverted)
  Serial.println("\n=== Pin 22 = LOW ===");
  digitalWrite(TEST_PIN, LOW);
  Serial.println("⏳ 5 seconds - CHECK RELAY & MOTOR!");
  delay(5000);

  // OFF
  Serial.println("Pin 22 = HIGH (OFF)");
  digitalWrite(TEST_PIN, HIGH);
  delay(2000);
}
