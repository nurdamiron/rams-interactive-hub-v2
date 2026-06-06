/**
 * Arduino Mega - Full Cycle Test
 * Сначала UP, потом DOWN для каждого пина отдельно
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  FULL CYCLE TEST - Block 8");
  Serial.println("========================================\n");

  pinMode(46, OUTPUT);  // Act1 UP
  pinMode(47, OUTPUT);  // Act1 DOWN
  pinMode(48, OUTPUT);  // Act2 UP
  pinMode(49, OUTPUT);  // Act2 DOWN

  // Все OFF
  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Block 8 ready\n");
  delay(3000);
}

void loop() {
  // ========== ACTUATOR 1 TEST ==========
  Serial.println("\n========== ACTUATOR 1 ==========");

  Serial.println(">>> PIN 46 UP (10s)");
  digitalWrite(46, LOW);   // Act1 UP
  digitalWrite(47, HIGH);  // Act1 DOWN off
  delay(10000);
  digitalWrite(46, HIGH);
  Serial.println("STOP (1s)");
  delay(1000);

  Serial.println(">>> PIN 47 DOWN (10s)");
  digitalWrite(47, LOW);   // Act1 DOWN
  digitalWrite(46, HIGH);  // Act1 UP off
  delay(10000);
  digitalWrite(47, HIGH);
  Serial.println("STOP (2s)\n");
  delay(2000);

  // ========== ACTUATOR 2 TEST ==========
  Serial.println("========== ACTUATOR 2 ==========");

  Serial.println(">>> PIN 48 UP (10s)");
  digitalWrite(48, LOW);   // Act2 UP
  digitalWrite(49, HIGH);  // Act2 DOWN off
  delay(10000);
  digitalWrite(48, HIGH);
  Serial.println("STOP (1s)");
  delay(1000);

  Serial.println(">>> PIN 49 DOWN (10s)");
  digitalWrite(49, LOW);   // Act2 DOWN
  digitalWrite(48, HIGH);  // Act2 UP off
  delay(10000);
  digitalWrite(49, HIGH);
  Serial.println("STOP (2s)\n");
  delay(2000);

  // ========== BOTH TOGETHER ==========
  Serial.println("========== BOTH TOGETHER ==========");

  Serial.println(">>> BOTH UP (10s)");
  digitalWrite(46, LOW);   // Act1 UP
  digitalWrite(48, LOW);   // Act2 UP
  delay(10000);
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  Serial.println("STOP (1s)");
  delay(1000);

  Serial.println(">>> BOTH DOWN (10s)");
  digitalWrite(47, LOW);   // Act1 DOWN
  digitalWrite(49, LOW);   // Act2 DOWN
  delay(10000);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  Serial.println("STOP\n");
  delay(3000);

  Serial.println("========================================");
  Serial.println("CYCLE COMPLETE! Repeating...\n");
  delay(2000);
}
