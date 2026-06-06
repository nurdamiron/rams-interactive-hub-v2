/**
 * Arduino Mega #2 - Test Pins 50-53
 * Медленный тест каждого пина
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST PINS 50-53");
  Serial.println("========================================\n");

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);
  digitalWrite(52, HIGH);
  digitalWrite(53, HIGH);

  Serial.println("[INIT] Pins 50-53 ready\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 50 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(50, LOW);
  delay(10000);
  digitalWrite(50, HIGH);
  Serial.println("PIN 50 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 51 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(51, LOW);
  delay(10000);
  digitalWrite(51, HIGH);
  Serial.println("PIN 51 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 52 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(52, LOW);
  delay(10000);
  digitalWrite(52, HIGH);
  Serial.println("PIN 52 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 53 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(53, LOW);
  delay(10000);
  digitalWrite(53, HIGH);
  Serial.println("PIN 53 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("PINS 50-53 TESTED!");
  Serial.println("Which pins worked?");
  Serial.println("Repeating in 10s...");
  Serial.println("========================================\n");
  delay(10000);
}
