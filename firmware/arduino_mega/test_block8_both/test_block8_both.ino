/**
 * Arduino Mega - Block 8 Both Actuators Test
 * Оба актуатора вместе UP и DOWN по 10 секунд
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  BLOCK 8 - BOTH ACTUATORS TEST");
  Serial.println("========================================\n");

  // Block 8 пины
  pinMode(46, OUTPUT);  // Act1 UP
  pinMode(47, OUTPUT);  // Act1 DOWN
  pinMode(48, OUTPUT);  // Act2 UP
  pinMode(49, OUTPUT);  // Act2 DOWN

  // Все OFF (HIGH)
  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Block 8 ready\n");
  delay(3000);
}

void loop() {
  Serial.println("\n>>> BOTH UP (10s)");
  // UP: пины 46,48 = LOW
  digitalWrite(46, LOW);   // Act1 UP
  digitalWrite(48, LOW);   // Act2 UP
  digitalWrite(47, HIGH);  // Act1 DOWN off
  digitalWrite(49, HIGH);  // Act2 DOWN off
  delay(10000);

  // STOP
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  Serial.println("STOP\n");
  delay(1000);

  Serial.println(">>> BOTH DOWN (10s)");
  // DOWN: пины 47,49 = LOW
  digitalWrite(46, HIGH);  // Act1 UP off
  digitalWrite(48, HIGH);  // Act2 UP off
  digitalWrite(47, LOW);   // Act1 DOWN
  digitalWrite(49, LOW);   // Act2 DOWN
  delay(10000);

  // STOP
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  Serial.println("STOP\n");
  delay(1000);
}
