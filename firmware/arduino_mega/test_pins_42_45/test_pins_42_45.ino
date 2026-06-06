/**
 * Arduino Mega #2 - Test Pins 42-45
 * Тестируем 4 пина для одного блока
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST PINS 42-45 (First 4 pins)");
  Serial.println("========================================\n");

  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);

  // Начальное состояние - все HIGH (инверсная логика)
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
  digitalWrite(44, HIGH);
  digitalWrite(45, HIGH);

  Serial.println("[INIT] Pins 42-45 ready (all HIGH)\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("TEST 1: Check each pin separately");
  Serial.println("========================================\n");

  // Тест каждого пина отдельно (LOW = ON)
  Serial.println(">>> PIN 42 = LOW (5s)");
  digitalWrite(42, LOW);
  delay(5000);
  digitalWrite(42, HIGH);
  delay(2000);

  Serial.println(">>> PIN 43 = LOW (5s)");
  digitalWrite(43, LOW);
  delay(5000);
  digitalWrite(43, HIGH);
  delay(2000);

  Serial.println(">>> PIN 44 = LOW (5s)");
  digitalWrite(44, LOW);
  delay(5000);
  digitalWrite(44, HIGH);
  delay(2000);

  Serial.println(">>> PIN 45 = LOW (5s)");
  digitalWrite(45, LOW);
  delay(5000);
  digitalWrite(45, HIGH);
  delay(2000);

  Serial.println("\n========================================");
  Serial.println("TEST 2: Try both actuators UP");
  Serial.println("Assuming: 42,44=UP | 43,45=DOWN");
  Serial.println("========================================\n");

  Serial.println(">>> BOTH UP: PIN 42,44 = LOW (5s)");
  digitalWrite(42, LOW);
  digitalWrite(44, LOW);
  delay(5000);
  digitalWrite(42, HIGH);
  digitalWrite(44, HIGH);
  delay(2000);

  Serial.println(">>> BOTH DOWN: PIN 43,45 = LOW (5s)");
  digitalWrite(43, LOW);
  digitalWrite(45, LOW);
  delay(5000);
  digitalWrite(43, HIGH);
  digitalWrite(45, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println("CYCLE COMPLETE!");
  Serial.println("Did pins 42-45 control any actuators?");
  Serial.println("Repeating in 5s...");
  Serial.println("========================================\n");
  delay(5000);
}
