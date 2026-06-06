/**
 * Arduino Mega - Test DOWN pins separately
 * Тестирует пины 47 и 49 отдельно
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST DOWN PINS - 47 and 49");
  Serial.println("========================================\n");

  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);

  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Pins ready\n");
  delay(2000);
}

void loop() {
  Serial.println(">>> PIN 47 = LOW (10s)");
  Serial.println("Expected: One actuator DOWN");
  digitalWrite(47, LOW);
  delay(10000);
  digitalWrite(47, HIGH);
  Serial.println("STOP\n");
  delay(2000);

  Serial.println(">>> PIN 49 = LOW (10s)");
  Serial.println("Expected: Another actuator DOWN");
  digitalWrite(49, LOW);
  delay(10000);
  digitalWrite(49, HIGH);
  Serial.println("STOP\n");
  delay(2000);

  Serial.println(">>> BOTH 47 and 49 = LOW (10s)");
  Serial.println("Expected: Both actuators DOWN");
  digitalWrite(47, LOW);
  digitalWrite(49, LOW);
  delay(10000);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  Serial.println("STOP\n");
  delay(3000);
}
