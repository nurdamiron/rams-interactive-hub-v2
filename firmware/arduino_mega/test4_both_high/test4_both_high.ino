/**
 * TEST 4: BOTH HIGH (50=HIGH, 51=HIGH)
 */
void setup() {
  Serial.begin(115200);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, LOW);
  digitalWrite(51, LOW);

  Serial.println("\n========================================");
  Serial.println("TEST 4: BOTH HIGH (50=HIGH, 51=HIGH)");
  Serial.println("========================================\n");
  delay(3000);
}

void loop() {
  Serial.println(">>> ACTIVATING: BOTH HIGH");
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  Active: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  Serial.println("\n>>> STOP: Both LOW");
  digitalWrite(50, LOW);
  digitalWrite(51, LOW);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  Pause: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }
  Serial.println("\n");
}
