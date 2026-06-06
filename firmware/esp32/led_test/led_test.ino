/**
 * RAMS KRUG2 - LED Line Test (WS2815, 9 линий)
 *
 * Простой тест физической распайки: по очереди зажигает каждую из 9 линий,
 * потом все вместе. Удобно проверить, что DI каждой линии сидит на своём GPIO,
 * порядок цветов (GRB) и резервная линия BI (перемычка на DI) спаяны верно.
 *
 * ЗАГЛУШКИ: GPIO и число диодов — уточнить по реальной распайке (TODO ниже).
 */

#include <FastLED.h>

// ============================================================================
// КОНФИГУРАЦИЯ (ЗАГЛУШКА — синхронизировать с ZONE_CONFIG.h)
// ============================================================================
#define NUM_LINES   9
#define MAX_LEDS    300        // запас под самую длинную линию

#define LED_TYPE    WS2815     // 12V dual-signal (DI+BI). Нет в FastLED → WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  180

// GPIO пины (DI каждой линии). TODO: реальная распайка
const uint8_t LINE_PINS[NUM_LINES] = {
  16, 17, 18, 19, 21, 22, 23, 25, 26
};

// Число диодов на каждой линии. TODO: измерить
const uint16_t LEDS_PER_LINE[NUM_LINES] = {
  100, 100, 100, 100, 100, 100, 100, 100, 100
};

// Буферы линий
CRGB lines[NUM_LINES][MAX_LEDS];

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== RAMS KRUG2 LED TEST (9 lines, WS2815) ===");

  // FastLED требует пины константами на этапе компиляции — перечисляем явно.
  FastLED.addLeds<LED_TYPE, 16, COLOR_ORDER>(lines[0], LEDS_PER_LINE[0]);
  FastLED.addLeds<LED_TYPE, 17, COLOR_ORDER>(lines[1], LEDS_PER_LINE[1]);
  FastLED.addLeds<LED_TYPE, 18, COLOR_ORDER>(lines[2], LEDS_PER_LINE[2]);
  FastLED.addLeds<LED_TYPE, 19, COLOR_ORDER>(lines[3], LEDS_PER_LINE[3]);
  FastLED.addLeds<LED_TYPE, 21, COLOR_ORDER>(lines[4], LEDS_PER_LINE[4]);
  FastLED.addLeds<LED_TYPE, 22, COLOR_ORDER>(lines[5], LEDS_PER_LINE[5]);
  FastLED.addLeds<LED_TYPE, 23, COLOR_ORDER>(lines[6], LEDS_PER_LINE[6]);
  FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(lines[7], LEDS_PER_LINE[7]);
  FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(lines[8], LEDS_PER_LINE[8]);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);
}

void clearAll() {
  for (uint8_t l = 0; l < NUM_LINES; l++) {
    fill_solid(lines[l], LEDS_PER_LINE[l], CRGB::Black);
  }
}

void loop() {
  // 1) По очереди каждая линия — белым (проверка GPIO/распайки)
  for (uint8_t l = 0; l < NUM_LINES; l++) {
    clearAll();
    fill_solid(lines[l], LEDS_PER_LINE[l], CRGB::White);
    FastLED.show();
    Serial.print("Line "); Serial.print(l);
    Serial.print(" (GPIO "); Serial.print(LINE_PINS[l]); Serial.println(") ON");
    delay(700);
  }

  // 2) Проверка порядка цветов: R, G, B на всех линиях
  const CRGB rgb[3] = { CRGB::Red, CRGB::Green, CRGB::Blue };
  const char* nm[3] = { "RED", "GREEN", "BLUE" };
  for (uint8_t c = 0; c < 3; c++) {
    for (uint8_t l = 0; l < NUM_LINES; l++) {
      fill_solid(lines[l], LEDS_PER_LINE[l], rgb[c]);
    }
    FastLED.show();
    Serial.print("All lines: "); Serial.println(nm[c]);
    delay(800);
  }

  clearAll();
  FastLED.show();
  delay(500);
}
