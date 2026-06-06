/**
 * Arduino Mega - Variant 2 Test
 * UP: pins 22,24
 * DOWN: pins 23,25
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  VARIANT 2 TEST");
  Serial.println("  UP: pins 22,24");
  Serial.println("  DOWN: pins 23,25");
  Serial.println("========================================\n");

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);

  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);

  Serial.println("[INIT] Ready\n");
  delay(2000);
}

void loop() {
  // ============ UP ============
  Serial.println(">>> GOING UP <<<");
  Serial.println("Pins 22,24 = HIGH");
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(25, LOW);

  delay(8000);

  // STOP
  digitalWrite(22, LOW);
  digitalWrite(24, LOW);
  Serial.println("STOPPED\n");
  delay(1000);

  // ============ DOWN ============
  Serial.println(">>> GOING DOWN <<<");
  Serial.println("Pins 23,25 = HIGH");
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  digitalWrite(24, LOW);
  digitalWrite(25, HIGH);

  delay(8000);

  // STOP
  digitalWrite(23, LOW);
  digitalWrite(25, LOW);
  Serial.println("STOPPED\n");
  delay(1000);

  Serial.println("--- Cycle complete ---\n");
  delay(1000);
}
