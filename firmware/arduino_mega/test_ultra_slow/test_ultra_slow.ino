/**
 * Arduino Mega #2 - Ultra Slow Test
 * Каждый тест 30 секунд с огромными надписями
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  ULTRA SLOW TEST - 30 SEC EACH");
  Serial.println("========================================\n");

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, LOW);
  digitalWrite(51, LOW);

  Serial.println("[INIT] Both pins LOW\n");
  delay(5000);
}

void loop() {
  Serial.println("\n\n");
  Serial.println("****************************************");
  Serial.println("*                                      *");
  Serial.println("*   TEST 1: PIN 50 = HIGH (30 SEC)    *");
  Serial.println("*   PIN 51 = LOW                       *");
  Serial.println("*   WATCH FOR UP MOVEMENT!             *");
  Serial.println("*                                      *");
  Serial.println("****************************************\n");
  digitalWrite(50, HIGH);
  digitalWrite(51, LOW);
  delay(30000);
  digitalWrite(50, LOW);
  Serial.println("\nPIN 50 = LOW (waiting 10s)\n\n");
  delay(10000);

  Serial.println("\n\n");
  Serial.println("****************************************");
  Serial.println("*                                      *");
  Serial.println("*   TEST 2: PIN 51 = HIGH (30 SEC)    *");
  Serial.println("*   PIN 50 = LOW                       *");
  Serial.println("*   WATCH FOR DOWN MOVEMENT!           *");
  Serial.println("*                                      *");
  Serial.println("****************************************\n");
  digitalWrite(50, LOW);
  digitalWrite(51, HIGH);
  delay(30000);
  digitalWrite(51, LOW);
  Serial.println("\nPIN 51 = LOW (waiting 10s)\n\n");
  delay(10000);

  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("CYCLE COMPLETE!");
  Serial.println("Results:");
  Serial.println("- TEST 1 (PIN 50=HIGH): Did actuator go UP?");
  Serial.println("- TEST 2 (PIN 51=HIGH): Did actuator go DOWN?");
  Serial.println("========================================");
  Serial.println("\nRepeating in 15 seconds...\n\n");
  delay(15000);
}
