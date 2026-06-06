/**
 * RAMS Actuator Controller - Arduino Mega #2
 *
 * Управляет блоками 9-15 (15 актуаторов: 6×2 + 1×3)
 * Простой текстовый протокол (стиль DroneControl.ino)
 * Использует общий конфигурационный файл ACTUATOR_CONFIG.h
 *
 * ВАЖНО: Блок 15 имеет 3 АКТУАТОРА (6 пинов: 42-47)
 *
 * Протокол связи с ESP32 (Serial1):
 * RX: BLOCK:9:UP:10000\n      - Блок 9 вверх на 10 сек
 * RX: BLOCK:15:DOWN:5000\n    - Блок 15 вниз на 5 сек
 * RX: BLOCK:12:STOP\n         - Остановить блок 12
 * RX: ALL:STOP\n              - Остановить все блоки
 * RX: PING\n                  - Проверка связи
 *
 * TX: ACK:9:UP\n              - Подтверждение
 * TX: DONE:9\n                - Блок завершил работу
 * TX: PONG\n                  - Ответ на пинг
 * TX: ERROR:message\n         - Ошибка
 *
 * @version 3.1 (DroneControl style)
 * @date 2026-02-15
 * @author RAMS Global Team
 */

#include "../shared/ACTUATOR_CONFIG.h"

// ============================================================================
// SERIAL CONFIGURATION
// ============================================================================

#define ESP32_SERIAL Serial1  // TX1 (pin 18), RX1 (pin 19)
#define DEBUG_SERIAL Serial   // USB serial для отладки

// ============================================================================
// СОСТОЯНИЕ БЛОКОВ
// ============================================================================

struct BlockState {
  bool isActive;
  unsigned long startTime;
  int duration;
};

// Состояния блоков 9-15 (индекс 0 не используется)
BlockState blockStates[MEGA2_BLOCK_COUNT + 1];

// Буфер для команд
String cmdRaw;
int blockNum;
char action[10];
int duration;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // Debug serial
  DEBUG_SERIAL.begin(SERIAL_BAUD);
  DEBUG_SERIAL.println("\n========================================");
  DEBUG_SERIAL.println("  RAMS ACTUATOR CONTROLLER - MEGA #2");
  DEBUG_SERIAL.println("  Blocks 9-15 | DroneControl Style");
  DEBUG_SERIAL.println("========================================");

  // Инициализация всех пинов
  for (int i = MEGA2_BLOCK_START; i <= MEGA2_BLOCK_END; i++) {
    const BlockConfig* cfg = getBlockConfig(i);
    if (cfg == nullptr) continue;

    pinMode(cfg->actuator1.upPin, OUTPUT);
    pinMode(cfg->actuator1.downPin, OUTPUT);
    pinMode(cfg->actuator2.upPin, OUTPUT);
    pinMode(cfg->actuator2.downPin, OUTPUT);

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

    DEBUG_SERIAL.print("  Block ");
    DEBUG_SERIAL.print(i);
    DEBUG_SERIAL.print(": [");
    DEBUG_SERIAL.print(cfg->actuator1.upPin);
    DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print(cfg->actuator1.downPin);
    DEBUG_SERIAL.print("] [");
    DEBUG_SERIAL.print(cfg->actuator2.upPin);
    DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print(cfg->actuator2.downPin);
    DEBUG_SERIAL.print("]");

    if (cfg->actuatorCount == 3) {
      DEBUG_SERIAL.print(" [");
      DEBUG_SERIAL.print(cfg->actuator3.upPin);
      DEBUG_SERIAL.print(",");
      DEBUG_SERIAL.print(cfg->actuator3.downPin);
      DEBUG_SERIAL.print("]");
    }

    DEBUG_SERIAL.println();
  }

  // Инициализация состояний
  for (int i = 0; i <= MEGA2_BLOCK_COUNT; i++) {
    blockStates[i].isActive = false;
    blockStates[i].startTime = 0;
    blockStates[i].duration = 0;
  }

  // ESP32 serial
  ESP32_SERIAL.begin(SERIAL_BAUD);

  DEBUG_SERIAL.println("[READY] System initialized!\n");
}

// ============================================================================
// MAIN LOOP - СТИЛЬ DroneControl.ino
// ============================================================================

void loop() {
  // ===== ЧТЕНИЕ КОМАНД ОТ ESP32 =====
  if (ESP32_SERIAL.available()) {
    cmdRaw = ESP32_SERIAL.readStringUntil('\n');
    cmdRaw.trim();

    if (cmdRaw.length() > 0) {
      DEBUG_SERIAL.print("[RX] ");
      DEBUG_SERIAL.println(cmdRaw);

      // PING
      if (cmdRaw == CMD_PING) {
        ESP32_SERIAL.println(CMD_PONG);
        DEBUG_SERIAL.println("[TX] PONG");
      }
      // ALL:STOP
      else if (cmdRaw.startsWith(CMD_ALL_STOP)) {
        DEBUG_SERIAL.println("[ALL] STOP ALL");

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

          int idx = i - MEGA2_BLOCK_START + 1;
          blockStates[idx].isActive = false;
        }

        ESP32_SERIAL.println("ACK:0:STOP");
      }
      // BLOCK:N:ACTION:DURATION
      else if (cmdRaw.startsWith(CMD_BLOCK)) {
        blockNum = 0;
        duration = 0;
        memset(action, 0, sizeof(action));

        // Парсинг как в DroneControl.ino
        int parsed = sscanf(cmdRaw.c_str(), "BLOCK:%d:%[^:]:%d", &blockNum, action, &duration);

        if (parsed >= 2 && blockNum >= MEGA2_BLOCK_START && blockNum <= MEGA2_BLOCK_END) {
          const BlockConfig* cfg = getBlockConfig(blockNum);
          int idx = blockNum - MEGA2_BLOCK_START + 1;

          if (duration == 0) duration = DEFAULT_DURATION_MS;

          // UP
          if (strcmp(action, ACTION_UP) == 0) {
            digitalWrite(cfg->actuator1.upPin, RELAY_ON);
            digitalWrite(cfg->actuator2.upPin, RELAY_ON);
            digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
            digitalWrite(cfg->actuator2.downPin, RELAY_OFF);

            // Актуатор 3 (только блок 15)
            if (cfg->actuatorCount == 3) {
              digitalWrite(cfg->actuator3.upPin, RELAY_ON);
              digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
            }

            blockStates[idx].isActive = true;
            blockStates[idx].startTime = millis();
            blockStates[idx].duration = duration;

            DEBUG_SERIAL.print("[BLOCK ");
            DEBUG_SERIAL.print(blockNum);
            DEBUG_SERIAL.print("] UP ");
            DEBUG_SERIAL.print(duration);
            DEBUG_SERIAL.print("ms (");
            DEBUG_SERIAL.print(cfg->actuatorCount);
            DEBUG_SERIAL.println(" actuators)");

            ESP32_SERIAL.print("ACK:");
            ESP32_SERIAL.print(blockNum);
            ESP32_SERIAL.println(":UP");
          }
          // DOWN
          else if (strcmp(action, ACTION_DOWN) == 0) {
            digitalWrite(cfg->actuator1.downPin, RELAY_ON);
            digitalWrite(cfg->actuator2.downPin, RELAY_ON);
            digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
            digitalWrite(cfg->actuator2.upPin, RELAY_OFF);

            // Актуатор 3 (только блок 15)
            if (cfg->actuatorCount == 3) {
              digitalWrite(cfg->actuator3.downPin, RELAY_ON);
              digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
            }

            blockStates[idx].isActive = true;
            blockStates[idx].startTime = millis();
            blockStates[idx].duration = duration;

            DEBUG_SERIAL.print("[BLOCK ");
            DEBUG_SERIAL.print(blockNum);
            DEBUG_SERIAL.print("] DOWN ");
            DEBUG_SERIAL.print(duration);
            DEBUG_SERIAL.print("ms (");
            DEBUG_SERIAL.print(cfg->actuatorCount);
            DEBUG_SERIAL.println(" actuators)");

            ESP32_SERIAL.print("ACK:");
            ESP32_SERIAL.print(blockNum);
            ESP32_SERIAL.println(":DOWN");
          }
          // STOP
          else if (strcmp(action, ACTION_STOP) == 0) {
            digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
            digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
            digitalWrite(cfg->actuator2.upPin, RELAY_OFF);
            digitalWrite(cfg->actuator2.downPin, RELAY_OFF);

            if (cfg->actuatorCount == 3) {
              digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
              digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
            }

            blockStates[idx].isActive = false;

            DEBUG_SERIAL.print("[BLOCK ");
            DEBUG_SERIAL.print(blockNum);
            DEBUG_SERIAL.println("] STOP");

            ESP32_SERIAL.print("ACK:");
            ESP32_SERIAL.print(blockNum);
            ESP32_SERIAL.println(":STOP");
          }
          else {
            ESP32_SERIAL.println("ERROR:Invalid action");
          }
        }
        else {
          ESP32_SERIAL.println("ERROR:Invalid block");
        }
      }
      else {
        ESP32_SERIAL.println("ERROR:Unknown command");
      }
    }
  }

  // ===== ПРОВЕРКА ТАЙМАУТОВ =====
  unsigned long now = millis();

  for (int i = 1; i <= MEGA2_BLOCK_COUNT; i++) {
    if (blockStates[i].isActive) {
      unsigned long elapsed = now - blockStates[i].startTime;

      if (elapsed >= blockStates[i].duration) {
        int bNum = MEGA2_BLOCK_START + i - 1;
        const BlockConfig* cfg = getBlockConfig(bNum);

        // Остановить блок
        digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
        digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
        digitalWrite(cfg->actuator2.upPin, RELAY_OFF);
        digitalWrite(cfg->actuator2.downPin, RELAY_OFF);

        if (cfg->actuatorCount == 3) {
          digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
          digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
        }

        blockStates[i].isActive = false;

        DEBUG_SERIAL.print("[TIMEOUT] Block ");
        DEBUG_SERIAL.println(bNum);

        ESP32_SERIAL.print("DONE:");
        ESP32_SERIAL.println(bNum);
      }
    }
  }
}
