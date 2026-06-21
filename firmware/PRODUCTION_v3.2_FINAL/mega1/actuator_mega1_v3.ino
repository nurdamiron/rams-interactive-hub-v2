#include <Arduino.h>
#include "ACTUATOR_CONFIG.h"

// Переопределяем логику реле для Mega 1 (здесь инверсная логика: LOW = ON, HIGH = OFF)
#undef RELAY_ON
#undef RELAY_OFF
#define RELAY_ON            LOW
#define RELAY_OFF           HIGH

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

// Состояния блоков 1-7 (индекс 0 не используется)
BlockState blockStates[MEGA1_BLOCK_COUNT + 1];

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
  DEBUG_SERIAL.println("  RAMS ACTUATOR CONTROLLER - MEGA #1");
  DEBUG_SERIAL.println("  Blocks 1-7 | DroneControl Style");
  DEBUG_SERIAL.println("========================================");

  // Инициализация всех пинов
  for (int i = MEGA1_BLOCK_START; i <= MEGA1_BLOCK_END; i++) {
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

    if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
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
    if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
      DEBUG_SERIAL.print("] [");
      DEBUG_SERIAL.print(cfg->actuator3.upPin);
      DEBUG_SERIAL.print(",");
      DEBUG_SERIAL.print(cfg->actuator3.downPin);
    }
    DEBUG_SERIAL.println("]");
  }

  // Инициализация состояний
  for (int i = 0; i <= MEGA1_BLOCK_COUNT; i++) {
    blockStates[i].isActive = false;
    blockStates[i].startTime = 0;
    blockStates[i].duration = 0;
  }

  // ESP32 serial
  ESP32_SERIAL.begin(SERIAL_BAUD);
  // Устанавливаем таймаут 50мс чтобы readStringUntil не блокировал цикл loop
  ESP32_SERIAL.setTimeout(50);

  DEBUG_SERIAL.println("[READY] System initialized!\n");
}

// ============================================================================
// MAIN LOOP - СТИЛЬ DroneControl.ino
// ============================================================================

void loop() {
  // ===== ЧТЕНИЕ КОМАНД ОТ ESP32 или USB =====
  bool hasCommand = false;
  if (ESP32_SERIAL.available()) {
    cmdRaw = ESP32_SERIAL.readStringUntil('\n');
    cmdRaw.trim();
    if (cmdRaw.length() > 0) {
      hasCommand = true;
    }
  }
  
  if (!hasCommand && DEBUG_SERIAL.available()) {
    cmdRaw = DEBUG_SERIAL.readStringUntil('\n');
    cmdRaw.trim();
    if (cmdRaw.length() > 0) {
      hasCommand = true;
    }
  }

  if (hasCommand) {
    DEBUG_SERIAL.print("[RX] ");
    DEBUG_SERIAL.println(cmdRaw);

      // PING
      if (cmdRaw == CMD_PING) {
        ESP32_SERIAL.println(CMD_PONG);
        DEBUG_SERIAL.println("[TX] PONG");
      }
      // ON:pin
      else if (cmdRaw.startsWith("ON:")) {
        int pin = cmdRaw.substring(3).toInt();
        if (pin >= 22 && pin <= 53) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, RELAY_ON);
          DEBUG_SERIAL.print("[PIN ON] ");
          DEBUG_SERIAL.println(pin);
          ESP32_SERIAL.print("ACK:PIN:");
          ESP32_SERIAL.print(pin);
          ESP32_SERIAL.println(":ON");
        }
      }
      // OFF:pin
      else if (cmdRaw.startsWith("OFF:")) {
        int pin = cmdRaw.substring(4).toInt();
        if (pin >= 22 && pin <= 53) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, RELAY_OFF);
          DEBUG_SERIAL.print("[PIN OFF] ");
          DEBUG_SERIAL.println(pin);
          ESP32_SERIAL.print("ACK:PIN:");
          ESP32_SERIAL.print(pin);
          ESP32_SERIAL.println(":OFF");
        }
      }
      // PIN:pin:action:duration
      else if (cmdRaw.startsWith("PIN:")) {
        int firstColon = cmdRaw.indexOf(':');
        int secondColon = cmdRaw.indexOf(':', firstColon + 1);
        int thirdColon = cmdRaw.indexOf(':', secondColon + 1);
        if (firstColon != -1 && secondColon != -1) {
          int pin = cmdRaw.substring(firstColon + 1, secondColon).toInt();
          String actionStr = (thirdColon != -1) ? cmdRaw.substring(secondColon + 1, thirdColon) : cmdRaw.substring(secondColon + 1);
          int dur = (thirdColon != -1) ? cmdRaw.substring(thirdColon + 1).toInt() : 0;
          if (pin >= 22 && pin <= 53) {
            if (actionStr == "ON") {
              pinMode(pin, OUTPUT);
              digitalWrite(pin, RELAY_ON);
              DEBUG_SERIAL.print("[PIN ON] ");
              DEBUG_SERIAL.print(pin);
              if (dur > 0) {
                DEBUG_SERIAL.print(" for ");
                DEBUG_SERIAL.print(dur);
                DEBUG_SERIAL.println("ms");
                delay(dur);
                digitalWrite(pin, RELAY_OFF);
                DEBUG_SERIAL.print("[PIN OFF (TIMEOUT)] ");
                DEBUG_SERIAL.println(pin);
              } else {
                DEBUG_SERIAL.println();
              }
              ESP32_SERIAL.print("ACK:PIN:");
              ESP32_SERIAL.print(pin);
              ESP32_SERIAL.println(":ON");
            } else if (actionStr == "OFF") {
              pinMode(pin, OUTPUT);
              digitalWrite(pin, RELAY_OFF);
              DEBUG_SERIAL.print("[PIN OFF] ");
              DEBUG_SERIAL.println(pin);
              ESP32_SERIAL.print("ACK:PIN:");
              ESP32_SERIAL.print(pin);
              ESP32_SERIAL.println(":OFF");
            }
          }
        }
      }
      // STOP (all off)
      else if (cmdRaw == "STOP") {
        DEBUG_SERIAL.println("[DIRECT] STOP ALL PINS");
        for (int pin = 22; pin <= 53; pin++) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, RELAY_OFF);
        }
        ESP32_SERIAL.println("ACK:STOP");
      }
      // ALL:STOP
      else if (cmdRaw.startsWith(CMD_ALL_STOP)) {
        DEBUG_SERIAL.println("[ALL] STOP ALL");

        for (int i = MEGA1_BLOCK_START; i <= MEGA1_BLOCK_END; i++) {
          const BlockConfig* cfg = getBlockConfig(i);
          if (cfg == nullptr) continue;

          digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
          digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
          digitalWrite(cfg->actuator2.upPin, RELAY_OFF);
          digitalWrite(cfg->actuator2.downPin, RELAY_OFF);
          if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
            digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
            digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
          }

          int idx = i - MEGA1_BLOCK_START + 1;
          blockStates[idx].isActive = false;
        }

        ESP32_SERIAL.println("ACK:0:STOP");
      }
      // ACTUATOR:blockNum:actNum:action:duration
      else if (cmdRaw.startsWith("ACTUATOR")) {
        int block = 0;
        int act = 0;
        char actAction[10] = {0};
        int dur = 0;

        int parsed = sscanf(cmdRaw.c_str(), "ACTUATOR:%d:%d:%[^:]:%d", &block, &act, actAction, &dur);
        if (parsed >= 3 && block >= MEGA1_BLOCK_START && block <= MEGA1_BLOCK_END) {
          const BlockConfig* cfg = getBlockConfig(block);
          int idx = block - MEGA1_BLOCK_START + 1;
          if (dur == 0) {
            if (strcmp(actAction, ACTION_UP) == 0) dur = 4000;
            else if (strcmp(actAction, ACTION_DOWN) == 0) dur = 5000;
            else dur = DEFAULT_DURATION_MS;
          }

          // Определяем H-Bridge
          ActuatorPins* pins = nullptr;
          if (act == 1) pins = (ActuatorPins*)&cfg->actuator1;
          else if (act == 2) pins = (ActuatorPins*)&cfg->actuator2;
          else if (act == 3 && cfg->actuatorCount >= 3) pins = (ActuatorPins*)&cfg->actuator3;

          if (pins != nullptr && pins->upPin != 0) {
            if (strcmp(actAction, ACTION_UP) == 0) {
              digitalWrite(pins->upPin, RELAY_ON);
              digitalWrite(pins->downPin, RELAY_OFF);
              blockStates[idx].isActive = true;
              blockStates[idx].startTime = millis();
              blockStates[idx].duration = dur;
              DEBUG_SERIAL.print("[ACTUATOR ");
              DEBUG_SERIAL.print(block);
              DEBUG_SERIAL.print(" ACT ");
              DEBUG_SERIAL.print(act);
              DEBUG_SERIAL.print("] UP ");
              DEBUG_SERIAL.println(dur);
              ESP32_SERIAL.print("ACK:");
              ESP32_SERIAL.print(block);
              ESP32_SERIAL.print(":");
              ESP32_SERIAL.print(act);
              ESP32_SERIAL.println(":UP");
            }
            else if (strcmp(actAction, ACTION_DOWN) == 0) {
              digitalWrite(pins->downPin, RELAY_ON);
              digitalWrite(pins->upPin, RELAY_OFF);
              blockStates[idx].isActive = true;
              blockStates[idx].startTime = millis();
              blockStates[idx].duration = dur;
              DEBUG_SERIAL.print("[ACTUATOR ");
              DEBUG_SERIAL.print(block);
              DEBUG_SERIAL.print(" ACT ");
              DEBUG_SERIAL.print(act);
              DEBUG_SERIAL.print("] DOWN ");
              DEBUG_SERIAL.println(dur);
              ESP32_SERIAL.print("ACK:");
              ESP32_SERIAL.print(block);
              ESP32_SERIAL.print(":");
              ESP32_SERIAL.print(act);
              ESP32_SERIAL.println(":DOWN");
            }
            else if (strcmp(actAction, ACTION_STOP) == 0) {
              digitalWrite(pins->upPin, RELAY_OFF);
              digitalWrite(pins->downPin, RELAY_OFF);
              DEBUG_SERIAL.print("[ACTUATOR ");
              DEBUG_SERIAL.print(block);
              DEBUG_SERIAL.print(" ACT ");
              DEBUG_SERIAL.print(act);
              DEBUG_SERIAL.println("] STOP");
              ESP32_SERIAL.print("ACK:");
              ESP32_SERIAL.print(block);
              ESP32_SERIAL.print(":");
              ESP32_SERIAL.print(act);
              ESP32_SERIAL.println(":STOP");
            }
          }
        }
      }
      // BLOCK:N:ACTION:DURATION
      else if (cmdRaw.startsWith(CMD_BLOCK)) {
        blockNum = 0;
        duration = 0;
        memset(action, 0, sizeof(action));

        // Парсинг как в DroneControl.ino
        int parsed = sscanf(cmdRaw.c_str(), "BLOCK:%d:%[^:]:%d", &blockNum, action, &duration);

        if (parsed >= 2 && blockNum >= MEGA1_BLOCK_START && blockNum <= MEGA1_BLOCK_END) {
          const BlockConfig* cfg = getBlockConfig(blockNum);
          int idx = blockNum - MEGA1_BLOCK_START + 1;

          if (duration == 0) {
            if (strcmp(action, ACTION_UP) == 0) duration = 4000;
            else if (strcmp(action, ACTION_DOWN) == 0) duration = 5000;
            else duration = DEFAULT_DURATION_MS;
          }

          // UP
          if (strcmp(action, ACTION_UP) == 0) {
            digitalWrite(cfg->actuator1.upPin, RELAY_ON);
            digitalWrite(cfg->actuator2.upPin, RELAY_ON);
            digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
            digitalWrite(cfg->actuator2.downPin, RELAY_OFF);
            if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
              digitalWrite(cfg->actuator3.upPin, RELAY_ON);
              digitalWrite(cfg->actuator3.downPin, RELAY_OFF);
            }

            blockStates[idx].isActive = true;
            blockStates[idx].startTime = millis();
            blockStates[idx].duration = duration;

            DEBUG_SERIAL.print("[BLOCK ");
            DEBUG_SERIAL.print(blockNum);
            DEBUG_SERIAL.print("] UP ");
            DEBUG_SERIAL.println(duration);

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
            if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
              digitalWrite(cfg->actuator3.downPin, RELAY_ON);
              digitalWrite(cfg->actuator3.upPin, RELAY_OFF);
            }

            blockStates[idx].isActive = true;
            blockStates[idx].startTime = millis();
            blockStates[idx].duration = duration;

            DEBUG_SERIAL.print("[BLOCK ");
            DEBUG_SERIAL.print(blockNum);
            DEBUG_SERIAL.print("] DOWN ");
            DEBUG_SERIAL.println(duration);

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
            if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
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

  // ===== ПРОВЕРКА ТАЙМАУТОВ =====
  unsigned long now = millis();

  for (int i = 1; i <= MEGA1_BLOCK_COUNT; i++) {
    if (blockStates[i].isActive) {
      unsigned long elapsed = now - blockStates[i].startTime;

      if (elapsed >= (unsigned long)blockStates[i].duration) {
        int bNum = MEGA1_BLOCK_START + i - 1;
        const BlockConfig* cfg = getBlockConfig(bNum);

        // Остановить блок
        digitalWrite(cfg->actuator1.upPin, RELAY_OFF);
        digitalWrite(cfg->actuator1.downPin, RELAY_OFF);
        digitalWrite(cfg->actuator2.upPin, RELAY_OFF);
        digitalWrite(cfg->actuator2.downPin, RELAY_OFF);
        if (cfg->actuatorCount >= 3 && cfg->actuator3.upPin != 0) {
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
