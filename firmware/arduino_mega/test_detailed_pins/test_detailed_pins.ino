/**
 * Arduino Mega #2 - Detailed Pin Test
 * Детальный тест каждого пина с большими паузами
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  DETAILED PIN TEST FOR BLOCKS 14 & 15");
  Serial.println("========================================\n");

  // Проверяем пины 42-53
  for (int pin = 42; pin <= 53; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  Serial.println("[INIT] Pins 42-53 ready (all LOW)\n");
  delay(3000);
}

void loop() {
  // Тестируем каждый пин подробно
  for (int pin = 42; pin <= 53; pin++) {
    Serial.println("\n========================================");
    Serial.println("========================================");
    Serial.print(">>> TESTING PIN ");
    Serial.println(pin);
    Serial.println("========================================");
    Serial.println("========================================");

    Serial.println("\nSwitching to HIGH (10 seconds)...");
    Serial.println("WATCH CAREFULLY:");
    Serial.println("- Which block? (14 or 15?)");
    Serial.println("- Which actuator? (1st, 2nd, or 3rd?)");
    Serial.println("- Direction? (UP or DOWN?)");
    Serial.println("");

    digitalWrite(pin, HIGH);
    delay(10000);

    digitalWrite(pin, LOW);
    Serial.println("\nPIN OFF");
    Serial.println("\n--- Waiting 5 seconds before next pin ---\n");
    delay(5000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL PINS TESTED!");
  Serial.println("========================================");
  Serial.println("\nResults to report:");
  Serial.println("PIN 48 = ? (Block, Actuator, Direction)");
  Serial.println("PIN 49 = ? (Block, Actuator, Direction)");
  Serial.println("Other pins = ?");
  Serial.println("\nRepeating in 10 seconds...\n");
  delay(10000);
}
