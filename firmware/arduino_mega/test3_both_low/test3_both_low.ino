/**
 * TEST 3: BOTH LOW (50=LOW, 51=LOW)
 */
void setup() {
  Serial.begin(115200);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  Serial.println("\n========================================");
  Serial.println("TEST 3: BOTH LOW (50=LOW, 51=LOW)");
  Serial.println("========================================\n");
  delay(3000);
}

void loop() {
  Serial.println(">>> ACTIVATING: BOTH LOW");
  digitalWrite(50, LOW);
  digitalWrite(51, LOW);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  Active: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  Serial.println("\n>>> STOP: Both HIGH");
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  Pause: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }
  Serial.println("\n");
}
