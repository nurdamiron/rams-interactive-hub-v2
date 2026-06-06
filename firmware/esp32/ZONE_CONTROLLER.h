/**
 * RAMS Screen - Zone Controller Library
 *
 * Библиотека для управления зонами через FastLED
 * Каждая зона состоит из сегментов на разных физических линиях
 *
 * @version 2.0
 * @date 2026-02-04
 */

#ifndef ZONE_CONTROLLER_H
#define ZONE_CONTROLLER_H

#include <FastLED.h>
#include "ZONE_CONFIG.h"

// ============================================================================
// ГЛОБАЛЬНЫЕ МАССИВЫ LED
// ============================================================================

// Массивы для каждой физической линии
CRGB line0[LEDS_PER_LINE[0]];
CRGB line1[LEDS_PER_LINE[1]];
CRGB line2[LEDS_PER_LINE[2]];
CRGB line3[LEDS_PER_LINE[3]];
CRGB line4[LEDS_PER_LINE[4]];
CRGB line5[LEDS_PER_LINE[5]];
CRGB line6[LEDS_PER_LINE[6]];
CRGB line7[LEDS_PER_LINE[7]];
CRGB line8[LEDS_PER_LINE[8]];
CRGB line9[LEDS_PER_LINE[9]];

// Указатели на массивы для удобного доступа по индексу
CRGB* ledLines[NUM_PHYSICAL_LINES] = {
  line0, line1, line2, line3, line4,
  line5, line6, line7, line8, line9
};

// ============================================================================
// КЛАСС ZONE CONTROLLER
// ============================================================================

class ZoneController {
public:

  /**
   * Инициализация FastLED для всех физических линий
   */
  static void init() {
    Serial.println("[ZONE] Initializing FastLED for all lines...");

    // Добавляем каждую линию в FastLED с её собственным пином
    FastLED.addLeds<LED_TYPE, LED_PINS[0], COLOR_ORDER>(line0, LEDS_PER_LINE[0]);
    FastLED.addLeds<LED_TYPE, LED_PINS[1], COLOR_ORDER>(line1, LEDS_PER_LINE[1]);
    FastLED.addLeds<LED_TYPE, LED_PINS[2], COLOR_ORDER>(line2, LEDS_PER_LINE[2]);
    FastLED.addLeds<LED_TYPE, LED_PINS[3], COLOR_ORDER>(line3, LEDS_PER_LINE[3]);
    FastLED.addLeds<LED_TYPE, LED_PINS[4], COLOR_ORDER>(line4, LEDS_PER_LINE[4]);
    FastLED.addLeds<LED_TYPE, LED_PINS[5], COLOR_ORDER>(line5, LEDS_PER_LINE[5]);
    FastLED.addLeds<LED_TYPE, LED_PINS[6], COLOR_ORDER>(line6, LEDS_PER_LINE[6]);
    FastLED.addLeds<LED_TYPE, LED_PINS[7], COLOR_ORDER>(line7, LEDS_PER_LINE[7]);
    FastLED.addLeds<LED_TYPE, LED_PINS[8], COLOR_ORDER>(line8, LEDS_PER_LINE[8]);
    FastLED.addLeds<LED_TYPE, LED_PINS[9], COLOR_ORDER>(line9, LEDS_PER_LINE[9]);

    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();

    Serial.print("[ZONE] Initialized ");
    Serial.print(NUM_PHYSICAL_LINES);
    Serial.println(" physical LED lines ✓");
  }

  /**
   * Установить цвет для всей зоны
   */
  static void setZoneColor(uint8_t zoneId, CRGB color) {
    Zone* zone = getZone(zoneId);
    if (!zone) {
      Serial.print("[ERROR] Zone not found: ");
      Serial.println(zoneId);
      return;
    }

    // Закрасить все сегменты зоны
    for (uint8_t i = 0; i < zone->segmentCount; i++) {
      LEDSegment* seg = &zone->segments[i];
      fillSegment(seg, color);
    }

    zone->currentColor = color;
    zone->isActive = true;

    FastLED.show();

    Serial.print("[ZONE] Set color for zone ");
    Serial.print(zoneId);
    Serial.print(" [");
    Serial.print(zone->name);
    Serial.println("]");
  }

  /**
   * Установить цвет с яркостью
   */
  static void setZoneColor(uint8_t zoneId, CRGB color, uint8_t brightness) {
    Zone* zone = getZone(zoneId);
    if (!zone) return;

    // Применить яркость к цвету
    color.fadeLightBy(255 - brightness);

    for (uint8_t i = 0; i < zone->segmentCount; i++) {
      LEDSegment* seg = &zone->segments[i];
      fillSegment(seg, color);
    }

    zone->currentColor = color;
    zone->brightness = brightness;
    zone->isActive = true;

    FastLED.show();
  }

  /**
   * Выключить зону (плавное затухание)
   */
  static void fadeOutZone(uint8_t zoneId, uint16_t duration = 500) {
    Zone* zone = getZone(zoneId);
    if (!zone || !zone->isActive) return;

    Serial.print("[ZONE] Fading out zone ");
    Serial.println(zoneId);

    uint8_t steps = 50;
    uint16_t delayMs = duration / steps;

    for (uint8_t brightness = zone->brightness; brightness > 0; brightness -= (zone->brightness / steps)) {
      for (uint8_t i = 0; i < zone->segmentCount; i++) {
        LEDSegment* seg = &zone->segments[i];
        CRGB color = zone->currentColor;
        color.fadeLightBy(255 - brightness);
        fillSegment(seg, color);
      }
      FastLED.show();
      delay(delayMs);
    }

    // Полностью выключить
    clearZone(zoneId);
    zone->isActive = false;
  }

  /**
   * Включить зону с плавным появлением
   */
  static void fadeInZone(uint8_t zoneId, CRGB color, uint16_t duration = 500) {
    Zone* zone = getZone(zoneId);
    if (!zone) return;

    Serial.print("[ZONE] Fading in zone ");
    Serial.println(zoneId);

    uint8_t steps = 50;
    uint16_t delayMs = duration / steps;
    uint8_t targetBrightness = zone->brightness;

    for (uint8_t brightness = 0; brightness <= targetBrightness; brightness += (targetBrightness / steps)) {
      for (uint8_t i = 0; i < zone->segmentCount; i++) {
        LEDSegment* seg = &zone->segments[i];
        CRGB c = color;
        c.fadeLightBy(255 - brightness);
        fillSegment(seg, c);
      }
      FastLED.show();
      delay(delayMs);
    }

    zone->currentColor = color;
    zone->isActive = true;
  }

  /**
   * Очистить зону (мгновенно)
   */
  static void clearZone(uint8_t zoneId) {
    Zone* zone = getZone(zoneId);
    if (!zone) return;

    for (uint8_t i = 0; i < zone->segmentCount; i++) {
      LEDSegment* seg = &zone->segments[i];
      fillSegment(seg, CRGB::Black);
    }

    FastLED.show();
    zone->isActive = false;
  }

  /**
   * Очистить все зоны
   */
  static void clearAll() {
    Serial.println("[ZONE] Clearing all zones...");

    for (uint8_t i = 0; i < NUM_ZONES; i++) {
      clearZone(i);
    }

    FastLED.clear();
    FastLED.show();
  }

  /**
   * Тестовый режим - подсветка всех зон по очереди
   */
  static void testAllZones(uint16_t delayMs = 1000) {
    Serial.println("[ZONE] Starting test mode...");

    for (uint8_t i = 0; i < NUM_ZONES; i++) {
      Zone* zone = &zones[i];

      Serial.print("Testing Zone ");
      Serial.print(i);
      Serial.print(" [");
      Serial.print(zone->name);
      Serial.println("]");

      // Случайный цвет для каждой зоны
      CRGB testColor = CHSV(random(0, 255), 255, 255);
      setZoneColor(i, testColor);

      delay(delayMs);
      clearZone(i);
      delay(200);
    }

    Serial.println("[ZONE] Test complete");
  }

  /**
   * Эффект: пульсация зоны
   */
  static void pulseZone(uint8_t zoneId, CRGB color, uint8_t speed = 5) {
    static uint8_t pulseValue = 0;
    static int8_t pulseDirection = 1;

    Zone* zone = getZone(zoneId);
    if (!zone) return;

    CRGB c = color;
    c.fadeLightBy(255 - pulseValue);

    for (uint8_t i = 0; i < zone->segmentCount; i++) {
      fillSegment(&zone->segments[i], c);
    }

    FastLED.show();

    pulseValue += pulseDirection * speed;
    if (pulseValue >= 255 || pulseValue <= 50) {
      pulseDirection *= -1;
    }

    zone->currentColor = color;
    zone->isActive = true;
  }

  /**
   * Эффект: радуга для зоны
   */
  static void rainbowZone(uint8_t zoneId, uint8_t hueOffset = 0) {
    Zone* zone = getZone(zoneId);
    if (!zone) return;

    for (uint8_t i = 0; i < zone->segmentCount; i++) {
      LEDSegment* seg = &zone->segments[i];
      CRGB* line = ledLines[seg->lineId];

      for (uint16_t j = seg->startPixel; j <= seg->endPixel; j++) {
        line[j] = CHSV(hueOffset + (j * 5), 255, zone->brightness);
      }
    }

    FastLED.show();
    zone->isActive = true;
  }

  /**
   * Установить яркость для зоны
   */
  static void setZoneBrightness(uint8_t zoneId, uint8_t brightness) {
    Zone* zone = getZone(zoneId);
    if (!zone || !zone->isActive) return;

    zone->brightness = brightness;

    CRGB color = zone->currentColor;
    color.fadeLightBy(255 - brightness);

    for (uint8_t i = 0; i < zone->segmentCount; i++) {
      fillSegment(&zone->segments[i], color);
    }

    FastLED.show();
  }

private:

  /**
   * Заполнить сегмент цветом
   */
  static void fillSegment(LEDSegment* segment, CRGB color) {
    CRGB* line = ledLines[segment->lineId];

    for (uint16_t i = segment->startPixel; i <= segment->endPixel; i++) {
      line[i] = color;
    }
  }
};

// ============================================================================
// МЕНЕДЖЕР АКТУАТОРОВ
// ============================================================================

class ActuatorController {
public:

  /**
   * Инициализация всех пинов актуаторов
   */
  static void init() {
    Serial.println("[ACTUATOR] Initializing actuator pins...");

    for (uint8_t i = 0; i < NUM_ACTUATORS; i++) {
      ActuatorConfig* act = &actuators[i];
      pinMode(act->relayForward, OUTPUT);
      pinMode(act->relayBackward, OUTPUT);
      digitalWrite(act->relayForward, LOW);
      digitalWrite(act->relayBackward, LOW);
      act->state = ActuatorConfig::STOP;
    }

    Serial.print("[ACTUATOR] Initialized ");
    Serial.print(NUM_ACTUATORS);
    Serial.println(" actuators ✓");
  }

  /**
   * Двигать актуатор вперед
   */
  static void moveForward(uint8_t actuatorId, uint16_t duration = 0) {
    ActuatorConfig* act = getActuator(actuatorId);
    if (!act) return;

    stopActuator(actuatorId);  // Сначала остановить
    delay(10);  // Короткая пауза для защиты реле

    digitalWrite(act->relayForward, HIGH);
    digitalWrite(act->relayBackward, LOW);
    act->state = ActuatorConfig::FORWARD;
    act->moveStartTime = millis();

    if (duration > 0) {
      act->moveDuration = duration;
    }

    Serial.print("[ACTUATOR] ");
    Serial.print(act->name);
    Serial.println(" -> FORWARD");
  }

  /**
   * Двигать актуатор назад
   */
  static void moveBackward(uint8_t actuatorId, uint16_t duration = 0) {
    ActuatorConfig* act = getActuator(actuatorId);
    if (!act) return;

    stopActuator(actuatorId);
    delay(10);

    digitalWrite(act->relayForward, LOW);
    digitalWrite(act->relayBackward, HIGH);
    act->state = ActuatorConfig::BACKWARD;
    act->moveStartTime = millis();

    if (duration > 0) {
      act->moveDuration = duration;
    }

    Serial.print("[ACTUATOR] ");
    Serial.print(act->name);
    Serial.println(" -> BACKWARD");
  }

  /**
   * Остановить актуатор
   */
  static void stopActuator(uint8_t actuatorId) {
    ActuatorConfig* act = getActuator(actuatorId);
    if (!act) return;

    digitalWrite(act->relayForward, LOW);
    digitalWrite(act->relayBackward, LOW);
    act->state = ActuatorConfig::STOP;

    Serial.print("[ACTUATOR] ");
    Serial.print(act->name);
    Serial.println(" -> STOP");
  }

  /**
   * Остановить все актуаторы
   */
  static void stopAll() {
    Serial.println("[ACTUATOR] Stopping all actuators...");

    for (uint8_t i = 0; i < NUM_ACTUATORS; i++) {
      stopActuator(actuators[i].id);
    }
  }

  /**
   * Обновление состояния актуаторов (вызывать в loop)
   * Автоматически останавливает актуаторы по таймеру
   */
  static void update() {
    unsigned long now = millis();

    for (uint8_t i = 0; i < NUM_ACTUATORS; i++) {
      ActuatorConfig* act = &actuators[i];

      if (act->state != ActuatorConfig::STOP) {
        if (now - act->moveStartTime >= act->moveDuration) {
          stopActuator(act->id);
        }
      }
    }
  }
};

#endif // ZONE_CONTROLLER_H
