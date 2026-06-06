/**
 * Arduino Mega #2 - Find Block 11 Second Actuator
 * Ищем пин для второго актуатора Block 11
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  FIND BLOCK 11 - SECOND ACTUATOR");
  Serial.println("========================================\n");

  // Проверяем пины 30-41 (вокруг Block 11)
  for (int pin = 30; pin <= 41; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  Serial.println("[INIT] Pins 30-41 ready\n");
  delay(3000);
}

void loop() {
  // Тестируем каждый пин
  for (int pin = 30; pin <= 41; pin++) {
    Serial.println("\n========================================");
    Serial.print(">>> PIN ");
    Serial.print(pin);
    Serial.println(" = LOW (5s)");
    Serial.println("WATCH: Does Block 11 second actuator move?");
    Serial.println("========================================");

    digitalWrite(pin, LOW);
    delay(5000);
    digitalWrite(pin, HIGH);
    delay(3000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL PINS TESTED!");
  Serial.println("Did you find the pin for second actuator?");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
