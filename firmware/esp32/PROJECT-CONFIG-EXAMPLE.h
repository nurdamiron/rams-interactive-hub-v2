/**
 * RAMS Interactive Hub - Project Configuration Example
 *
 * Этот файл показывает как настроить маппинг проектов на физический хардвер.
 * Скопируйте этот файл и адаптируйте под свою установку.
 *
 * ВАЖНО для 12V WS2812B:
 * - 1 адрес = 3 физических LED
 * - NUM_LEDS = количество адресов (не физических LED!)
 * - Физические LED = NUM_LEDS × 3
 */

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// ============================================================================
// КОНФИГУРАЦИЯ LED
// ============================================================================

// Для 12V WS2812B (WS2811)
#define LED_PIN           16        // GPIO для Data line
#define NUM_LEDS          200       // АДРЕСА! (600 физических LED ÷ 3)
#define LED_TYPE          WS2811    // Для 12V используем WS2811
#define COLOR_ORDER       RGB       // Или GRB - зависит от производителя
#define BRIGHTNESS        180       // Яркость по умолчанию (0-255)
#define FRAMES_PER_SECOND 120

// ============================================================================
// КОНФИГУРАЦИЯ GPIO
// ============================================================================

// Реле (активные LOW)
#define RELAY_1_PIN       17        // Основное освещение / элемент 1
#define RELAY_2_PIN       18        // Дополнительное освещение / элемент 2
#define RELAY_3_PIN       19        // Вентилятор / элемент 3
#define RELAY_4_PIN       23        // Резерв (если есть 4-й канал)

// Актуаторы (через L298N H-Bridge)
#define ACTUATOR_1_UP     21        // Актуатор 1 - вперед
#define ACTUATOR_1_DOWN   22        // Актуатор 1 - назад
#define ACTUATOR_2_UP     25        // Актуатор 2 - вперед (опционально)
#define ACTUATOR_2_DOWN   26        // Актуатор 2 - назад (опционально)

// Система
#define STATUS_LED_PIN    2         // Встроенный LED ESP32
#define BUTTON_PIN        0         // Кнопка для тестирования

// ============================================================================
// МАППИНГ ПРОЕКТОВ НА ХАРДВЕР
// ============================================================================

/**
 * Структура конфигурации проекта:
 * - id: ID проекта (соответствует hardwareId в projects.ts)
 * - name: Название для логов
 * - ledStart: Начальный АДРЕС (не физический LED!)
 * - ledCount: Количество АДРЕСОВ (физические LED = count × 3)
 * - color: Цвет по умолчанию (RGB)
 * - effect: Эффект по умолчанию
 * - relay: Номер реле (0 = нет, 1-4)
 * - actuator: Номер актуатора (0 = нет, 1-2)
 */

struct ProjectConfig {
  int id;
  const char* name;
  int ledStart;
  int ledCount;
  uint32_t color;         // RGB color (0xRRGGBB)
  const char* effect;
  int relay;              // 0 = none, 1-4 = relay number
  int actuator;           // 0 = none, 1-2 = actuator number
};

// ============================================================================
// ПРИМЕР КОНФИГУРАЦИИ ДЛЯ МАКЕТА С 28 ПРОЕКТАМИ
// ============================================================================

const ProjectConfig PROJECT_CONFIGS[] = {

  // ──────────────────────────────────────────────────────────────────────
  // ПРОЕКТЫ В ПРОДАЖЕ / ФЛАГМАНЫ (яркие цвета, эффекты)
  // ──────────────────────────────────────────────────────────────────────

  // ID 0: RAMS BEYOND - Flagship проект
  {
    .id = 0,
    .name = "RAMS BEYOND",
    .ledStart = 0,          // Адреса 0-19
    .ledCount = 20,         // 20 адресов = 60 физических LED
    .color = 0xFFD700,      // Gold
    .effect = "pulse",      // Пульсация для привлечения внимания
    .relay = 1,             // Дополнительная подсветка через реле #1
    .actuator = 0           // Без актуатора
  },

  // ID 1: RAMS CITY KAZAKHSTAN
  {
    .id = 1,
    .name = "RAMS CITY",
    .ledStart = 20,         // Адреса 20-29
    .ledCount = 10,         // 10 адресов = 30 LED
    .color = 0xFFFFFF,      // White
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 2: NOMAD
  {
    .id = 2,
    .name = "NOMAD",
    .ledStart = 30,         // Адреса 30-39
    .ledCount = 10,         // 30 LED
    .color = 0x00A8E8,      // Sky Blue
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 3: NOMAD 2
  {
    .id = 3,
    .name = "NOMAD 2",
    .ledStart = 40,         // Адреса 40-49
    .ledCount = 10,         // 30 LED
    .color = 0x007EA7,      // Ocean Blue
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 5: ДОМ НА АБАЯ
  {
    .id = 5,
    .name = "DOM NA ABAYA",
    .ledStart = 50,         // Адреса 50-59
    .ledCount = 10,         // 30 LED
    .color = 0xC9A961,      // Business Gold
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 8: ВОСТОЧНЫЙ ПАРК
  {
    .id = 8,
    .name = "VOSTOCHNY PARK",
    .ledStart = 60,         // Адреса 60-69
    .ledCount = 10,         // 30 LED
    .color = 0x2ECC71,      // Park Green
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 9: RAMS GARDEN ALMATY - Премиум проект
  {
    .id = 9,
    .name = "RAMS GARDEN",
    .ledStart = 70,         // Адреса 70-99
    .ledCount = 30,         // 30 адресов = 90 LED
    .color = 0x27AE60,      // Garden Green
    .effect = "pulse",
    .relay = 2,             // Ландшафтное освещение через реле #2
    .actuator = 1           // Подъем элемента ландшафта
  },

  // ID 11: RAMS SIGNATURE - Flagship
  {
    .id = 11,
    .name = "RAMS SIGNATURE",
    .ledStart = 100,        // Адреса 100-124
    .ledCount = 25,         // 25 адресов = 75 LED
    .color = 0x3498DB,      // Signature Blue
    .effect = "pulse",
    .relay = 3,             // Фасадная подсветка
    .actuator = 2           // Открытие крыши
  },

  // ID 12: RAMS SAIAHAT
  {
    .id = 12,
    .name = "RAMS SAIAHAT",
    .ledStart = 125,        // Адреса 125-144
    .ledCount = 20,         // 60 LED
    .color = 0xE74C3C,      // Heritage Red
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 13: RAMS GARDEN ATYRAU
  {
    .id = 13,
    .name = "RAMS GARDEN ATYRAU",
    .ledStart = 145,        // Адреса 145-164
    .ledCount = 20,         // 60 LED
    .color = 0x1ABC9C,      // Turquoise
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ID 15: RAMS EVO
  {
    .id = 15,
    .name = "RAMS EVO",
    .ledStart = 165,        // Адреса 165-179
    .ledCount = 15,         // 45 LED
    .color = 0x9B59B6,      // Evolution Purple
    .effect = "fade",
    .relay = 0,
    .actuator = 0
  },

  // ──────────────────────────────────────────────────────────────────────
  // ЗАВЕРШЕННЫЕ ПРОЕКТЫ (более спокойные цвета)
  // ──────────────────────────────────────────────────────────────────────

  // ID 6: LATIFA RESIDENCE
  {
    .id = 6,
    .name = "LATIFA",
    .ledStart = 180,
    .ledCount = 8,          // 24 LED
    .color = 0xF39C12,      // Orange
    .effect = "none",       // Статичный для завершенных
    .relay = 0,
    .actuator = 0
  },

  // ID 7: IZUMRUD RESIDENCE
  {
    .id = 7,
    .name = "IZUMRUD",
    .ledStart = 188,
    .ledCount = 8,          // 24 LED
    .color = 0x27AE60,      // Emerald Green
    .effect = "none",
    .relay = 0,
    .actuator = 0
  },

  // ID 17: ORTAU
  {
    .id = 17,
    .name = "ORTAU",
    .ledStart = 196,
    .ledCount = 4,          // 12 LED (маленький)
    .color = 0x95A5A6,      // Gray
    .effect = "none",
    .relay = 0,
    .actuator = 0
  }

  // ... Добавьте остальные проекты по аналогии

};

// Общее количество настроенных проектов
#define NUM_CONFIGURED_PROJECTS (sizeof(PROJECT_CONFIGS) / sizeof(ProjectConfig))

// ============================================================================
// НАСТРОЙКИ АКТУАТОРОВ
// ============================================================================

struct ActuatorConfig {
  int id;
  const char* name;
  int pinUp;
  int pinDown;
  int moveDuration;       // Время движения в мс
  int pauseAfterMove;     // Пауза после движения (мс)
};

const ActuatorConfig ACTUATOR_CONFIGS[] = {
  {
    .id = 1,
    .name = "landscape",    // Ландшафт для RAMS GARDEN
    .pinUp = ACTUATOR_1_UP,
    .pinDown = ACTUATOR_1_DOWN,
    .moveDuration = 5000,   // 5 секунд
    .pauseAfterMove = 1000  // 1 секунда пауза
  },
  {
    .id = 2,
    .name = "roof",         // Крыша для RAMS SIGNATURE
    .pinUp = ACTUATOR_2_UP,
    .pinDown = ACTUATOR_2_DOWN,
    .moveDuration = 8000,   // 8 секунд (медленнее)
    .pauseAfterMove = 2000  // 2 секунды пауза
  }
};

#define NUM_ACTUATORS (sizeof(ACTUATOR_CONFIGS) / sizeof(ActuatorConfig))

// ============================================================================
// НАСТРОЙКИ РЕЛЕ
// ============================================================================

struct RelayConfig {
  int id;
  const char* name;
  int pin;
  bool activeLow;         // true если реле активно LOW
};

const RelayConfig RELAY_CONFIGS[] = {
  {
    .id = 1,
    .name = "main_light",
    .pin = RELAY_1_PIN,
    .activeLow = true       // Большинство реле активны LOW
  },
  {
    .id = 2,
    .name = "landscape_light",
    .pin = RELAY_2_PIN,
    .activeLow = true
  },
  {
    .id = 3,
    .name = "facade_light",
    .pin = RELAY_3_PIN,
    .activeLow = true
  }
};

#define NUM_RELAYS (sizeof(RELAY_CONFIGS) / sizeof(RelayConfig))

// ============================================================================
// ЦВЕТОВАЯ ПАЛИТРА
// ============================================================================

// Предустановленные цвета для быстрого доступа
#define COLOR_GOLD          0xFFD700
#define COLOR_WHITE         0xFFFFFF
#define COLOR_WARM_WHITE    0xFFF3E0
#define COLOR_BLUE          0x3498DB
#define COLOR_GREEN         0x27AE60
#define COLOR_RED           0xE74C3C
#define COLOR_PURPLE        0x9B59B6
#define COLOR_ORANGE        0xF39C12
#define COLOR_TURQUOISE     0x1ABC9C
#define COLOR_GRAY          0x95A5A6

// ============================================================================
// НАСТРОЙКИ ЭФФЕКТОВ
// ============================================================================

// Скорости эффектов
#define FADE_SPEED          10      // мс на шаг
#define PULSE_SPEED         15      // мс на шаг
#define RAINBOW_SPEED       5       // мс на шаг

// Параметры эффектов
#define PULSE_MIN_BRIGHTNESS  50    // Минимальная яркость для pulse
#define PULSE_MAX_BRIGHTNESS  255   // Максимальная яркость для pulse

// ============================================================================
// СЕТЕВЫЕ НАСТРОЙКИ
// ============================================================================

// WiFi
#define WIFI_SSID           "RAMS_Hub"
#define WIFI_PASSWORD       "RamsInteractive2026"

// Static IP
#define IP_ADDRESS          192, 168, 1, 100
#define GATEWAY             192, 168, 1, 1
#define SUBNET              255, 255, 255, 0

// ============================================================================
// ФУНКЦИИ ПОМОЩНИКИ
// ============================================================================

/**
 * Найти конфигурацию проекта по ID
 */
const ProjectConfig* getProjectConfig(int projectId) {
  for (int i = 0; i < NUM_CONFIGURED_PROJECTS; i++) {
    if (PROJECT_CONFIGS[i].id == projectId) {
      return &PROJECT_CONFIGS[i];
    }
  }
  return nullptr;  // Не найдено
}

/**
 * Конвертация hex цвета в CRGB
 */
CRGB hexToRGB(uint32_t hex) {
  return CRGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
}

/**
 * Вывод конфигурации в Serial
 */
void printConfiguration() {
  Serial.println("\n========================================");
  Serial.println("  PROJECT CONFIGURATION");
  Serial.println("========================================");
  Serial.print("Total Projects: ");
  Serial.println(NUM_CONFIGURED_PROJECTS);
  Serial.print("Total LEDs (addresses): ");
  Serial.println(NUM_LEDS);
  Serial.print("Physical LEDs: ");
  Serial.println(NUM_LEDS * 3);
  Serial.println("\nProject Mapping:");

  for (int i = 0; i < NUM_CONFIGURED_PROJECTS; i++) {
    const ProjectConfig* cfg = &PROJECT_CONFIGS[i];
    Serial.print("  [");
    Serial.print(cfg->id);
    Serial.print("] ");
    Serial.print(cfg->name);
    Serial.print(" | LED: ");
    Serial.print(cfg->ledStart);
    Serial.print("-");
    Serial.print(cfg->ledStart + cfg->ledCount - 1);
    Serial.print(" (");
    Serial.print(cfg->ledCount * 3);
    Serial.print(" LEDs)");
    if (cfg->relay > 0) {
      Serial.print(" | Relay #");
      Serial.print(cfg->relay);
    }
    if (cfg->actuator > 0) {
      Serial.print(" | Actuator #");
      Serial.print(cfg->actuator);
    }
    Serial.println();
  }

  Serial.println("========================================\n");
}

#endif // PROJECT_CONFIG_H

// ============================================================================
// КАК ИСПОЛЬЗОВАТЬ ЭТОТ ФАЙЛ
// ============================================================================

/**
 * 1. Скопировать этот файл в папку с прошивкой ESP32
 *
 * 2. В rams_controller.ino добавить:
 *    #include "PROJECT-CONFIG-EXAMPLE.h"
 *
 * 3. В setup() вызвать:
 *    printConfiguration();
 *
 * 4. Адаптировать PROJECT_CONFIGS под ваш макет:
 *    - Измерить количество LED на каждом проекте
 *    - Разделить на 3 для получения количества адресов
 *    - Назначить GPIO пины для реле и актуаторов
 *    - Выбрать цвета и эффекты
 *
 * 5. Загрузить на ESP32 и проверить через Serial Monitor
 *
 * 6. Тестировать каждый проект отдельно:
 *    curl -X POST http://192.168.1.100/command \
 *      -H "Content-Type: application/json" \
 *      -d '{"action":"highlight","projectId":9}'
 */
