/**
 * Arduino Mega - One Block Full Test
 * Тестирует Block 1 полностью - UP и DOWN
 * Только по 1 пину на направление
 */

// Block 1 - только первый пин из каждой пары
#define UP_PIN   22  // Первый UP пин
#define DOWN_PIN 24  // Первый DOWN пин

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  BLOCK 1 FULL TEST - UP & DOWN");
  Serial.println("  UP pin: 22");
  Serial.println("  DOWN pin: 24");
  Serial.println("========================================\n");

  pinMode(UP_PIN, OUTPUT);
  pinMode(DOWN_PIN, OUTPUT);

  digitalWrite(UP_PIN, LOW);
  digitalWrite(DOWN_PIN, LOW);

  Serial.println("[INIT] Ready");
  delay(2000);
}

void loop() {
  // ============ UP ============
  Serial.println("\n=== BLOCK 1 UP ===");
  Serial.println("Pin 22 = HIGH, Pin 24 = LOW");

  digitalWrite(UP_PIN, HIGH);
  digitalWrite(DOWN_PIN, LOW);

  Serial.println("⏳ 8 seconds - MOTOR SHOULD GO UP!");
  delay(8000);

  // STOP
  Serial.println("STOP - both pins LOW");
  digitalWrite(UP_PIN, LOW);
  digitalWrite(DOWN_PIN, LOW);
  delay(2000);

  // ============ DOWN ============
  Serial.println("\n=== BLOCK 1 DOWN ===");
  Serial.println("Pin 22 = LOW, Pin 24 = HIGH");

  digitalWrite(UP_PIN, LOW);
  digitalWrite(DOWN_PIN, HIGH);

  Serial.println("⏳ 8 seconds - MOTOR SHOULD GO DOWN!");
  delay(8000);

  // STOP
  Serial.println("STOP - both pins LOW");
  digitalWrite(UP_PIN, LOW);
  digitalWrite(DOWN_PIN, LOW);
  delay(3000);

  Serial.println("\n========================================");
  Serial.println("Cycle complete - repeating...");
  Serial.println("========================================\n");
  delay(2000);
}
