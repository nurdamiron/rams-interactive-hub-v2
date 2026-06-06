/**
 * Test PIN 51 ONLY - 5 sec cycles
 */

void setup() {
  Serial.begin(115200);
  pinMode(51, OUTPUT);
  digitalWrite(51, LOW);
  Serial.println("\n=== PIN 51 TEST (5 SEC) ===\n");
}

void loop() {
  Serial.println("PIN 51 = LOW (ON) (5 sec)");
  digitalWrite(51, LOW);
  delay(5000);

  Serial.println("PIN 51 = HIGH (OFF) (5 sec)");
  digitalWrite(51, HIGH);
  delay(5000);
}
