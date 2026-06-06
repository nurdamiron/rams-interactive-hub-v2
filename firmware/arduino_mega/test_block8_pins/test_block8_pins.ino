/**
 * Arduino Mega - Test Block 8 Each Pin
 * Тестирует каждый пин Block 8 отдельно
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  BLOCK 8 - EACH PIN TEST");
  Serial.println("========================================\n");

  // Block 8 пины (по текущему маппингу)
  pinMode(46, OUTPUT);  // Act1 UP
  pinMode(47, OUTPUT);  // Act1 DOWN
  pinMode(48, OUTPUT);  // Act2 UP
  pinMode(49, OUTPUT);  // Act2 DOWN

  // Все OFF
  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);

  Serial.println("[INIT] Block 8 pins ready\n");
  delay(2000);
}

void loop() {
  Serial.println("\n========================================");
  Serial.println("Testing Block 8 - Each pin separately");
  Serial.println("========================================\n");

  // Тест PIN 46 (Act1 UP)
  Serial.println(">>> PIN 46 = LOW (5s)");
  Serial.println("Expected: Actuator 1 UP");
  digitalWrite(46, LOW);
  delay(5000);
  digitalWrite(46, HIGH);
  Serial.println("STOP\n");
  delay(2000);

  // Тест PIN 47 (Act1 DOWN)
  Serial.println(">>> PIN 47 = LOW (5s)");
  Serial.println("Expected: Actuator 1 DOWN");
  digitalWrite(47, LOW);
  delay(5000);
  digitalWrite(47, HIGH);
  Serial.println("STOP\n");
  delay(2000);

  // Тест PIN 48 (Act2 UP)
  Serial.println(">>> PIN 48 = LOW (5s)");
  Serial.println("Expected: Actuator 2 UP");
  digitalWrite(48, LOW);
  delay(5000);
  digitalWrite(48, HIGH);
  Serial.println("STOP\n");
  delay(2000);

  // Тест PIN 49 (Act2 DOWN)
  Serial.println(">>> PIN 49 = LOW (5s)");
  Serial.println("Expected: Actuator 2 DOWN");
  digitalWrite(49, LOW);
  delay(5000);
  digitalWrite(49, HIGH);
  Serial.println("STOP\n");
  delay(2000);

  Serial.println("========================================");
  Serial.println("All pins tested! Repeating...\n");
  delay(3000);
}
