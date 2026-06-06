/**
 * Arduino Mega - Test Block 2 Each Pin
 * Тестирует каждый пин Block 2 отдельно
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  BLOCK 2 - EACH PIN TEST");
  Serial.println("  Pins: 26, 27, 28, 29");
  Serial.println("========================================\n");

  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);

  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(29, HIGH);

  Serial.println("[INIT] All pins HIGH (OFF)\n");
  delay(2000);
}

void loop() {
  // Test Pin 26
  Serial.println("\n=== PIN 26 = LOW ===");
  digitalWrite(26, LOW);
  Serial.println("⏳ 3 seconds - CHECK!");
  delay(3000);
  digitalWrite(26, HIGH);
  Serial.println("OFF");
  delay(1000);

  // Test Pin 27
  Serial.println("\n=== PIN 27 = LOW ===");
  digitalWrite(27, LOW);
  Serial.println("⏳ 3 seconds - CHECK!");
  delay(3000);
  digitalWrite(27, HIGH);
  Serial.println("OFF");
  delay(1000);

  // Test Pin 28
  Serial.println("\n=== PIN 28 = LOW ===");
  digitalWrite(28, LOW);
  Serial.println("⏳ 3 seconds - CHECK!");
  delay(3000);
  digitalWrite(28, HIGH);
  Serial.println("OFF");
  delay(1000);

  // Test Pin 29
  Serial.println("\n=== PIN 29 = LOW ===");
  digitalWrite(29, LOW);
  Serial.println("⏳ 3 seconds - CHECK!");
  delay(3000);
  digitalWrite(29, HIGH);
  Serial.println("OFF");
  delay(1000);

  Serial.println("\n========================================");
  Serial.println("All pins tested - repeating...");
  Serial.println("========================================\n");
  delay(2000);
}
