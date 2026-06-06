/**
 * Test PIN 53-54 PAIR (LOW = ON)
 */

void setup() {
  Serial.begin(115200);
  pinMode(53, OUTPUT);
  pinMode(54, OUTPUT);

  digitalWrite(53, HIGH);
  digitalWrite(54, HIGH);

  Serial.println("\n=== PIN 53-54 PAIR TEST ===\n");
  delay(2000);
}

void loop() {
  Serial.println(">>> PIN 53 = LOW (5 sec)");
  digitalWrite(54, HIGH);  // 54 OFF
  digitalWrite(53, LOW);   // 53 ON
  delay(5000);
  Serial.println("    STOP: Both HIGH\n");
  digitalWrite(53, HIGH);
  digitalWrite(54, HIGH);
  delay(3000);

  Serial.println(">>> PIN 54 = LOW (5 sec)");
  digitalWrite(53, HIGH);  // 53 OFF
  digitalWrite(54, LOW);   // 54 ON
  delay(5000);
  Serial.println("    STOP: Both HIGH\n");
  digitalWrite(53, HIGH);
  digitalWrite(54, HIGH);
  delay(3000);
}
