/**
 * Arduino Mega #2 - Test Pins 46-49
 * Тестируем следующие 4 пина
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST PINS 46-49 (Second 4 pins)");
  Serial.println("========================================\n");

  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);

  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Pins 46-49 ready (all HIGH)\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("TEST 1: Check each pin separately");
  Serial.println("========================================\n");

  Serial.println(">>> PIN 46 = LOW (5s)");
  digitalWrite(46, LOW);
  delay(5000);
  digitalWrite(46, HIGH);
  delay(2000);

  Serial.println(">>> PIN 47 = LOW (5s)");
  digitalWrite(47, LOW);
  delay(5000);
  digitalWrite(47, HIGH);
  delay(2000);

  Serial.println(">>> PIN 48 = LOW (5s)");
  digitalWrite(48, LOW);
  delay(5000);
  digitalWrite(48, HIGH);
  delay(2000);

  Serial.println(">>> PIN 49 = LOW (5s)");
  digitalWrite(49, LOW);
  delay(5000);
  digitalWrite(49, HIGH);
  delay(2000);

  Serial.println("\n========================================");
  Serial.println("TEST 2: Try both actuators");
  Serial.println("Assuming: 46,48=UP | 47,49=DOWN");
  Serial.println("========================================\n");

  Serial.println(">>> BOTH UP: PIN 46,48 = LOW (5s)");
  digitalWrite(46, LOW);
  digitalWrite(48, LOW);
  delay(5000);
  digitalWrite(46, HIGH);
  digitalWrite(48, HIGH);
  delay(2000);

  Serial.println(">>> BOTH DOWN: PIN 47,49 = LOW (5s)");
  digitalWrite(47, LOW);
  digitalWrite(49, LOW);
  delay(5000);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println("CYCLE COMPLETE!");
  Serial.println("Did pins 46-49 control any actuators?");
  Serial.println("Repeating in 3s...");
  Serial.println("========================================\n");
  delay(3000);
}
