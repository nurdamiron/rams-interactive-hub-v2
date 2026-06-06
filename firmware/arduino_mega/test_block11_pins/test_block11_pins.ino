/**
 * Arduino Mega #2 - Find Block 11 Pins
 * Тестируем пины 34,35,36,37 (предполагаемый Block 11)
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  FIND BLOCK 11 (3rd) PINS");
  Serial.println("========================================\n");

  // Пины 34-37 (предполагаемый Block 11)
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);

  // Все HIGH = OFF
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
  digitalWrite(36, HIGH);
  digitalWrite(37, HIGH);

  Serial.println("[INIT] Pins 34-37 ready\n");
  delay(3000);
}

void loop() {
  // Тестируем каждый пин отдельно с обеими логиками

  Serial.println("\n========================================");
  Serial.println(">>> PIN 34 = LOW (5s)");
  Serial.println("WATCH: Which actuator? UP or DOWN?");
  Serial.println("========================================");
  digitalWrite(34, LOW);
  delay(5000);
  digitalWrite(34, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println(">>> PIN 35 = LOW (5s)");
  Serial.println("WATCH: Which actuator? UP or DOWN?");
  Serial.println("========================================");
  digitalWrite(35, LOW);
  delay(5000);
  digitalWrite(35, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println(">>> PIN 36 = LOW (5s)");
  Serial.println("WATCH: Which actuator? UP or DOWN?");
  Serial.println("========================================");
  digitalWrite(36, LOW);
  delay(5000);
  digitalWrite(36, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println(">>> PIN 37 = LOW (5s)");
  Serial.println("WATCH: Which actuator? UP or DOWN?");
  Serial.println("========================================");
  digitalWrite(37, LOW);
  delay(5000);
  digitalWrite(37, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println(">>> BOTH ACT UP: PIN 34,36 = LOW (5s)");
  Serial.println("WATCH: Both actuators UP?");
  Serial.println("========================================");
  digitalWrite(34, LOW);
  digitalWrite(36, LOW);
  delay(5000);
  digitalWrite(34, HIGH);
  digitalWrite(36, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println(">>> BOTH ACT DOWN: PIN 35,37 = LOW (5s)");
  Serial.println("WATCH: Both actuators DOWN?");
  Serial.println("========================================");
  digitalWrite(35, LOW);
  digitalWrite(37, LOW);
  delay(5000);
  digitalWrite(35, HIGH);
  digitalWrite(37, HIGH);
  delay(5000);

  Serial.println("\n========================================");
  Serial.println("CYCLE COMPLETE! Repeating...\n");
  delay(3000);
}
