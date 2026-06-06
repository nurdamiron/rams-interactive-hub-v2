/**
 * TEST: Scan pins 42-45 one by one
 */

int testPins[] = {42, 43, 44, 45};
int numPins = 4;
int currentPin = 0;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < numPins; i++) {
    pinMode(testPins[i], OUTPUT);
    digitalWrite(testPins[i], HIGH);
  }

  Serial.println("\n========================================");
  Serial.println("SCANNING PINS 42-45");
  Serial.println("========================================\n");
  delay(3000);
}

void loop() {
  int pin = testPins[currentPin];
  
  Serial.print("\n>>> Testing PIN ");
  Serial.print(pin);
  Serial.println(" = LOW");
  
  digitalWrite(pin, LOW);

  for (int i = 1; i <= 10; i++) {
    Serial.print("  ");
    Serial.print(i);
    Serial.println(" sec");
    delay(1000);
  }

  digitalWrite(pin, HIGH);
  Serial.println("    STOP");
  delay(5000);

  currentPin++;
  if (currentPin >= numPins) {
    Serial.println("\n========================================");
    Serial.println("SCAN COMPLETE! Restarting...");
    Serial.println("========================================\n");
    currentPin = 0;
    delay(5000);
  }
}
