/**
 * Find correct UP/DOWN logic for pins 50-51
 */

void setup() {
  Serial.begin(115200);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);

  Serial.println("\n========================================");
  Serial.println("  FIND UP/DOWN LOGIC - PINS 50-51");
  Serial.println("========================================\n");
  delay(3000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("TEST 1: PIN 50=LOW, PIN 51=HIGH");
  Serial.println("Watch actuator! (5 sec)");
  Serial.println("========================================");
  digitalWrite(50, LOW);
  digitalWrite(51, HIGH);
  delay(5000);
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);
  Serial.println("STOP - Pause 5 sec\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("TEST 2: PIN 50=HIGH, PIN 51=LOW");
  Serial.println("Watch actuator! (5 sec)");
  Serial.println("========================================");
  digitalWrite(50, HIGH);
  digitalWrite(51, LOW);
  delay(5000);
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);
  Serial.println("STOP - Pause 5 sec\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("TEST 3: BOTH LOW (50=LOW, 51=LOW)");
  Serial.println("Watch actuator! (5 sec)");
  Serial.println("========================================");
  digitalWrite(50, LOW);
  digitalWrite(51, LOW);
  delay(5000);
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);
  Serial.println("STOP - Pause 5 sec\n");
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("ALL TESTS DONE!");
  Serial.println("Tell me which test moved actuator UP/DOWN");
  Serial.println("Repeating in 10 sec...");
  Serial.println("========================================\n");
  delay(10000);
}
