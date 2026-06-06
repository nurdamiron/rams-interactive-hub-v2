/**
 * Test PIN 48 ONLY - 5 sec cycles (LOW = ON)
 */

void setup() {
  Serial.begin(115200);
  pinMode(48, OUTPUT);
  digitalWrite(48, HIGH);
  Serial.println("\n=== PIN 48 TEST (LOW = ON) ===\n");
}

void loop() {
  Serial.println("PIN 48 = LOW (ON) (5 sec)");
  digitalWrite(48, LOW);
  delay(5000);

  Serial.println("PIN 48 = HIGH (OFF) (5 sec)");
  digitalWrite(48, HIGH);
  delay(5000);
}
