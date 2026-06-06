/**
 * Arduino Mega - Simple UP/DOWN Test
 * Block 1 - только одна комбинация: 22,23=UP / 24,25=DOWN
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  SIMPLE UP/DOWN TEST");
  Serial.println("  UP: pins 22,23");
  Serial.println("  DOWN: pins 24,25");
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
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);

  delay(8000);

  // STOP
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  Serial.println("STOPPED\n");
  delay(1000);

  // ============ DOWN ============
  Serial.println(">>> GOING DOWN <<<");
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);

  delay(8000);

  // STOP
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);
  Serial.println("STOPPED\n");
  delay(1000);

  Serial.println("--- Cycle complete, repeating... ---\n");
  delay(1000);
}
