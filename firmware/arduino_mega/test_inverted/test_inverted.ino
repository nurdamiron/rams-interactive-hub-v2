/**
 * Arduino Mega - Inverted Logic Test
 * LOW = ON, HIGH = OFF
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  INVERTED LOGIC TEST");
  Serial.println("  LOW = ON, HIGH = OFF");
  Serial.println("========================================\n");

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);

  // Начальное состояние - всё OFF (HIGH)
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);

  Serial.println("[INIT] All pins HIGH (OFF)\n");
  delay(2000);
}

void loop() {
  // ============ UP ============
  Serial.println(">>> GOING UP <<<");
  Serial.println("Pins 22,23 = LOW (ON)");
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);

  delay(10000);  // 10 секунд

  // STOP - всё HIGH
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  Serial.println("STOPPED (all HIGH)\n");
  delay(1000);

  // ============ DOWN ============
  Serial.println(">>> GOING DOWN <<<");
  Serial.println("Pins 24,25 = LOW (ON)");
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);

  delay(10000);  // 10 секунд

  // STOP - всё HIGH
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  Serial.println("STOPPED (all HIGH)\n");
  delay(1000);

  Serial.println("--- Cycle complete ---\n");
  delay(1000);
}
