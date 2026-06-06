/**
 * Arduino Mega - Test DOWN Relays Separately
 * Проверяем каждое DOWN реле отдельно
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST DOWN RELAYS - PIN 47 and 49");
  Serial.println("========================================\n");

  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);

  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Pins ready\n");
  delay(3000);
}

void loop() {
  Serial.println("\n>>> PIN 47 = LOW (10s)");
  Serial.println("CHECK: Does relay for PIN 47 turn ON?");
  digitalWrite(47, LOW);
  delay(10000);
  digitalWrite(47, HIGH);
  Serial.println("PIN 47 = HIGH (OFF)\n");
  delay(3000);

  Serial.println(">>> PIN 49 = LOW (10s)");
  Serial.println("CHECK: Does relay for PIN 49 turn ON?");
  digitalWrite(49, LOW);
  delay(10000);
  digitalWrite(49, HIGH);
  Serial.println("PIN 49 = HIGH (OFF)\n");
  delay(3000);

  Serial.println("========================================");
  Serial.println("Cycle complete! Repeating...\n");
  delay(2000);
}
