/**
 * EMERGENCY: Lower ALL Actuators - Mega 2
 *
 * Опускает ВСЕ актуаторы блоков 9-15 БЕЗ ПОДНЯТИЯ
 * Для безопасности и сброса системы в начальное состояние
 *
 * @version 1.0
 * @date 2026-02-17
 */

#include "ACTUATOR_CONFIG.h"

#define LOWER_DURATION 8000  // 8 секунд на опускание

void setup() {
  Serial.begin(115200);
  Serial.println("\n========================================");
  Serial.println("  EMERGENCY: LOWERING ALL ACTUATORS");
  Serial.println("  Mega #2 - Blocks 9-15");
  Serial.println("========================================");

  // Инициализация всех пинов
  for (int i = MEGA2_BLOCK_START; i <= MEGA2_BLOCK_END; i++) {
    const BlockConfig* cfg = getBlockConfig(i);
    if (cfg == nullptr) continue;

    pinMode(cfg->actuator1.upPin, OUTPUT);
    pinMode(cfg->actuator1.downPin, OUTPUT);
    pinMode(cfg->actuator2.upPin, OUTPUT);
    pinMode(cfg->actuator2.downPin, OUTPUT);

    // Сначала все OFF
    digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
    digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
    digitalWrite(cfg->actuator2.upPin, RELAY_OFF);
    digitalWrite(cfg->actuator2.downPin, RELAY_OFF);

    // Актуатор 3 (только для блока 15)
    if (cfg->actuatorCount == 3) {
      pinMode(cfg->actuator3.upPin, OUTPUT);
      pinMode(cfg->actuator3.downPin, OUTPUT);
      digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
      digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
    }

    Serial.print("  Block ");
    Serial.print(i);
    Serial.println(" initialized");
  }

  Serial.println("\n[START] Lowering all actuators...");
  Serial.print("[TIME] Duration: ");
  Serial.print(LOWER_DURATION);
  Serial.println(" ms");

  // ОПУСТИТЬ ВСЕ АКТУАТОРЫ
  for (int i = MEGA2_BLOCK_START; i <= MEGA2_BLOCK_END; i++) {
    const BlockConfig* cfg = getBlockConfig(i);
    if (cfg == nullptr) continue;

    // DOWN = включить DOWN реле, выключить UP
    digitalWrite(cfg->actuator1.downPin, RELAY_ON);
    digitalWrite(cfg->actuator2.downPin, RELAY_ON);
    digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
    digitalWrite(cfg->actuator2.upPin, RELAY_OFF);

    // Актуатор 3 (только блок 15)
    if (cfg->actuatorCount == 3) {
      digitalWrite(cfg->actuator3.downPin, RELAY_ON);
      digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
    }

    Serial.print("  Block ");
    Serial.print(i);
    Serial.println(" DOWN");
  }

  // Ждем LOWER_DURATION
  Serial.println("\n[WAIT] Lowering in progress...");
  delay(LOWER_DURATION);

  // ОСТАНОВИТЬ ВСЕ
  Serial.println("\n[STOP] Stopping all actuators...");
  for (int i = MEGA2_BLOCK_START; i <= MEGA2_BLOCK_END; i++) {
    const BlockConfig* cfg = getBlockConfig(i);
    if (cfg == nullptr) continue;

    digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
    digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
    digitalWrite(cfg->actuator2.upPin, RELAY_OFF);
    digitalWrite(cfg->actuator2.downPin, RELAY_OFF);

    if (cfg->actuatorCount == 3) {
      digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
      digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
    }

    Serial.print("  Block ");
    Serial.print(i);
    Serial.println(" STOPPED");
  }

  Serial.println("\n========================================");
  Serial.println("  ✓ ALL ACTUATORS LOWERED");
  Serial.println("  System is now in SAFE position");
  Serial.println("========================================");
}

void loop() {
  // Ничего не делаем - все актуаторы опущены и остановлены
  delay(1000);
}
