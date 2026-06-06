/**
 * TEST 1: PIN 50=LOW, PIN 51=HIGH
 */
void setup() {
  Serial.begin(115200);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  Serial.println("\n========================================");
  Serial.println("TEST 1: PIN 50=LOW, PIN 51=HIGH");
  Serial.println("========================================\n");
  delay(3000);
}

void loop() {
  Serial.println(">>> ACTIVATING: PIN 50=LOW, PIN 51=HIGH");
  digitalWrite(50, LOW);
  digitalWrite(51, HIGH);

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
