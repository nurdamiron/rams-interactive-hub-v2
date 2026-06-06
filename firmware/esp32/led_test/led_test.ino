/**
 * RAMS Screen - Zone Test (Sector Highlighting)
 *
 * Подсветка ЗОНАМИ (секторами), а не целыми линиями
 *
 * Каждая зона = сегменты на нескольких линиях одновременно
 */

#include <FastLED.h>

// Конфигурация
#define NUM_LINES 10
#define MAX_LEDS 400

// GPIO пины
const uint8_t LINE_PINS[NUM_LINES] = {
  16, 17, 18, 19, 21, 22, 23, 25, 26, 27
};

// Реальные размеры линий
const uint16_t LEDS_PER_LINE[NUM_LINES] = {
  45,  // Ray 1
  45,  // Ray 2
  45,  // Ray 3
  45,  // Ray 4
  45,  // Ray 5
  45,  // Ray 6
  45,  // Ray 7
  45,  // Ray 8
  378, // Inner Circle (против часовой)
  360  // Outer Circle (против часовой)
};

// Массивы LED
CRGB line0[MAX_LEDS];
CRGB line1[MAX_LEDS];
CRGB line2[MAX_LEDS];
CRGB line3[MAX_LEDS];
CRGB line4[MAX_LEDS];
CRGB line5[MAX_LEDS];
CRGB line6[MAX_LEDS];
CRGB line7[MAX_LEDS];
CRGB line8[MAX_LEDS];
CRGB line9[MAX_LEDS];

CRGB* lines[NUM_LINES] = {
  line0, line1, line2, line3, line4,
  line5, line6, line7, line8, line9
};

#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS 180

// ============================================================================
// СТРУКТУРА СЕГМЕНТА
// ============================================================================

struct Segment {
  uint8_t lineId;      // Какая линия
  uint16_t startPixel; // Начало
  uint16_t endPixel;   // Конец
};

// ============================================================================
// ОПРЕДЕЛЕНИЕ ЗОН (СЕКТОРОВ)
// ============================================================================

// ЗОНА 1 (между Ray 1 и Ray 2, вверху)
Segment zone1[] = {
  {0, 0, 44},      // Ray 1: весь луч (45 LED)
  {1, 0, 44},      // Ray 2: весь луч
  {9, 0, 44},      // Outer Circle: 1/8 (45 LED)
  {8, 0, 53}       // Inner Circle: 1/7 (54 LED) - ПРИМЕРНО
};
#define ZONE1_COUNT 4

// ЗОНА 2 (между Ray 2 и Ray 3)
Segment zone2[] = {
  {1, 0, 44},      // Ray 2
  {2, 0, 44},      // Ray 3
  {9, 45, 89},     // Outer Circle: 2/8
  {8, 54, 107}     // Inner Circle: 2/7
};
#define ZONE2_COUNT 4

// ЗОНА 3 (между Ray 3 и Ray 4, справа)
Segment zone3[] = {
  {2, 0, 44},      // Ray 3
  {3, 0, 44},      // Ray 4
  {9, 90, 134},    // Outer Circle: 3/8
  {8, 108, 161}    // Inner Circle: 3/7
};
#define ZONE3_COUNT 4

// ЗОНА 4 (между Ray 4 и Ray 5)
Segment zone4[] = {
  {3, 0, 44},      // Ray 4
  {4, 0, 44},      // Ray 5
  {9, 135, 179},   // Outer Circle: 4/8
  {8, 162, 215}    // Inner Circle: 4/7
};
#define ZONE4_COUNT 4

// ЗОНА 5 (между Ray 5 и Ray 6, внизу)
Segment zone5[] = {
  {4, 0, 44},      // Ray 5
  {5, 0, 44},      // Ray 6
  {9, 180, 224},   // Outer Circle: 5/8
  {8, 216, 269}    // Inner Circle: 5/7
};
#define ZONE5_COUNT 4

// ЗОНА 6 (между Ray 6 и Ray 7)
Segment zone6[] = {
  {5, 0, 44},      // Ray 6
  {6, 0, 44},      // Ray 7
  {9, 225, 269},   // Outer Circle: 6/8
  {8, 270, 323}    // Inner Circle: 6/7
};
#define ZONE6_COUNT 4

// ЗОНА 7 (между Ray 7 и Ray 8, слева)
Segment zone7[] = {
  {6, 0, 44},      // Ray 7
  {7, 0, 44},      // Ray 8
  {9, 270, 314},   // Outer Circle: 7/8
  {8, 324, 377}    // Inner Circle: 7/7 (последняя)
};
#define ZONE7_COUNT 4

// ЗОНА 8 (между Ray 8 и Ray 1)
Segment zone8[] = {
  {7, 0, 44},      // Ray 8
  {0, 0, 44},      // Ray 1 (замыкает круг)
  {9, 315, 359},   // Outer Circle: 8/8 (последняя)
  {8, 0, 53}       // Inner Circle: снова начало (круг замыкается)
};
#define ZONE8_COUNT 4

// ============================================================================
// ВНУТРЕННИЕ ЗОНЫ (9-15) - только сегменты внутреннего круга
// ============================================================================

// ЗОНА 9 (внутренняя, между Ray 1 и Ray 2)
Segment zone9[] = {
  {8, 0, 53}       // Inner Circle: 1/7
};
#define ZONE9_COUNT 1

// ЗОНА 10 (внутренняя, между Ray 2 и Ray 3)
Segment zone10[] = {
  {8, 54, 107}     // Inner Circle: 2/7
};
#define ZONE10_COUNT 1

// ЗОНА 11 (внутренняя, между Ray 3 и Ray 4)
Segment zone11[] = {
  {8, 108, 161}    // Inner Circle: 3/7
};
#define ZONE11_COUNT 1

// ЗОНА 12 (внутренняя, между Ray 4 и Ray 5)
Segment zone12[] = {
  {8, 162, 215}    // Inner Circle: 4/7
};
#define ZONE12_COUNT 1

// ЗОНА 13 (внутренняя, между Ray 5 и Ray 6)
Segment zone13[] = {
  {8, 216, 269}    // Inner Circle: 5/7
};
#define ZONE13_COUNT 1

// ЗОНА 14 (внутренняя, между Ray 6 и Ray 7)
Segment zone14[] = {
  {8, 270, 323}    // Inner Circle: 6/7
};
#define ZONE14_COUNT 1

// ЗОНА 15 (внутренняя, между Ray 7 и Ray 8)
Segment zone15[] = {
  {8, 324, 377}    // Inner Circle: 7/7
};
#define ZONE15_COUNT 1

// ============================================================================
// ФУНКЦИИ УПРАВЛЕНИЯ ЗОНАМИ
// ============================================================================

void lightUpZone(Segment* segments, uint8_t count, CRGB color) {
  for(uint8_t i = 0; i < count; i++) {
    Segment* seg = &segments[i];
    CRGB* line = lines[seg->lineId];

    for(uint16_t j = seg->startPixel; j <= seg->endPixel; j++) {
      line[j] = color;
    }
  }
  FastLED.show();
}

void clearAll() {
  for(uint8_t i = 0; i < NUM_LINES; i++) {
    fill_solid(lines[i], LEDS_PER_LINE[i], CRGB::Black);
  }
  FastLED.show();
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n========================================");
  Serial.println("  RAMS SCREEN - ZONE TEST");
  Serial.println("  (Sector Highlighting)");
  Serial.println("========================================\n");

  // Инициализация FastLED
  FastLED.addLeds<LED_TYPE, 16, COLOR_ORDER>(line0, LEDS_PER_LINE[0]);
  FastLED.addLeds<LED_TYPE, 17, COLOR_ORDER>(line1, LEDS_PER_LINE[1]);
  FastLED.addLeds<LED_TYPE, 18, COLOR_ORDER>(line2, LEDS_PER_LINE[2]);
  FastLED.addLeds<LED_TYPE, 19, COLOR_ORDER>(line3, LEDS_PER_LINE[3]);
  FastLED.addLeds<LED_TYPE, 21, COLOR_ORDER>(line4, LEDS_PER_LINE[4]);
  FastLED.addLeds<LED_TYPE, 22, COLOR_ORDER>(line5, LEDS_PER_LINE[5]);
  FastLED.addLeds<LED_TYPE, 23, COLOR_ORDER>(line6, LEDS_PER_LINE[6]);
  FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(line7, LEDS_PER_LINE[7]);
  FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(line8, LEDS_PER_LINE[8]);
  FastLED.addLeds<LED_TYPE, 27, COLOR_ORDER>(line9, LEDS_PER_LINE[9]);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  Serial.println("[LED] 10 lines initialized");
  Serial.println("[ZONE] 15 zones configured (8 outer + 7 inner)");
  Serial.println("\n[READY] Starting zone test...");
  Serial.println("========================================\n");

  delay(1000);
}

// ============================================================================
// MAIN LOOP - ТЕСТ ЗОН
// ============================================================================

void loop() {
  CRGB colors[15] = {
    CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green,
    CRGB::Cyan, CRGB::Blue, CRGB::Purple, CRGB::Magenta,
    CRGB::Pink, CRGB::Lime, CRGB::Aqua, CRGB::Fuchsia,
    CRGB::Gold, CRGB::Turquoise, CRGB::Coral
  };

  // ТЕСТ 1: Подсветка внешних зон (1-8)
  Serial.println("\n[TEST 1] Highlighting OUTER zones (1-8)...");

  Serial.println("  Zone 1 → RED");
  lightUpZone(zone1, ZONE1_COUNT, CRGB::Red);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 2 → ORANGE");
  lightUpZone(zone2, ZONE2_COUNT, CRGB::Orange);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 3 → YELLOW");
  lightUpZone(zone3, ZONE3_COUNT, CRGB::Yellow);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 4 → GREEN");
  lightUpZone(zone4, ZONE4_COUNT, CRGB::Green);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 5 → CYAN");
  lightUpZone(zone5, ZONE5_COUNT, CRGB::Cyan);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 6 → BLUE");
  lightUpZone(zone6, ZONE6_COUNT, CRGB::Blue);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 7 → PURPLE");
  lightUpZone(zone7, ZONE7_COUNT, CRGB::Purple);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 8 → MAGENTA");
  lightUpZone(zone8, ZONE8_COUNT, CRGB::Magenta);
  delay(1500);
  clearAll();
  delay(300);

  // ТЕСТ 1B: Подсветка внутренних зон (9-15)
  Serial.println("\n[TEST 1B] Highlighting INNER zones (9-15)...");

  Serial.println("  Zone 9 → PINK");
  lightUpZone(zone9, ZONE9_COUNT, CRGB::Pink);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 10 → LIME");
  lightUpZone(zone10, ZONE10_COUNT, CRGB::Lime);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 11 → AQUA");
  lightUpZone(zone11, ZONE11_COUNT, CRGB::Aqua);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 12 → FUCHSIA");
  lightUpZone(zone12, ZONE12_COUNT, CRGB::Fuchsia);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 13 → GOLD");
  lightUpZone(zone13, ZONE13_COUNT, CRGB::Gold);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 14 → TURQUOISE");
  lightUpZone(zone14, ZONE14_COUNT, CRGB::Turquoise);
  delay(1500);
  clearAll();
  delay(300);

  Serial.println("  Zone 15 → CORAL");
  lightUpZone(zone15, ZONE15_COUNT, CRGB::Coral);
  delay(1500);
  clearAll();
  delay(300);

  // ТЕСТ 2: Все зоны одновременно - разные цвета
  Serial.println("\n[TEST 2] All 15 zones at once - different colors");
  lightUpZone(zone1, ZONE1_COUNT, colors[0]);
  lightUpZone(zone2, ZONE2_COUNT, colors[1]);
  lightUpZone(zone3, ZONE3_COUNT, colors[2]);
  lightUpZone(zone4, ZONE4_COUNT, colors[3]);
  lightUpZone(zone5, ZONE5_COUNT, colors[4]);
  lightUpZone(zone6, ZONE6_COUNT, colors[5]);
  lightUpZone(zone7, ZONE7_COUNT, colors[6]);
  lightUpZone(zone8, ZONE8_COUNT, colors[7]);
  lightUpZone(zone9, ZONE9_COUNT, colors[8]);
  lightUpZone(zone10, ZONE10_COUNT, colors[9]);
  lightUpZone(zone11, ZONE11_COUNT, colors[10]);
  lightUpZone(zone12, ZONE12_COUNT, colors[11]);
  lightUpZone(zone13, ZONE13_COUNT, colors[12]);
  lightUpZone(zone14, ZONE14_COUNT, colors[13]);
  lightUpZone(zone15, ZONE15_COUNT, colors[14]);
  delay(3000);

  clearAll();
  delay(500);

  // ТЕСТ 3: Волна по внешним зонам (1-8)
  Serial.println("\n[TEST 3] Wave across OUTER zones (1-8)");
  for(int wave = 0; wave < 3; wave++) {
    lightUpZone(zone1, ZONE1_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone2, ZONE2_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone3, ZONE3_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone4, ZONE4_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone5, ZONE5_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone6, ZONE6_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone7, ZONE7_COUNT, CRGB::White);
    delay(200);
    clearAll();

    lightUpZone(zone8, ZONE8_COUNT, CRGB::White);
    delay(200);
    clearAll();
  }

  delay(500);

  // ТЕСТ 4: Волна по внутренним зонам (9-15)
  Serial.println("\n[TEST 4] Wave across INNER zones (9-15)");
  for(int wave = 0; wave < 3; wave++) {
    lightUpZone(zone9, ZONE9_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();

    lightUpZone(zone10, ZONE10_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();

    lightUpZone(zone11, ZONE11_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();

    lightUpZone(zone12, ZONE12_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();

    lightUpZone(zone13, ZONE13_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();

    lightUpZone(zone14, ZONE14_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();

    lightUpZone(zone15, ZONE15_COUNT, CRGB::Cyan);
    delay(150);
    clearAll();
  }

  Serial.println("\n[TEST] Cycle complete. Restarting in 2 seconds...\n");
  Serial.println("========================================");
  delay(2000);
}
