/**
 * Arduino Mega #2 - SUPER SLOW TEST
 * Очень медленный тест с большими паузами
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  SUPER SLOW TEST - PINS 50-53");
  Serial.println("  Logic: HIGH = ON");
  Serial.println("========================================\n");

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  digitalWrite(50, LOW);
  digitalWrite(51, LOW);
  digitalWrite(52, LOW);
  digitalWrite(53, LOW);

  Serial.println("[INIT] All pins LOW\n");
  delay(3000);
}

void loop() {
  Serial.println("\n\n\n");
  Serial.println("****************************************");
  Serial.println("****************************************");
  Serial.println("***                                  ***");
  Serial.println("***        PIN 50 = HIGH NOW!        ***");
  Serial.println("***     WATCH ACTUATOR! (15 SEC)     ***");
  Serial.println("***                                  ***");
  Serial.println("****************************************");
  Serial.println("****************************************\n");
  digitalWrite(50, HIGH);
  delay(15000);
  digitalWrite(50, LOW);
  Serial.println("\nPIN 50 OFF - Waiting 10 seconds...\n\n");
  delay(10000);

  Serial.println("\n\n\n");
  Serial.println("****************************************");
  Serial.println("****************************************");
  Serial.println("***                                  ***");
  Serial.println("***        PIN 51 = HIGH NOW!        ***");
  Serial.println("***     WATCH ACTUATOR! (15 SEC)     ***");
  Serial.println("***                                  ***");
  Serial.println("****************************************");
  Serial.println("****************************************\n");
  digitalWrite(51, HIGH);
  delay(15000);
  digitalWrite(51, LOW);
  Serial.println("\nPIN 51 OFF - Waiting 10 seconds...\n\n");
  delay(10000);

  Serial.println("\n\n\n");
  Serial.println("****************************************");
  Serial.println("****************************************");
  Serial.println("***                                  ***");
  Serial.println("***        PIN 52 = HIGH NOW!        ***");
  Serial.println("***     WATCH ACTUATOR! (15 SEC)     ***");
  Serial.println("***                                  ***");
  Serial.println("****************************************");
  Serial.println("****************************************\n");
  digitalWrite(52, HIGH);
  delay(15000);
  digitalWrite(52, LOW);
  Serial.println("\nPIN 52 OFF - Waiting 10 seconds...\n\n");
  delay(10000);

  Serial.println("\n\n\n");
  Serial.println("****************************************");
  Serial.println("****************************************");
  Serial.println("***                                  ***");
  Serial.println("***        PIN 53 = HIGH NOW!        ***");
  Serial.println("***     WATCH ACTUATOR! (15 SEC)     ***");
  Serial.println("***                                  ***");
  Serial.println("****************************************");
  Serial.println("****************************************\n");
  digitalWrite(53, HIGH);
  delay(15000);
  digitalWrite(53, LOW);
  Serial.println("\nPIN 53 OFF - Waiting 10 seconds...\n\n");
  delay(10000);

  Serial.println("\n\n========================================");
  Serial.println("ALL PINS TESTED!");
  Serial.println("Which pin made 3rd actuator move?");
  Serial.println("Repeating in 15 seconds...");
  Serial.println("========================================\n");
  delay(15000);
}
