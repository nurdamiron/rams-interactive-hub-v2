/**
 * RAMS ACTUATOR CONFIGURATION (v2 / KRUG2)
 *
 * ОБЩИЙ КОНФИГУРАЦИОННЫЙ ФАЙЛ для ESP32 и Arduino Mega
 * Содержит маппинг всех блоков актуаторов на пины
 *
 * ВАЖНО: Этот файл используется ОДНОВРЕМЕННО в:
 * - ESP32 (для роутинга команд на правильную Mega)
 * - Arduino Mega #1 (для управления блоками 1-7)
 * - Arduino Mega #2 (для управления блоками 8-13)
 *
 * 13 зон круга KRUG2, 27 актуаторов (см. docs/V2-SETUP/README.md §3)
 *
 * @version 4.0
 * @date 2026-06-07
 * @author RAMS Global Team
 */

#ifndef ACTUATOR_CONFIG_H
#define ACTUATOR_CONFIG_H

#include <Arduino.h>

// ============================================================================
// ОБЩИЕ КОНСТАНТЫ СИСТЕМЫ
// ============================================================================

// Общее количество блоков в системе
#define TOTAL_BLOCKS        13

// Разделение блоков по Mega платам
#define MEGA1_BLOCK_START   1
#define MEGA1_BLOCK_END     7
#define MEGA1_BLOCK_COUNT   7

#define MEGA2_BLOCK_START   8
#define MEGA2_BLOCK_END     13
#define MEGA2_BLOCK_COUNT   6

// Логика реле (КРИТИЧНО!)
#define RELAY_ON            LOW   // Инверсная логика: LOW = включено
#define RELAY_OFF           HIGH  // HIGH = выключено

// Длительность по умолчанию
#define DEFAULT_DURATION_MS 6000  // 6 секунд

// Максимальное количество одновременно активных блоков
#define MAX_ACTIVE_BLOCKS   2

// ============================================================================
// ПРОТОКОЛ СВЯЗИ (ТЕКСТОВЫЙ - БЫСТРЫЙ)
// ============================================================================

// Baud rate для Serial связи
#define SERIAL_BAUD         115200

// Разделитель в протоколе
#define PROTOCOL_DELIMITER  ':'

// Команды
#define CMD_BLOCK           "BLOCK"   // BLOCK:5:UP:10000
#define CMD_ALL_STOP        "ALL"     // ALL:STOP
#define CMD_PING            "PING"    // PING
#define CMD_PONG            "PONG"    // PONG

// Действия
#define ACTION_UP           "UP"
#define ACTION_DOWN         "DOWN"
#define ACTION_STOP         "STOP"

// Ответы
#define RESPONSE_ACK        "ACK"     // ACK:5:UP
#define RESPONSE_DONE       "DONE"    // DONE:5
#define RESPONSE_ERROR      "ERROR"   // ERROR:message

// Heartbeat интервал (мс)
#define HEARTBEAT_INTERVAL  2000      // 2 секунды

// ============================================================================
// СТРУКТУРА ДАННЫХ АКТУАТОРА
// ============================================================================

/**
 * Структура для хранения пинов одного актуатора
 * Каждый актуатор управляется H-Bridge через 2 пина
 */
struct ActuatorPins {
  uint8_t upPin;      // Пин для движения ВВЕРХ
  uint8_t downPin;    // Пин для движения ВНИЗ
};

/**
 * Структура для хранения конфигурации блока
 * Каждый блок имеет 2 актуатора (кроме блока 7 PARK HOUSE MASLAK, у него 3)
 */
struct BlockConfig {
  uint8_t blockNum;           // Номер блока (1-13)
  uint8_t megaNum;            // Номер Mega платы (1 или 2)
  ActuatorPins actuator1;     // Актуатор 1 (UP и DOWN пины)
  ActuatorPins actuator2;     // Актуатор 2 (UP и DOWN пины)
  ActuatorPins actuator3;     // Актуатор 3 (только для блока 7)
  uint8_t actuatorCount;      // Количество актуаторов (обычно 2, для блока 7 = 3)
};

// ============================================================================
// МАППИНГ БЛОКОВ НА ПИНЫ (ЕДИНСТВЕННЫЙ ИСТОЧНИК ИСТИНЫ)
// ============================================================================

/**
 * ВНИМАНИЕ: ЭТО ЕДИНСТВЕННОЕ МЕСТО ГДЕ ОПРЕДЕЛЯЕТСЯ МАППИНГ!
 *
 * Логика пинов:
 * - Каждый блок = 2 актуатора (кроме блока 7 = 3)
 * - Каждый актуатор = 2 пина (UP и DOWN)
 * - Стандартный блок = 4 пина
 * - Блок 7 (PARK HOUSE MASLAK) = 6 пинов (3 актуатора)
 *
 * РАСПИНОВКА (чистая, последовательно с пина 22):
 * - Mega #1: блоки 1-7, пины 22-51 (15 актуаторов)
 * - Mega #2: блоки 8-13, пины 22-45 (12 актуаторов)
 */
const BlockConfig BLOCK_CONFIGS[TOTAL_BLOCKS] = {
  // ===== MEGA #1 - Блоки 1-7 =====
  {
    .blockNum = 1,            // NOMAD
    .megaNum = 1,
    .actuator1 = {46, 47},
    .actuator2 = {49, 48},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 2,            // GRANDE VIE
    .megaNum = 1,
    .actuator1 = {27, 26},
    .actuator2 = {29, 28},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 3,            // KERUEN CITY
    .megaNum = 1,
    .actuator1 = {23, 22},
    .actuator2 = {25, 24},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 4,            // RAMS GARDEN BAHCELIEVLER
    .megaNum = 1,
    .actuator1 = {35, 34},
    .actuator2 = {37, 36},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 5,            // RAMS RESORT BODRUM
    .megaNum = 1,
    .actuator1 = {31, 30},
    .actuator2 = {33, 32},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 6,            // RAMS CITY HALIC 2
    .megaNum = 1,
    .actuator1 = {43, 42},
    .actuator2 = {45, 44},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 7,            // PARK HOUSE MASLAK
    .megaNum = 1,
    .actuator1 = {39, 38},
    .actuator2 = {41, 40},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },

  // ===== MEGA #2 - Блоки 8-13 =====
  {
    .blockNum = 8,            // SAKURA
    .megaNum = 2,
    .actuator1 = {47, 46},
    .actuator2 = {49, 48},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 9,            // RAMS CITY HALIC 1
    .megaNum = 2,
    .actuator1 = {43, 42},
    .actuator2 = {45, 44},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 10,           // RAMS CITY GAZIANTEP
    .megaNum = 2,
    .actuator1 = {35, 34},
    .actuator2 = {37, 36},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 11,           // BAITEREK SCHOOL
    .megaNum = 2,
    .actuator1 = {39, 38},
    .actuator2 = {41, 40},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  },
  {
    .blockNum = 12,           // HYATT REGENCY (У него 3 физических актуатора)
    .megaNum = 2,
    .actuator1 = {23, 22},
    .actuator2 = {26, 27},
    .actuator3 = {25, 24},
    .actuatorCount = 3
  },
  {
    .blockNum = 13,           // RAMS CITY ALMATY
    .megaNum = 2,
    .actuator1 = {31, 30},
    .actuator2 = {33, 32},
    .actuator3 = {0, 0},
    .actuatorCount = 2
  }
};

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================

/**
 * Получить конфигурацию блока по его номеру
 * @param blockNum Номер блока (1-13)
 * @return Указатель на BlockConfig или nullptr если блок не найден
 */
inline const BlockConfig* getBlockConfig(uint8_t blockNum) {
  if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
    return nullptr;
  }
  return &BLOCK_CONFIGS[blockNum - 1];
}

/**
 * Проверить принадлежность блока к конкретной Mega
 * @param blockNum Номер блока (1-13)
 * @param megaNum Номер Mega (1 или 2)
 * @return true если блок принадлежит указанной Mega
 */
inline bool isBlockOnMega(uint8_t blockNum, uint8_t megaNum) {
  const BlockConfig* config = getBlockConfig(blockNum);
  if (config == nullptr) return false;
  return config->megaNum == megaNum;
}

/**
 * Получить количество актуаторов для блока
 * @param blockNum Номер блока (1-13)
 * @return Количество актуаторов (обычно 2, для блока 7 = 3)
 */
inline uint8_t getBlockActuatorCount(uint8_t blockNum) {
  const BlockConfig* config = getBlockConfig(blockNum);
  if (config == nullptr) return 0;
  return config->actuatorCount;
}

/**
 * Проверить валидность номера блока
 * @param blockNum Номер блока
 * @return true если номер блока валиден (1-13)
 */
inline bool isValidBlockNum(uint8_t blockNum) {
  return (blockNum >= 1 && blockNum <= TOTAL_BLOCKS);
}

/**
 * Проверить валидность действия
 * @param action Строка с действием
 * @return true если действие валидно (UP, DOWN, STOP)
 */
inline bool isValidAction(const char* action) {
  return (strcmp(action, ACTION_UP) == 0 ||
          strcmp(action, ACTION_DOWN) == 0 ||
          strcmp(action, ACTION_STOP) == 0);
}

// ============================================================================
// DEBUG ФУНКЦИИ
// ============================================================================

/**
 * Вывести конфигурацию всех блоков в Serial
 * Полезно для отладки и проверки маппинга
 */
inline void printAllBlockConfigs() {
  Serial.println("\n========================================");
  Serial.println("  ACTUATOR BLOCKS CONFIGURATION");
  Serial.println("========================================");
  Serial.print("Total blocks: ");
  Serial.println(TOTAL_BLOCKS);
  Serial.print("Mega #1: blocks ");
  Serial.print(MEGA1_BLOCK_START);
  Serial.print("-");
  Serial.println(MEGA1_BLOCK_END);
  Serial.print("Mega #2: blocks ");
  Serial.print(MEGA2_BLOCK_START);
  Serial.print("-");
  Serial.println(MEGA2_BLOCK_END);
  Serial.print("Relay logic: ");
  Serial.print(RELAY_ON == LOW ? "INVERSE (LOW=ON)" : "DIRECT (HIGH=ON)");
  Serial.println("\n");

  for (int i = 0; i < TOTAL_BLOCKS; i++) {
    const BlockConfig* cfg = &BLOCK_CONFIGS[i];
    Serial.print("Block ");
    Serial.print(cfg->blockNum);
    Serial.print(" (Mega #");
    Serial.print(cfg->megaNum);
    Serial.print("):");

    Serial.print(" Act1[");
    Serial.print(cfg->actuator1.upPin);
    Serial.print(",");
    Serial.print(cfg->actuator1.downPin);
    Serial.print("]");

    Serial.print(" Act2[");
    Serial.print(cfg->actuator2.upPin);
    Serial.print(",");
    Serial.print(cfg->actuator2.downPin);
    Serial.print("]");

    if (cfg->actuatorCount == 3) {
      Serial.print(" Act3[");
      Serial.print(cfg->actuator3.upPin);
      Serial.print(",");
      Serial.print(cfg->actuator3.downPin);
      Serial.print("]");
    }

    Serial.println();
  }

  Serial.println("========================================\n");
}

/**
 * Вывести информацию о конкретном блоке
 * @param blockNum Номер блока (1-13)
 */
inline void printBlockConfig(uint8_t blockNum) {
  const BlockConfig* cfg = getBlockConfig(blockNum);
  if (cfg == nullptr) {
    Serial.print("ERROR: Invalid block number ");
    Serial.println(blockNum);
    return;
  }

  Serial.print("Block ");
  Serial.print(cfg->blockNum);
  Serial.print(" (Mega #");
  Serial.print(cfg->megaNum);
  Serial.print(", ");
  Serial.print(cfg->actuatorCount);
  Serial.println(" actuators):");

  Serial.print("  Actuator 1: UP=");
  Serial.print(cfg->actuator1.upPin);
  Serial.print(", DOWN=");
  Serial.println(cfg->actuator1.downPin);

  Serial.print("  Actuator 2: UP=");
  Serial.print(cfg->actuator2.upPin);
  Serial.print(", DOWN=");
  Serial.println(cfg->actuator2.downPin);

  if (cfg->actuatorCount == 3) {
    Serial.print("  Actuator 3: UP=");
    Serial.print(cfg->actuator3.upPin);
    Serial.print(", DOWN=");
    Serial.println(cfg->actuator3.downPin);
  }
}

#endif // ACTUATOR_CONFIG_H
