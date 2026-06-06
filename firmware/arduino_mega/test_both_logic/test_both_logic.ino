/**
 * Arduino Mega - Test Both Logic Types
 * Тестирует Block 1 с обеими логиками
 */

// Block 1 пины
#define UP_PIN1   22
#define UP_PIN2   23
#define DOWN_PIN1 24
#define DOWN_PIN2 25

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  TESTING BOTH RELAY LOGICS");
  Serial.println("  Block 1 UP pins: 22, 23");
  Serial.println("========================================\n");

  // Инициализация пинов
  pinMode(UP_PIN1, OUTPUT);
  pinMode(UP_PIN2, OUTPUT);
  pinMode(DOWN_PIN1, OUTPUT);
  pinMode(DOWN_PIN2, OUTPUT);

  // Начальное состояние - всё выключено
  digitalWrite(UP_PIN1, LOW);
  digitalWrite(UP_PIN2, LOW);
  digitalWrite(DOWN_PIN1, LOW);
  digitalWrite(DOWN_PIN2, LOW);

  delay(2000);
}

void loop() {
  // ============ TEST 1: HIGH = ON ============
  Serial.println("\n=== TEST 1: HIGH = ON (normal logic) ===");
  Serial.println("Setting pins 22,23 = HIGH");
  Serial.println("DOWN pins 24,25 = LOW");

  digitalWrite(UP_PIN1, HIGH);
  digitalWrite(UP_PIN2, HIGH);
  digitalWrite(DOWN_PIN1, LOW);
  digitalWrite(DOWN_PIN2, LOW);

  Serial.println("⏳ Waiting 5 seconds - CHECK IF MOTOR RUNS!");
  delay(5000);

  Serial.println("Stopping...\n");
  digitalWrite(UP_PIN1, LOW);
  digitalWrite(UP_PIN2, LOW);
  delay(2000);

  // ============ TEST 2: LOW = ON ============
  Serial.println("=== TEST 2: LOW = ON (inverted logic) ===");
  Serial.println("Setting pins 22,23 = LOW");
  Serial.println("DOWN pins 24,25 = HIGH");

  digitalWrite(UP_PIN1, LOW);
  digitalWrite(UP_PIN2, LOW);
  digitalWrite(DOWN_PIN1, HIGH);
  digitalWrite(DOWN_PIN2, HIGH);

  Serial.println("⏳ Waiting 5 seconds - CHECK IF MOTOR RUNS!");
  delay(5000);

  Serial.println("Stopping...\n");
  digitalWrite(UP_PIN1, HIGH);
  digitalWrite(UP_PIN2, HIGH);
  digitalWrite(DOWN_PIN1, HIGH);
  digitalWrite(DOWN_PIN2, HIGH);
  delay(2000);

  Serial.println("========================================");
  Serial.println("TESTS COMPLETE - Repeating...");
  Serial.println("========================================\n");
  delay(3000);
}
