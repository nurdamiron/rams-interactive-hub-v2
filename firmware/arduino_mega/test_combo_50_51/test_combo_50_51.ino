/**
 * Arduino Mega #2 - Test Combinations with Pins 50-51
 * Пробуем разные комбинации HIGH/LOW
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST COMBINATIONS - PINS 50-51");
  Serial.println("========================================\n");

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, LOW);
  digitalWrite(51, LOW);

  Serial.println("[INIT] Both LOW\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("TEST 1: PIN 50 = HIGH, PIN 51 = LOW");
  Serial.println("(15 seconds)");
  Serial.println("========================================\n");
  digitalWrite(50, HIGH);
  digitalWrite(51, LOW);
  delay(15000);
  digitalWrite(50, LOW);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("TEST 2: PIN 50 = LOW, PIN 51 = HIGH");
  Serial.println("(15 seconds)");
  Serial.println("========================================\n");
  digitalWrite(50, LOW);
  digitalWrite(51, HIGH);
  delay(15000);
  digitalWrite(51, LOW);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("TEST 3: BOTH HIGH");
  Serial.println("(15 seconds)");
  Serial.println("========================================\n");
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);
  delay(15000);
  digitalWrite(50, LOW);
  digitalWrite(51, LOW);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("TEST 4: PIN 50 TOGGLE (UP?)");
  Serial.println("LOW->HIGH (15s) then HIGH->LOW (15s)");
  Serial.println("========================================\n");
  Serial.println("Setting PIN 50 = HIGH...");
  digitalWrite(50, HIGH);
  delay(15000);
  Serial.println("Setting PIN 50 = LOW...");
  digitalWrite(50, LOW);
  delay(15000);

  Serial.println("\n========================================");
  Serial.println("TEST 5: PIN 51 TOGGLE (DOWN?)");
  Serial.println("LOW->HIGH (15s) then HIGH->LOW (15s)");
  Serial.println("========================================\n");
  Serial.println("Setting PIN 51 = HIGH...");
  digitalWrite(51, HIGH);
  delay(15000);
  Serial.println("Setting PIN 51 = LOW...");
  digitalWrite(51, LOW);
  delay(15000);

  Serial.println("\n========================================");
  Serial.println("ALL TESTS DONE!");
  Serial.println("Did any test move the actuator?");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
