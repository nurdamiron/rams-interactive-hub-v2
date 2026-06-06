/**
 * RAMS Screen - Zone-Based LED Configuration
 *
 * Архитектура для управления ЗОНАМИ вместо отдельных лент
 *
 * Геометрия инсталляции:
 * - 8 радиальных лучей
 * - 2 концентрических круга (внутренний, внешний)
 * - 31 актуатор в точках пересечения
 * - 15 логических зон (секторов между лучами и кругами)
 *
 * Физическое оборудование:
 * - 10 физических линий WS2811 (12V)
 * - Каждая линия на отдельном GPIO
 * - Каждая зона формируется из СЕГМЕНТОВ на разных линиях
 *
 * @version 2.0
 * @date 2026-02-04
 */

#ifndef ZONE_CONFIG_H
#define ZONE_CONFIG_H

#include <FastLED.h>

// ============================================================================
// КОНФИГУРАЦИЯ ФИЗИЧЕСКИХ ЛИНИЙ LED
// ============================================================================

// Количество физических линий (8 лучей + 2 круга)
#define NUM_PHYSICAL_LINES 10

// GPIO пины для каждой линии
const uint8_t LED_PINS[NUM_PHYSICAL_LINES] = {
  16,  // Line 0: Ray 1
  17,  // Line 1: Ray 2
  18,  // Line 2: Ray 3
  19,  // Line 3: Ray 4
  21,  // Line 4: Ray 5
  22,  // Line 5: Ray 6
  23,  // Line 6: Ray 7
  25,  // Line 7: Ray 8
  26,  // Line 8: Inner Circle
  27   // Line 9: Outer Circle
};

// Максимальное количество LED на линию (для массивов)
#define MAX_LEDS_PER_LINE 200

// Фактическое количество LED на каждой линии (TODO: измерить реальное количество)
const uint16_t LEDS_PER_LINE[NUM_PHYSICAL_LINES] = {
  100,  // Ray 1 - TODO: изменить на реальное значение
  100,  // Ray 2
  100,  // Ray 3
  100,  // Ray 4
  100,  // Ray 5
  100,  // Ray 6
  100,  // Ray 7
  100,  // Ray 8
  150,  // Inner Circle (больше, т.к. по окружности)
  200   // Outer Circle (еще больше)
};

// Тип светодиодов и порядок цветов
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS 180
#define FRAMES_PER_SECOND 120

// ============================================================================
// СТРУКТУРА СЕГМЕНТА
// ============================================================================

/**
 * Сегмент - это участок на одной физической линии LED
 * Несколько сегментов образуют Зону
 */
struct LEDSegment {
  uint8_t lineId;        // ID физической линии (0-9)
  uint16_t startPixel;   // Начальный пиксель на этой линии
  uint16_t endPixel;     // Конечный пиксель (включительно)

  // Конструктор для удобства
  LEDSegment(uint8_t line, uint16_t start, uint16_t end)
    : lineId(line), startPixel(start), endPixel(end) {}

  // Количество пикселей в сегменте
  uint16_t length() const {
    return endPixel - startPixel + 1;
  }
};

// ============================================================================
// СТРУКТУРА ЗОНЫ
// ============================================================================

/**
 * Зона - это логическая область инсталляции
 * Состоит из нескольких сегментов на разных физических линиях
 */
struct Zone {
  uint8_t id;                    // ID зоны (0-14)
  const char* name;              // Имя для отладки
  LEDSegment* segments;          // Массив сегментов
  uint8_t segmentCount;          // Количество сегментов

  // Состояние зоны
  CRGB currentColor;             // Текущий цвет
  uint8_t brightness;            // Яркость (0-255)
  bool isActive;                 // Включена ли зона
  String effect;                 // Текущий эффект

  // Конструктор
  Zone(uint8_t zoneId, const char* zoneName, LEDSegment* segs, uint8_t count)
    : id(zoneId), name(zoneName), segments(segs), segmentCount(count),
      currentColor(CRGB::Black), brightness(BRIGHTNESS), isActive(false), effect("none") {}

  // Общее количество пикселей во всех сегментах зоны
  uint16_t totalPixels() const {
    uint16_t total = 0;
    for (uint8_t i = 0; i < segmentCount; i++) {
      total += segments[i].length();
    }
    return total;
  }
};

// ============================================================================
// ОПРЕДЕЛЕНИЕ ЗОН (ПРИМЕР)
// ============================================================================

/**
 * ВАЖНО: Эти значения - ЗАГЛУШКИ для демонстрации архитектуры
 * После инвентаризации лент нужно будет заполнить реальными данными
 *
 * Логика нумерации зон:
 * - Зоны 0-7: секторы между лучами (по часовой стрелке)
 * - Зоны 8-14: внутренние сегменты или специальные области
 */

// Пример: Зона 0 - сектор между Ray 1 и Ray 2
LEDSegment zone0_segments[] = {
  LEDSegment(0, 0, 30),      // Часть Ray 1 (TODO: реальные индексы)
  LEDSegment(1, 0, 30),      // Часть Ray 2
  LEDSegment(8, 0, 20),      // Часть Inner Circle
  LEDSegment(9, 0, 25)       // Часть Outer Circle
};

// Пример: Зона 1 - сектор между Ray 2 и Ray 3
LEDSegment zone1_segments[] = {
  LEDSegment(1, 31, 60),     // TODO: реальные индексы
  LEDSegment(2, 0, 30),
  LEDSegment(8, 21, 40),
  LEDSegment(9, 26, 50)
};

// Пример: Зона 2 - сектор между Ray 3 и Ray 4
LEDSegment zone2_segments[] = {
  LEDSegment(2, 31, 60),
  LEDSegment(3, 0, 30),
  LEDSegment(8, 41, 60),
  LEDSegment(9, 51, 75)
};

// TODO: Добавить остальные зоны по аналогии (зоны 3-14)

// ============================================================================
// МАССИВ ВСЕХ ЗОН
// ============================================================================

Zone zones[] = {
  Zone(0, "Sector 1-2", zone0_segments, 4),
  Zone(1, "Sector 2-3", zone1_segments, 4),
  Zone(2, "Sector 3-4", zone2_segments, 4),
  // TODO: Добавить зоны 3-14
};

#define NUM_ZONES (sizeof(zones) / sizeof(Zone))

// ============================================================================
// МАППИНГ АКТУАТОРОВ НА ЗОНЫ
// ============================================================================

/**
 * Каждый актуатор находится в точке пересечения луча и круга
 * При движении актуатора может быть логика подсветки соответствующей зоны
 */

struct ActuatorZoneMapping {
  uint8_t actuatorId;        // ID актуатора (0-30)
  uint8_t zoneId;            // ID зоны, которую он контролирует
  const char* location;      // Описание расположения
};

// Пример маппинга (TODO: заполнить для всех 31 актуатора)
const ActuatorZoneMapping ACTUATOR_MAPPINGS[] = {
  {0, 0, "Ray1-InnerCircle"},
  {1, 0, "Ray1-OuterCircle"},
  {2, 1, "Ray2-InnerCircle"},
  {3, 1, "Ray2-OuterCircle"},
  // TODO: остальные 27 актуаторов
};

#define NUM_ACTUATOR_MAPPINGS (sizeof(ACTUATOR_MAPPINGS) / sizeof(ActuatorZoneMapping))

// ============================================================================
// КОНФИГУРАЦИЯ РЕЛЕ ДЛЯ АКТУАТОРОВ
// ============================================================================

/**
 * Каждый актуатор управляется H-мостом через 2 реле
 * Всего 31 актуатор = 62 реле = 15 модулей по 4 канала + 1 модуль с 2 каналами
 */

struct ActuatorConfig {
  uint8_t id;                // ID актуатора (0-30)
  uint8_t relayForward;      // GPIO реле "вперед"
  uint8_t relayBackward;     // GPIO реле "назад"
  const char* name;          // Имя для отладки
  uint16_t moveDuration;     // Длительность движения (мс)

  // Текущее состояние
  enum State { STOP, FORWARD, BACKWARD } state;
  unsigned long moveStartTime;
};

// TODO: Заполнить конфигурацию для всех 31 актуатора
ActuatorConfig actuators[] = {
  {0, 32, 33, "Act-R1-Inner", 3000, ActuatorConfig::STOP, 0},
  {1, 34, 35, "Act-R1-Outer", 3000, ActuatorConfig::STOP, 0},
  // TODO: добавить остальные 29 актуаторов
};

#define NUM_ACTUATORS (sizeof(actuators) / sizeof(ActuatorConfig))

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================

/**
 * Получить зону по ID
 */
Zone* getZone(uint8_t zoneId) {
  if (zoneId >= NUM_ZONES) return nullptr;
  return &zones[zoneId];
}

/**
 * Получить актуатор по ID
 */
ActuatorConfig* getActuator(uint8_t actuatorId) {
  for (uint8_t i = 0; i < NUM_ACTUATORS; i++) {
    if (actuators[i].id == actuatorId) {
      return &actuators[i];
    }
  }
  return nullptr;
}

/**
 * Парсинг hex цвета
 */
CRGB parseHexColor(const String& hex) {
  String h = hex;
  if (h.startsWith("#")) h = h.substring(1);

  long number = strtol(h.c_str(), NULL, 16);
  uint8_t r = (number >> 16) & 0xFF;
  uint8_t g = (number >> 8) & 0xFF;
  uint8_t b = number & 0xFF;

  return CRGB(r, g, b);
}

/**
 * Вывод конфигурации в Serial
 */
void printZoneConfiguration() {
  Serial.println("\n========================================");
  Serial.println("  ZONE-BASED LED CONFIGURATION");
  Serial.println("========================================");
  Serial.print("Physical Lines: ");
  Serial.println(NUM_PHYSICAL_LINES);
  Serial.print("Logical Zones: ");
  Serial.println(NUM_ZONES);
  Serial.print("Actuators: ");
  Serial.println(NUM_ACTUATORS);

  Serial.println("\nZone Details:");
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    Zone* z = &zones[i];
    Serial.print("  Zone ");
    Serial.print(z->id);
    Serial.print(" [");
    Serial.print(z->name);
    Serial.print("]: ");
    Serial.print(z->segmentCount);
    Serial.print(" segments, ");
    Serial.print(z->totalPixels());
    Serial.println(" total pixels");

    for (uint8_t j = 0; j < z->segmentCount; j++) {
      LEDSegment* seg = &z->segments[j];
      Serial.print("    - Line ");
      Serial.print(seg->lineId);
      Serial.print(": pixels ");
      Serial.print(seg->startPixel);
      Serial.print("-");
      Serial.print(seg->endPixel);
      Serial.print(" (");
      Serial.print(seg->length());
      Serial.println(" LEDs)");
    }
  }

  Serial.println("========================================\n");
}

#endif // ZONE_CONFIG_H
