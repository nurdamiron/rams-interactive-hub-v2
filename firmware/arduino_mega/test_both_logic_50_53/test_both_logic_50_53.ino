/**
 * Arduino Mega #2 - Test Both Logics for Pins 50-53
 * Проверяем LOW=ON и HIGH=ON для каждого пина
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST BOTH LOGICS - PINS 50-53");
  Serial.println("========================================\n");

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  // Начнём с LOW
  digitalWrite(50, LOW);
  digitalWrite(51, LOW);
  digitalWrite(52, LOW);
  digitalWrite(53, LOW);

  Serial.println("[INIT] All pins LOW\n");
  delay(3000);
}

void loop() {
  for (int pin = 50; pin <= 53; pin++) {
    Serial.println("\n========================================");
    Serial.print(">>> PIN ");
    Serial.print(pin);
    Serial.println(" = HIGH (10s)");
    Serial.println("WATCH: Does actuator move?");
    Serial.println("========================================\n");
    digitalWrite(pin, HIGH);
    delay(10000);
    digitalWrite(pin, LOW);
    delay(5000);
  }

  Serial.println("\n========================================");
  Serial.println("ALL PINS TESTED WITH HIGH!");
  Serial.println("Did any pin make actuator move?");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
