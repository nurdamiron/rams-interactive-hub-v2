/**
 * Arduino Mega #2 - Find Blocks 14 & 15 Pins
 * Тестирует пины 42-A15 для поиска блоков 14 и 15
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  FIND BLOCKS 14 & 15 PINS");
  Serial.println("========================================\n");

  // Проверяем пины 42-69 (включая аналоговые A0-A15 = 54-69)
  for (int pin = 42; pin <= 69; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  Serial.println("[INIT] Pins 42-69 ready\n");
  delay(3000);
}

void loop() {
  // Тестируем каждый пин
  for (int pin = 42; pin <= 69; pin++) {
    Serial.println("\n========================================");
    Serial.print(">>> PIN ");
    Serial.print(pin);
    if (pin >= 54) {
      Serial.print(" (A");
      Serial.print(pin - 54);
      Serial.print(")");
    }
    Serial.println(" = LOW (5s)");
    Serial.println("WATCH: Block 14 or 15? UP or DOWN?");
    Serial.println("========================================");

    digitalWrite(pin, LOW);
    delay(5000);
    digitalWrite(pin, HIGH);
    delay(3000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL PINS TESTED!");
  Serial.println("Did you find Blocks 14 & 15?");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
