/**
 * Arduino Mega - Single Block Test
 * Тестирует только Block 1 - поднять и держать
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
  Serial.println("  SINGLE BLOCK TEST - Block 1");
  Serial.println("========================================\n");

  // Инициализация пинов
  pinMode(UP_PIN1, OUTPUT);
  pinMode(UP_PIN2, OUTPUT);
  pinMode(DOWN_PIN1, OUTPUT);
  pinMode(DOWN_PIN2, OUTPUT);

  // ИНВЕРСНАЯ ЛОГИКА: HIGH = выключено
  digitalWrite(UP_PIN1, HIGH);
  digitalWrite(UP_PIN2, HIGH);
  digitalWrite(DOWN_PIN1, HIGH);
  digitalWrite(DOWN_PIN2, HIGH);

  Serial.println("[INIT] All relays OFF (pins HIGH)");
  Serial.println("\nWaiting 3 seconds...\n");
  delay(3000);

  // ПОДНЯТЬ Block 1
  Serial.println("=== RAISING BLOCK 1 ===");
  Serial.println("[UP] Pins 22,23 = LOW (relay ON)");

  digitalWrite(UP_PIN1, LOW);   // Реле включено
  digitalWrite(UP_PIN2, LOW);   // Реле включено
  digitalWrite(DOWN_PIN1, HIGH); // Реле выключено
  digitalWrite(DOWN_PIN2, HIGH); // Реле выключено

  Serial.println("\nBlock 1 is now UP and will stay UP.");
  Serial.println("Motor should be running now!");
  Serial.println("After ~10 seconds, motor should stop (actuator reached top).");
  Serial.println("\nPress RESET button to test again.\n");
}

void loop() {
  // Block 1 остаётся поднятым
  // Ничего не делаем в loop
}
