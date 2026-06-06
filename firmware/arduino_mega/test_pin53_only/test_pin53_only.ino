/**
 * Test PIN 53 ONLY
 */

void setup() {
  Serial.begin(115200);
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH); // HIGH = ON

  Serial.println("PIN 53 = HIGH (ON)");
  Serial.println("Actuator should be moving!");
}

void loop() {
  // PIN 53 всегда HIGH
}
