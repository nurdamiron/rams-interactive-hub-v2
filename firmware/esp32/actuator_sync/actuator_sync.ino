/**
 * RAMS Screen - ESP32 Actuator Sync
 *
 * Синхронизация LED подсветки с актуаторами Arduino Mega
 * - Прием команд по Serial от Arduino Mega
 * - Плавное включение/выключение LED зон (5 секунд)
 * - Белый цвет подсветки
 *
 * Подключение:
 * Arduino Mega TX1 (pin 18) → Level Converter HV → LV → ESP32 RX (GPIO 16)
 * Arduino Mega RX1 (pin 19) → Level Converter LV → HV → ESP32 TX (GPIO 17)
 * Level Converter: HV=5V (Arduino side), LV=3.3V (ESP32 side)
 */

#include <FastLED.h>

// LED Configuration
#define LED_PIN 5
#define NUM_LEDS 810  // Общее количество светодиодов (подсчитаем точно)

// LED Zones (15 zones total)
// Зоны 1,3,6,8,10,12,14: по 45 LEDs (прямые линии)
// Зоны 2,4,5,7,9,11,13: по 54 LEDs (сегменты круга)
// Зона 15: 54 LEDs (внутренний круг, разделен на 8 сегментов)

struct Zone {
  int start;
  int count;
};

// Карта LED зон (начало и количество)
Zone zones[15] = {
  {0, 45},      // Zone 1 - прямая линия верх
  {45, 54},     // Zone 2 - круг верх-право
  {99, 45},     // Zone 3 - прямая линия право
  {144, 54},    // Zone 4 - круг право
  {198, 54},    // Zone 5 - круг право-низ
  {252, 45},    // Zone 6 - прямая линия право-низ
  {297, 54},    // Zone 7 - круг низ-право
  {351, 45},    // Zone 8 - прямая линия низ
  {396, 54},    // Zone 9 - круг низ-лево
  {450, 45},    // Zone 10 - прямая линия низ-лево
  {495, 54},    // Zone 11 - круг лево
  {549, 45},    // Zone 12 - прямая линия лево
  {594, 54},    // Zone 13 - круг лево-верх
  {648, 45},    // Zone 14 - прямая линия лево-верх
  {693, 54}     // Zone 15 - внутренний круг (центр)
};

// Карта территорий: какой блок актуатора включает какие LED зоны
int blockZones[8][4] = {
  {1, 2, 15, -1},     // Блок 1: зоны 1, 2, 15
  {3, 4, 15, -1},     // Блок 2: зоны 3, 4, 15
  {3, 5, 15, -1},     // Блок 3: зоны 3, 5, 15
  {6, 5, 7, 15},      // Блок 4: зоны 6, 5, 7, 15
  {8, 7, 15, -1},     // Блок 5: зоны 8, 7, 15
  {8, 9, 15, -1},     // Блок 6: зоны 8, 9, 15
  {10, 11, 15, -1},   // Блок 7: зоны 10, 11, 15
  {12, 13, 15, -1}    // Блок 8: зоны 12, 13, 15
};

CRGB leds[NUM_LEDS];

// Serial от Arduino Mega
#define RX_PIN 16
#define TX_PIN 17

// Fade параметры
#define FADE_DURATION 5000  // 5 секунд
unsigned long fadeStartTime = 0;
int currentBlock = -1;
bool isFadingIn = false;
bool isFadingOut = false;
uint8_t targetBrightness = 0;
uint8_t currentBrightness = 0;

void setup() {
  // Debug Serial (USB)
  Serial.begin(115200);
  delay(100);

  // Serial от Arduino Mega
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("\n========================================");
  Serial.println("  RAMS SCREEN - ESP32 LED Controller");
  Serial.println("  Waiting for Arduino Mega commands...");
  Serial.println("  RX: GPIO16, TX: GPIO17");
  Serial.println("========================================\n");

  // Инициализация LED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.clear();
  FastLED.show();

  Serial.println("[LED] FastLED initialized");
  Serial.println("[SERIAL] Ready to receive commands");
}

void loop() {
  // Читаем команды от Arduino Mega
  if (Serial2.available()) {
    String command = Serial2.readStringUntil('\n');
    command.trim();

    if (command.length() >= 3) {
      processCommand(command);
    }
  }

  // Обрабатываем fade
  if (isFadingIn || isFadingOut) {
    updateFade();
  }
}

void processCommand(String cmd) {
  Serial.print("[CMD] Received: ");
  Serial.println(cmd);

  // Формат команды: "B1U" = Block 1 Up, "B2D" = Block 2 Down
  if (cmd.charAt(0) != 'B') return;

  int blockNum = cmd.substring(1, cmd.length()-1).toInt();
  char action = cmd.charAt(cmd.length()-1);

  if (blockNum < 1 || blockNum > 8) {
    Serial.println("[ERROR] Invalid block number");
    return;
  }

  if (action == 'U') {
    // Блок поднимается - fade in
    startFadeIn(blockNum);
  } else if (action == 'D') {
    // Блок опускается - fade out
    startFadeOut(blockNum);
  }
}

void startFadeIn(int blockNum) {
  Serial.print("[FADE IN] Block ");
  Serial.print(blockNum);
  Serial.println(" - 5 seconds");

  currentBlock = blockNum - 1;  // 0-indexed
  isFadingIn = true;
  isFadingOut = false;
  fadeStartTime = millis();
  currentBrightness = 0;
  targetBrightness = 255;
}

void startFadeOut(int blockNum) {
  Serial.print("[FADE OUT] Block ");
  Serial.print(blockNum);
  Serial.println(" - 5 seconds");

  currentBlock = blockNum - 1;  // 0-indexed
  isFadingIn = false;
  isFadingOut = true;
  fadeStartTime = millis();
  currentBrightness = 255;
  targetBrightness = 0;
}

void updateFade() {
  unsigned long elapsed = millis() - fadeStartTime;

  if (elapsed >= FADE_DURATION) {
    // Fade завершен
    currentBrightness = targetBrightness;
    isFadingIn = false;
    isFadingOut = false;

    Serial.print("[FADE] Complete - Brightness: ");
    Serial.println(currentBrightness);
  } else {
    // Вычисляем текущую яркость
    float progress = (float)elapsed / FADE_DURATION;

    if (isFadingIn) {
      currentBrightness = (uint8_t)(progress * 255);
    } else {
      currentBrightness = (uint8_t)((1.0 - progress) * 255);
    }
  }

  // Применяем яркость к LED зонам текущего блока
  applyBlockBrightness(currentBlock, currentBrightness);
  FastLED.show();
}

void applyBlockBrightness(int block, uint8_t brightness) {
  if (block < 0 || block >= 8) return;

  // Получаем зоны для этого блока
  for (int i = 0; i < 4; i++) {
    int zoneNum = blockZones[block][i];
    if (zoneNum == -1) break;  // Нет больше зон

    int zoneIndex = zoneNum - 1;  // 0-indexed
    if (zoneIndex < 0 || zoneIndex >= 15) continue;

    // Устанавливаем белый цвет с яркостью для всех LED в зоне
    int start = zones[zoneIndex].start;
    int count = zones[zoneIndex].count;

    for (int j = start; j < start + count; j++) {
      leds[j] = CRGB(brightness, brightness, brightness);  // Белый цвет
    }
  }
}
