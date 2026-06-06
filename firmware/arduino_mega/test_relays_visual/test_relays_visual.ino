/**
 * Arduino Mega - Visual Relay Test
 * Сначала UP (чтобы было куда спускаться), потом проверяем DOWN реле
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  VISUAL RELAY TEST - Block 8");
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

  Serial.println("[INIT] All pins ready\n");
  delay(3000);
}

void loop() {
  // ЭТАП 1: Поднять оба актуатора
  Serial.println("\n===========================================");
  Serial.println("STAGE 1: BOTH UP (10s)");
  Serial.println("CHECK: Do relays 46 and 48 turn ON?");
  Serial.println("===========================================");
  digitalWrite(46, LOW);
  digitalWrite(48, LOW);
  delay(10000);

  // STOP
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  Serial.println("\nSTOP - All relays OFF (3s)\n");
  delay(3000);

  // ЭТАП 2: Проверить реле PIN 47
  Serial.println("===========================================");
  Serial.println("STAGE 2: Test PIN 47 relay (10s)");
  Serial.println("CHECK: Does relay 47 turn ON? (look at LED)");
  Serial.println("===========================================");
  digitalWrite(47, LOW);
  delay(10000);
  digitalWrite(47, HIGH);
  Serial.println("\nSTOP (3s)\n");
  delay(3000);

  // ЭТАП 3: Проверить реле PIN 49
  Serial.println("===========================================");
  Serial.println("STAGE 3: Test PIN 49 relay (10s)");
  Serial.println("CHECK: Does relay 49 turn ON? (look at LED)");
  Serial.println("===========================================");
  digitalWrite(49, LOW);
  delay(10000);
  digitalWrite(49, HIGH);
  Serial.println("\nSTOP (3s)\n");
  delay(3000);

  // ЭТАП 4: Оба DOWN вместе
  Serial.println("===========================================");
  Serial.println("STAGE 4: BOTH DOWN (10s)");
  Serial.println("CHECK: Do relays 47 and 49 turn ON?");
  Serial.println("===========================================");
  digitalWrite(47, LOW);
  digitalWrite(49, LOW);
  delay(10000);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  Serial.println("\nSTOP\n");
  delay(3000);

  Serial.println("===========================================");
  Serial.println("CYCLE COMPLETE! Repeating in 5 seconds...");
  Serial.println("===========================================\n");
  delay(5000);
}
