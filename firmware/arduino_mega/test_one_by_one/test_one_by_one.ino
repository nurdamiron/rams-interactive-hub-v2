/**
 * Arduino Mega #2 - One Pin at a Time
 * Очень медленный тест - каждый пин с большой паузой
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  ONE PIN AT A TIME - SLOW TEST");
  Serial.println("========================================\n");

  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);

  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Ready\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 46 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(46, LOW);
  delay(10000);
  digitalWrite(46, HIGH);
  Serial.println("PIN 46 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 47 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(47, LOW);
  delay(10000);
  digitalWrite(47, HIGH);
  Serial.println("PIN 47 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 48 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(48, LOW);
  delay(10000);
  digitalWrite(48, HIGH);
  Serial.println("PIN 48 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("========================================");
  Serial.println(">>> PIN 49 = LOW");
  Serial.println("WATCH NOW! (10 seconds)");
  Serial.println("========================================");
  Serial.println("========================================\n");
  digitalWrite(49, LOW);
  delay(10000);
  digitalWrite(49, HIGH);
  Serial.println("PIN 49 OFF\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("ALL 4 PINS TESTED!");
  Serial.println("Which pins made actuators move?");
  Serial.println("Repeating in 10 seconds...");
  Serial.println("========================================\n");
  delay(10000);
}
