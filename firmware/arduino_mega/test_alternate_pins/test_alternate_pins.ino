/**
 * Arduino Mega - Test Alternate Pins for Act2
 * Проверяем разные комбинации пинов для второго актуатора
 */

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TEST ALTERNATE PINS - Block 8");
  Serial.println("========================================\n");

  // Act1 точно работает на 46,47
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);

  // Попробуем разные пины для Act2
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  // Все OFF
  digitalWrite(46, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(48, HIGH);
  digitalWrite(49, HIGH);
  digitalWrite(50, HIGH);
  digitalWrite(51, HIGH);
  digitalWrite(52, HIGH);
  digitalWrite(53, HIGH);

  Serial.println("[INIT] All pins ready\n");
  delay(3000);
}

void loop() {
  // Act1 работает - поднимем его
  Serial.println(">>> Act1 UP (PIN 46) - 5s");
  digitalWrite(46, LOW);
  delay(5000);
  digitalWrite(46, HIGH);
  delay(2000);

  // Теперь тестируем каждый возможный пин для Act2
  Serial.println("\n=== Testing PIN 48 for Act2 UP ===");
  digitalWrite(48, LOW);
  delay(5000);
  digitalWrite(48, HIGH);
  delay(2000);

  Serial.println("=== Testing PIN 49 for Act2 UP ===");
  digitalWrite(49, LOW);
  delay(5000);
  digitalWrite(49, HIGH);
  delay(2000);

  Serial.println("=== Testing PIN 50 for Act2 UP ===");
  digitalWrite(50, LOW);
  delay(5000);
  digitalWrite(50, HIGH);
  delay(2000);

  Serial.println("=== Testing PIN 51 for Act2 UP ===");
  digitalWrite(51, LOW);
  delay(5000);
  digitalWrite(51, HIGH);
  delay(2000);

  Serial.println("=== Testing PIN 52 for Act2 UP ===");
  digitalWrite(52, LOW);
  delay(5000);
  digitalWrite(52, HIGH);
  delay(2000);

  Serial.println("=== Testing PIN 53 for Act2 UP ===");
  digitalWrite(53, LOW);
  delay(5000);
  digitalWrite(53, HIGH);
  delay(2000);

  // Act1 опустим
  Serial.println("\n>>> Act1 DOWN (PIN 47) - 5s");
  digitalWrite(47, LOW);
  delay(5000);
  digitalWrite(47, HIGH);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println("Cycle complete! Repeating...\n");
  delay(2000);
}
