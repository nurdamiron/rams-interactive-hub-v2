/**
 * Test PIN 50-51 PAIR (LOW = ON)
 * PIN 50 = UP, PIN 51 = DOWN
 */

void setup() {
  Serial.begin(115200);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  Serial.println("\n=== PIN 50-51 PAIR TEST (LOW=ON) ===\n");
  delay(2000);
}

void loop() {
  Serial.println("\n>>> UP: PIN 51=LOW - MOVING UP!");
  digitalWrite(50, HIGH);  // DOWN off
  digitalWrite(51, LOW);   // UP on
  for (int i = 1; i <= 10; i++) {
    Serial.print("  UP: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  Serial.println("\n>>> STOP: Both HIGH");
  digitalWrite(51, HIGH);
  digitalWrite(50, HIGH);
  for (int i = 1; i <= 10; i++) {
    Serial.print("  PAUSE: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  Serial.println("\n>>> DOWN: BOTH LOW - MOVING DOWN!");
  digitalWrite(51, LOW);   // Both on for DOWN
  digitalWrite(50, LOW);
  for (int i = 1; i <= 10; i++) {
    Serial.print("  DOWN: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  Serial.println("\n>>> STOP: Both HIGH");
  digitalWrite(51, HIGH);
  digitalWrite(50, HIGH);
  for (int i = 1; i <= 10; i++) {
    Serial.print("  PAUSE: ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }
}
