/**
 * Arduino Mega #2 - Test 3rd Actuator (Pins 50-51)
 * Проверяем точно какой пин за что отвечает
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST 3RD ACTUATOR - PINS 50-51");
  Serial.println("========================================\n");

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  Serial.println("[INIT] Ready\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println(">>> PIN 50 = LOW (10s)");
  Serial.println("Is it UP or DOWN?");
  Serial.println("========================================\n");
  digitalWrite(50, LOW);
  delay(10000);
  digitalWrite(50, HIGH);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println(">>> PIN 51 = LOW (10s)");
  Serial.println("Is it UP or DOWN?");
  Serial.println("========================================\n");
  digitalWrite(51, LOW);
  delay(10000);
  digitalWrite(51, HIGH);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println(">>> BOTH UP & DOWN TEST");
  Serial.println("PIN 50 = LOW (UP?) then PIN 51 = LOW (DOWN?)");
  Serial.println("========================================\n");

  Serial.println("Testing PIN 50 (UP?) - 10s");
  digitalWrite(50, LOW);
  delay(10000);
  digitalWrite(50, HIGH);
  delay(2000);

  Serial.println("Testing PIN 51 (DOWN?) - 10s");
  digitalWrite(51, LOW);
  delay(10000);
  digitalWrite(51, HIGH);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("Results:");
  Serial.println("PIN 50 = ? (UP or DOWN)");
  Serial.println("PIN 51 = ? (UP or DOWN)");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
