/**
 * Arduino Mega #2 - Find Pins for Blocks 14, 15
 * Тестирует разные пины чтобы найти Block 14 и 15
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  FIND PINS FOR BLOCKS 14 & 15");
  Serial.println("========================================\n");

  // Пробуем пины 42-53
  for (int pin = 42; pin <= 53; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  Serial.println("[INIT] Pins 42-53 ready\n");
  delay(3000);
}

void loop() {
  // Тестируем каждый пин по отдельности
  for (int pin = 42; pin <= 53; pin++) {
    Serial.println("\n========================================");
    Serial.print(">>> TESTING PIN ");
    Serial.println(pin);
    Serial.println("WATCH: Which actuator moves?");
    Serial.println("========================================");

    // Включаем пин (LOW = ON)
    digitalWrite(pin, LOW);
    delay(5000);

    // Выключаем
    digitalWrite(pin, HIGH);
    Serial.println("PIN OFF\n");
    delay(3000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL PINS TESTED! Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
