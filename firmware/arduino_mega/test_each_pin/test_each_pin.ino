/**
 * Arduino Mega - Test Each Pin of Block 1
 * Тестирует каждый пин по отдельности
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  BLOCK 1 - EACH PIN TEST");
  Serial.println("========================================\n");

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);

  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);

  Serial.println("[INIT] All pins LOW");
  delay(2000);
}

void loop() {
  // Test Pin 22
  Serial.println("\n=== PIN 22 = HIGH ===");
  digitalWrite(22, HIGH);
  Serial.println("⏳ 5 seconds - CHECK WHICH ACTUATOR MOVES!");
  delay(5000);
  digitalWrite(22, LOW);
  delay(2000);

  // Test Pin 23
  Serial.println("\n=== PIN 23 = HIGH ===");
  digitalWrite(23, HIGH);
  Serial.println("⏳ 5 seconds - CHECK WHICH ACTUATOR MOVES!");
  delay(5000);
  digitalWrite(23, LOW);
  delay(2000);

  // Test Pin 24
  Serial.println("\n=== PIN 24 = HIGH ===");
  digitalWrite(24, HIGH);
  Serial.println("⏳ 5 seconds - CHECK WHICH ACTUATOR MOVES!");
  delay(5000);
  digitalWrite(24, LOW);
  delay(2000);

  // Test Pin 25
  Serial.println("\n=== PIN 25 = HIGH ===");
  digitalWrite(25, HIGH);
  Serial.println("⏳ 5 seconds - CHECK WHICH ACTUATOR MOVES!");
  delay(5000);
  digitalWrite(25, LOW);
  delay(2000);

  Serial.println("\n========================================");
  Serial.println("All 4 pins tested - repeating...");
  Serial.println("========================================\n");
  delay(3000);
}
