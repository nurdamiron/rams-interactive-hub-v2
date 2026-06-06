/**
 * Test PIN 50 ONLY - 5 sec cycles (LOW = ON)
 */

void setup() {
  Serial.begin(115200);
  pinMode(50, OUTPUT);
  digitalWrite(50, HIGH);
  Serial.println("\n=== PIN 50 TEST (LOW = ON) ===\n");
}

void loop() {
  Serial.println("PIN 50 = LOW (ON) (5 sec)");
  digitalWrite(50, LOW);
  delay(5000);

  Serial.println("PIN 50 = HIGH (OFF) (5 sec)");
  digitalWrite(50, HIGH);
  delay(5000);
}
