/**
 * Test PIN 52 ONLY - 5 sec cycles (LOW = ON)
 */

void setup() {
  Serial.begin(115200);
  pinMode(52, OUTPUT);
  digitalWrite(52, HIGH);
  Serial.println("\n=== PIN 52 TEST (LOW = ON) ===\n");
}

void loop() {
  Serial.println("PIN 52 = LOW (ON) (5 sec)");
  digitalWrite(52, LOW);
  delay(5000);

  Serial.println("PIN 52 = HIGH (OFF) (5 sec)");
  digitalWrite(52, HIGH);
  delay(5000);
}
