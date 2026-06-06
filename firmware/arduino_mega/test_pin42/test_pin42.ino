/**
 * TEST PIN 42 ONLY - 10 sec cycles
 */
void setup() {
  Serial.begin(115200);
  pinMode(42, OUTPUT);
  digitalWrite(42, HIGH);
  
  Serial.println("\n=== PIN 42 TEST (LOW = ON) ===\n");
  delay(3000);
}

void loop() {
  Serial.println(">>> PIN 42 = LOW (ON)");
  digitalWrite(42, LOW);
  
  for (int i = 1; i <= 10; i++) {
    Serial.print("  ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }
  
  Serial.println(">>> PIN 42 = HIGH (OFF)\n");
  digitalWrite(42, HIGH);
  delay(5000);
}
