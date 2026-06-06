/**
 * Test PIN 51-52 PAIR (LOW = ON)
 */

void setup() {
  Serial.begin(115200);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);

  digitalWrite(51, HIGH);
  digitalWrite(52, HIGH);

  Serial.println("\n=== PIN 51-52 PAIR TEST ===\n");
  delay(2000);
}

void loop() {
  Serial.println(">>> PIN 51 = LOW (5 sec)");
  digitalWrite(52, HIGH);  // 52 OFF
  digitalWrite(51, LOW);   // 51 ON
  delay(5000);
  Serial.println("    STOP: Both HIGH\n");
  digitalWrite(51, HIGH);
  digitalWrite(52, HIGH);
  delay(3000);

  Serial.println(">>> PIN 52 = LOW (5 sec)");
  digitalWrite(51, HIGH);  // 51 OFF
  digitalWrite(52, LOW);   // 52 ON
  delay(5000);
  Serial.println("    STOP: Both HIGH\n");
  digitalWrite(51, HIGH);
  digitalWrite(52, HIGH);
  delay(3000);
}
