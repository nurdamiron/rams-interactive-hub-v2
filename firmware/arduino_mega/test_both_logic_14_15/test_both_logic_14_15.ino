/**
 * Arduino Mega #2 - Test Both Logics for Blocks 14 & 15
 * Проверяем обе логики: LOW=ON и HIGH=ON
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST BOTH LOGICS - BLOCKS 14 & 15");
  Serial.println("========================================\n");

  // Проверяем пины 42-53
  for (int pin = 42; pin <= 53; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  Serial.println("[INIT] Pins 42-53 ready\n");
  delay(3000);
}

void loop() {
  // Тестируем каждый пин с ОБЕИМИ логиками
  for (int pin = 42; pin <= 53; pin++) {
    Serial.println("\n========================================");
    Serial.print(">>> PIN ");
    Serial.print(pin);
    Serial.println(" = HIGH (5s)");
    Serial.println("WATCH: Any movement?");
    Serial.println("========================================");

    digitalWrite(pin, HIGH);
    delay(5000);
    digitalWrite(pin, LOW);
    delay(3000);

    Serial.println("\n========================================");
    Serial.print(">>> PIN ");
    Serial.print(pin);
    Serial.println(" = LOW (already tested)");
    Serial.println("========================================");
    delay(2000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL PINS TESTED WITH BOTH LOGICS!");
  Serial.println("Did you find any movement?");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
