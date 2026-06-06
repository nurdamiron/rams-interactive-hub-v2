/**
 * TEST: PINS 46-47 BOTH LOW
 */
void setup() {
  Serial.begin(115200);
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);

  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);

  Serial.println("\n========================================");
  Serial.println("TEST: PINS 46-47 BOTH LOW");
  Serial.println("========================================\n");
  delay(3000);
}

void loop() {
  Serial.println(">>> ACTIVATING: BOTH LOW (46+47)");
  digitalWrite(46, LOW);
  digitalWrite(47, LOW);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  Active: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  Serial.println("\n>>> STOP: Both HIGH");
  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  Pause: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }
  Serial.println("\n");
}
