/**
 * Arduino Mega - Simple Together Test
 * Оба актуатора ТОЧНО ВМЕСТЕ - UP и DOWN
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  SIMPLE TOGETHER TEST");
  Serial.println("========================================\n");

  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);

  // Все OFF
  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Block 8 ready\n");
  delay(3000);
}

void loop() {
  Serial.println("\n>>> BOTH UP (10s)");
  // Включаем оба UP пина ОДНОВРЕМЕННО
  digitalWrite(46, LOW);
  digitalWrite(48, LOW);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  delay(10000);

  // STOP
  Serial.println("STOP (1s)");
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  delay(1000);

  Serial.println("\n>>> BOTH DOWN (10s)");
  // Включаем оба DOWN пина ОДНОВРЕМЕННО
  digitalWrite(47, LOW);
  digitalWrite(49, LOW);
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  delay(10000);

  // STOP
  Serial.println("STOP (1s)\n");
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  delay(1000);

  Serial.println("========================================");
  Serial.println("Repeating...\n");
  delay(2000);
}
