/**
 * Arduino Mega - Which DOWN pin works?
 * Проверяем 47 и 49 отдельно с большой паузой
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  WHICH DOWN PIN WORKS?");
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
  // Поднять оба
  Serial.println("\n>>> BOTH UP (10s)");
  Serial.println("2 relays should turn ON (46, 48)");
  digitalWrite(46, LOW);
  digitalWrite(48, LOW);
  delay(10000);
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  Serial.println("STOP (5s)\n");
  delay(5000);

  // Тест PIN 47 отдельно
  Serial.println("===========================================");
  Serial.println(">>> TEST PIN 47 ONLY (15s)");
  Serial.println("CHECK: Does relay 47 turn ON?");
  Serial.println("Look at the relay board!");
  Serial.println("===========================================");
  digitalWrite(47, LOW);
  digitalWrite(49, HIGH);  // 49 точно OFF
  delay(15000);
  digitalWrite(47, HIGH);
  Serial.println("PIN 47 OFF\n");
  delay(5000);

  // Поднять снова
  Serial.println(">>> BOTH UP again (10s)");
  digitalWrite(46, LOW);
  digitalWrite(48, LOW);
  delay(10000);
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  Serial.println("STOP (5s)\n");
  delay(5000);

  // Тест PIN 49 отдельно
  Serial.println("===========================================");
  Serial.println(">>> TEST PIN 49 ONLY (15s)");
  Serial.println("CHECK: Does relay 49 turn ON?");
  Serial.println("Look at the relay board!");
  Serial.println("===========================================");
  digitalWrite(47, HIGH);  // 47 точно OFF
  digitalWrite(49, LOW);
  delay(15000);
  digitalWrite(49, HIGH);
  Serial.println("PIN 49 OFF\n");
  delay(5000);

  Serial.println("===========================================");
  Serial.println("RESULTS:");
  Serial.println("Which relay turned ON?");
  Serial.println("- PIN 47 relay ON? Tell me!");
  Serial.println("- PIN 49 relay ON? Tell me!");
  Serial.println("===========================================\n");
  delay(5000);
}
