/**
 * RAMS Controller v3.2 - PRODUCTION (DroneControl Style)
 *
 * Управляет 15 актуаторными блоками + LED зонами
 * - ESP32 → Serial1 (GPIO25/26) → Mega #1 (блоки 1-8)
 * - ESP32 → Serial2 (GPIO16/17) → Mega #2 (блоки 9-15)
 *
 * LED конфигурация (9 лент, подтверждено физическим тестом):
 *  idx  GPIO  Лента
 *   0    21   Большой круг (600 LED, WS2815)
 *   1     4   Луч 1 (110 LED, WS2815)
 *   2    14   Луч 2 (110 LED, WS2815)
 *   3    27   Луч 3 (110 LED, WS2815)
 *   4    32   Луч 4 (110 LED, WS2815)
 *   5     2   Луч 5 (110 LED, WS2815)
 *   6    23   Короткий луч (50 LED, WS2815, последовательно на GPIO 23)
 *   7    23   Внутренний круг (146 LED, WS2815)
 *   8    18   Луч 6 (110 LED, WS2815)
 *
 * Логика: Блок 1 UP → Актуаторы 1 UP + LED зона 1 ON
 *
 * @version 3.2 (Production)
 * @date 2026-02-15
 * @author RAMS Global Team
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include "led_tester_html.h"
// ВАЖНО: FASTLED_ALLOW_INTERRUPTS 0 был УБРАН!
// На ESP32 RMT-драйвер использует аппаратные прерывания для дозаполнения
// буфера при передаче данных на большое кол-во LED (600 шт).
// С FASTLED_ALLOW_INTERRUPTS 0 прерывания блокируются → мусор в ленте!
#define FASTLED_RMT_MAX_CHANNELS 8   // Явно задаем макс. кол-во RMT каналов
#include <FastLED.h>
#include <NeoPixelBus.h>
#include "ACTUATOR_CONFIG.h"


// ============================================================================
// WiFi КОНФИГУРАЦИЯ
// ============================================================================
// Station Mode - подключение к общему WiFi роутеру
#define WIFI_SSID   "Rams_WIFI"
#define WIFI_PASS   "Rams2021"

// AP Mode (резервный, если не подключился к роутеру)
#define AP_SSID     "RAMS_Controller"
#define AP_PASS     "rams2026"

// ============================================================================
// LED КОНФИГУРАЦИЯ (из svetdiod-project)
// ============================================================================
#define NUM_STRIPS  9
#define MAX_LEDS    600

// Индексы лучей и кругов
#define S_INNER  7   // GPIO 23, внутренний круг
#define S_OUTER  0   // GPIO 22, большой круг (внешний)

// Настройки поведения Большого круга (для тестов разделения)
const bool ENABLE_BIG_CIRCLE_ON_BLOCKS = true;  // true = большой круг зажигается вместе с блоками
const bool ENABLE_BIG_CIRCLE_ON_EFFECTS = true; // true = большая лента участвует в общих эффектах

// GPIO пины для LED лент (подтверждено физическим тестом)
//  idx:    0    1    2    3    4    5    6    7    8
// pin:    19    4   15   23   32    2   18   27   18
// desc:   Big  Ray1 Ray2 Ray3 Ray4  Ray5 Shrt Innr Ray6
// NOTE:   idx6=nullptr (Short chained after Ray 6 on GPIO18). GPIO19=Big Circle WS2815.
static const uint8_t  PIN_GPIO[NUM_STRIPS] = { 19,   4,  15,  23,  32,   2,  18,  27,  18 };
static const uint16_t PIN_LEDS[NUM_STRIPS] = {531, 110, 110, 110, 110, 110,  49, 147, 110 };


static CRGB leds[NUM_STRIPS][MAX_LEDS];
static uint8_t heat[NUM_STRIPS][MAX_LEDS];  // Для эффекта Fire
static CRGB ray6AndShortCombined[159];       // Объединенный буфер для Луча 6 (110) + Короткой линии (49)

// Инициализируем NeoPixelBus с использованием аппаратных RMT каналов 0-7
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt0Ws2812xMethod> strip0(531, 19);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt1Ws2812xMethod> strip1(110, 4);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt2Ws2812xMethod> strip2(110, 15);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt3Ws2812xMethod> strip3(110, 23);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt4Ws2812xMethod> strip4(110, 32);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt5Ws2812xMethod> strip5(110, 2);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt6Ws2812xMethod> strip7(147, 27);
NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt7Ws2812xMethod> strip8(159, 18);

// Глобальные LED параметры
uint8_t gR = 0, gG = 150, gB = 255;  // Текущий промежуточный цвет
uint8_t targetR = 0, targetG = 150, targetB = 255; // Целевой цвет для плавного перехода
uint8_t gBri = 200;
uint8_t gFx = 0;    // Текущий эффект: 0=Static, 1=Pulse, 2=Rainbow, 3=Chase, 4=Sparkle, 5=Wave, 6=Fire, 7=Meteor
uint8_t gSpd = 128; // Скорость эффекта (0-255)

#define FPS 50  // Частота обновления эффектов

void showLEDs() {
  // Копируем данные Луча 6 (110 LED) и Короткой линии (49 LED) в общий буфер
  memcpy(ray6AndShortCombined, leds[8], 110 * sizeof(CRGB));
  memcpy(ray6AndShortCombined + 110, leds[6], 49 * sizeof(CRGB));

  // 1. Копируем и масштабируем цвета в буферы NeoPixelBus
  
  // Strip 0: Big Circle (531 LED, GPIO 19, RMT 0)
  if (ENABLE_BIG_CIRCLE_ON_BLOCKS || ENABLE_BIG_CIRCLE_ON_EFFECTS) {
    for (uint16_t i = 0; i < 531; i++) {
      strip0.SetPixelColor(i, RgbColor(
        (uint16_t)leds[0][i].r * gBri / 255,
        (uint16_t)leds[0][i].g * gBri / 255,
        (uint16_t)leds[0][i].b * gBri / 255
      ));
    }
  }

  // Strip 1: Ray 1 (110 LED, GPIO 4, RMT 1)
  for (uint16_t i = 0; i < 110; i++) {
    strip1.SetPixelColor(i, RgbColor(
      (uint16_t)leds[1][i].r * gBri / 255,
      (uint16_t)leds[1][i].g * gBri / 255,
      (uint16_t)leds[1][i].b * gBri / 255
    ));
  }

  // Strip 2: Ray 2 (110 LED, GPIO 15, RMT 2)
  for (uint16_t i = 0; i < 110; i++) {
    strip2.SetPixelColor(i, RgbColor(
      (uint16_t)leds[2][i].r * gBri / 255,
      (uint16_t)leds[2][i].g * gBri / 255,
      (uint16_t)leds[2][i].b * gBri / 255
    ));
  }

  // Strip 3: Ray 3 (110 LED, GPIO 23, RMT 3)
  for (uint16_t i = 0; i < 110; i++) {
    strip3.SetPixelColor(i, RgbColor(
      (uint16_t)leds[3][i].r * gBri / 255,
      (uint16_t)leds[3][i].g * gBri / 255,
      (uint16_t)leds[3][i].b * gBri / 255
    ));
  }

  // Strip 4: Ray 4 (110 LED, GPIO 32, RMT 4)
  for (uint16_t i = 0; i < 110; i++) {
    strip4.SetPixelColor(i, RgbColor(
      (uint16_t)leds[4][i].r * gBri / 255,
      (uint16_t)leds[4][i].g * gBri / 255,
      (uint16_t)leds[4][i].b * gBri / 255
    ));
  }

  // Strip 5: Ray 5 (110 LED, GPIO 2, RMT 5)
  for (uint16_t i = 0; i < 110; i++) {
    strip5.SetPixelColor(i, RgbColor(
      (uint16_t)leds[5][i].r * gBri / 255,
      (uint16_t)leds[5][i].g * gBri / 255,
      (uint16_t)leds[5][i].b * gBri / 255
    ));
  }

  // Strip 7: Inner Circle (147 LED, GPIO 27, RMT 6)
  for (uint16_t i = 0; i < 147; i++) {
    strip7.SetPixelColor(i, RgbColor(
      (uint16_t)leds[7][i].r * gBri / 255,
      (uint16_t)leds[7][i].g * gBri / 255,
      (uint16_t)leds[7][i].b * gBri / 255
    ));
  }

  // Strip 8: Combined Ray 6 + Short (159 LED, GPIO 18, RMT 7)
  for (uint16_t i = 0; i < 159; i++) {
    strip8.SetPixelColor(i, RgbColor(
      (uint16_t)ray6AndShortCombined[i].r * gBri / 255,
      (uint16_t)ray6AndShortCombined[i].g * gBri / 255,
      (uint16_t)ray6AndShortCombined[i].b * gBri / 255
    ));
  }

  // 2. Отправляем данные на ленты (Show)
  if (ENABLE_BIG_CIRCLE_ON_BLOCKS || ENABLE_BIG_CIRCLE_ON_EFFECTS) {
    strip0.Show();
  }
  strip1.Show();
  strip2.Show();
  strip3.Show();
  strip4.Show();
  strip5.Show();
  strip7.Show();
  strip8.Show();
}

// Разделение луча на внутреннюю/внешнюю части
#define RAY_IN_START   0
#define RAY_IN_COUNT  18   // 0-17 (18 LED)
#define RAY_OUT_START 18
#define RAY_OUT_COUNT 15   // 18-32 (15 LED)



// Маппинг лучей (idx в массиве leds[] → Ray 1-8)
// Используем только существующие 6 лучей (индексы: 1, 2, 3, 4, 5, 8)
// Это предотвращает выход за пределы массива leds (индекс 9 был нелегальным)
static const uint8_t RAY[8] = { 1, 2, 3, 4, 5, 8, 1, 2 };

// Маппинг внутреннего круга (146 LED на 8 долей)
//                                     доля1    доля2   доля3   доля4   доля5   доля6   доля7   доля8
static const uint16_t INNER_START[8] = { 16,     8,      0,     56,     47,     40,     33,     24 };
static const uint16_t INNER_COUNT[8] = {  8,     8,      8,      9,      9,      7,      7,      9 };

// Маппинг внешнего круга (600 LED на 8 долей, блок 8 БЕЗ внешнего круга!)
//                                     доля1    доля2   доля3   доля4   доля5   доля6   доля7   доля8
static const uint16_t OUTER_START[8]   = { 128,   106,     84,     62,     38,     18,      0,      0 };
static const uint16_t OUTER_COUNT[8]   = {  22,    22,     22,     22,     24,     20,     18,      0 };

// ============================================================================
// POWER CONTROL КОНФИГУРАЦИЯ (ВРЕМЕННО ОТКЛЮЧЕНО)
// ============================================================================
// #define RELAY_MAIN_POWER  19  // GPIO19 → Relay 10A (актуаторы + LED + контроллеры)
// #define POWER_BUTTON      4   // GPIO4  → Физическая кнопка Power ON/OFF (INPUT_PULLUP)
// bool mainPowerOn = false;

// ============================================================================
// MEGA SERIAL КОНФИГУРАЦИЯ
// ============================================================================
#define MEGA1_TX 25
#define MEGA1_RX 26
HardwareSerial Mega1Serial(1);

#define MEGA2_TX 16
#define MEGA2_RX 17
HardwareSerial Mega2Serial(2);

// ============================================================================
// СОСТОЯНИЕ БЛОКОВ
// ============================================================================
struct BlockState {
  bool isActive;
  unsigned long startTime;
  int duration;
};

BlockState blockStates[TOTAL_BLOCKS + 1];  // 0 не используется
int activeBlocksCount = 0;

// LED состояния - ОТДЕЛЬНО от актуаторов!
// LED включается при UP и остается ВКЛ пока не придет STOP или DOWN
bool ledStates[TOTAL_BLOCKS + 1];  // true = LED ВКЛ, false = LED ВЫКЛ
bool testMode = false;             // Тест-режим: loop не перезаписывает LED
unsigned long lastRequestTime = 0; // Время последнего веб-запроса для предотвращения конфликта прерываний

// Fade состояния для плавного нарастания (UP) и угасания (DOWN) LED
#define LED_FADE_DURATION_MS 4000  // 4 секунды — длительность fade-in и fade-out

struct FadeState {
  bool isActive;
  bool fadeIn;           // true = нарастание (UP), false = угасание (DOWN)
  unsigned long startTime;
  int duration;
};

FadeState fadeStates[TOTAL_BLOCKS + 1];  // 0 не используется

// Heartbeat
bool mega1Alive = false;
bool mega2Alive = false;
unsigned long lastHeartbeat = 0;

// ===== АВТОМАТИЧЕСКИЙ РЕЖИМ (STARTUP AUTOPLAY) =====
bool autoMode = true; // Запускается автоматически при старте
unsigned long lastAutoChange = 0;
const unsigned long AUTO_CHANGE_INTERVAL = 60000; // Смена каждые 60 секунд (1 минута)

const int AUTO_COLORS_COUNT = 5;
const CRGB AUTO_COLORS[AUTO_COLORS_COUNT] = {
  CRGB(180, 0, 255),    // Фиолетовый
  CRGB(255, 255, 255),  // Ақ (Белый)
  CRGB(0, 150, 255),    // Көк (Голубой)
  CRGB(128, 255, 128),  // Жасыл мятный (Бело-зеленый)
  CRGB(255, 180, 40)    // Теплый белый (желтоватый без розового оттенка)
};
int autoColorIndex = 0;

const int AUTO_EFFECTS_COUNT = 9;
const uint8_t AUTO_EFFECTS[AUTO_EFFECTS_COUNT] = {
  1,  // Pulse
  3,  // Chase
  4,  // Sparkle
  5,  // Wave
  7,  // Meteor
  9,  // ColorWipe
  10, // Twinkle
  11, // Ripple
  12  // Breathing
};
int autoEffectIndex = 0;

// ============================================================================
// WEB SERVER
// ============================================================================
WebServer server(80);

String mega1Response;
String mega2Response;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  RAMS CONTROLLER v3.2 PRODUCTION");
  Serial.println("  Actuators + LED Zones + Power Control");
  Serial.println("  DroneControl Style");
  Serial.println("========================================");

  // Power Control инициализация (ВРЕМЕННО ОТКЛЮЧЕНО)
  // pinMode(RELAY_MAIN_POWER, OUTPUT);
  // pinMode(POWER_BUTTON, INPUT_PULLUP);
  // digitalWrite(RELAY_MAIN_POWER, LOW);
  // Serial.println("[POWER] Relay initialized");
  // Serial.println("[POWER] GPIO19 = Main Power (OFF)");
  // Serial.println("[POWER] GPIO4  = Power Button (INPUT)");

  // LED инициализация (из svetdiod-project)
  // Инициализируем NeoPixelBus ленты
  strip0.Begin();
  strip1.Begin();
  strip2.Begin();
  strip3.Begin();
  strip4.Begin();
  strip5.Begin();
  strip7.Begin();
  strip8.Begin();

  // Очистка при старте
  strip0.Show();
  strip1.Show();
  strip2.Show();
  strip3.Show();
  strip4.Show();
  strip5.Show();
  strip7.Show();
  strip8.Show();

  memset(leds, 0, sizeof(leds));
  Serial.println("[LED] 8 NeoPixelBus hardware controllers initialized (9th combined sequentially)");
  Serial.println("[LED] Rays: 6x110 LED | Short: 48 LED (on GPIO18 after Ray 6) | Inner: 146 LED | Big: 600 LED");


  // Инициализация состояний блоков
  for (int i = 0; i <= TOTAL_BLOCKS; i++) {
    blockStates[i].isActive = false;
    blockStates[i].startTime = 0;
    blockStates[i].duration = 0;
    ledStates[i] = false;  // LED выключены
    fadeStates[i].isActive = false;  // Fade выключен
    fadeStates[i].fadeIn   = false;
    fadeStates[i].startTime = 0;
    fadeStates[i].duration = 0;
  }

  // Mega Serial
  Mega1Serial.begin(SERIAL_BAUD, SERIAL_8N1, MEGA1_RX, MEGA1_TX);
  Mega2Serial.begin(SERIAL_BAUD, SERIAL_8N1, MEGA2_RX, MEGA2_TX);
  // Устанавливаем таймаут 50мс чтобы readStringUntil не блокировал loop надолго
  Mega1Serial.setTimeout(50);
  Mega2Serial.setTimeout(50);
  Serial.println("[MEGA] Serial ready on GPIO25/26 and GPIO16/17 (timeout=50ms)");

  // WiFi - сначала сканируем доступные сети
  Serial.println("[WIFI] Scanning networks...");
  int n = WiFi.scanNetworks();
  Serial.printf("[WIFI] Found %d networks:\n", n);
  bool foundRamsWiFi = false;
  for (int i = 0; i < n; i++) {
    Serial.printf("  %d: %s (RSSI: %d, Channel: %d)\n", i+1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i));
    if (WiFi.SSID(i) == WIFI_SSID) {
      foundRamsWiFi = true;
      Serial.println("    ✅ Found Rams_WIFI!");
    }
  }

  if (!foundRamsWiFi) {
    Serial.println("[WIFI] ⚠️ Rams_WIFI not found in scan! Check router is ON and 2.4GHz");
  }

  // Пытаемся подключиться к роутеру (Station Mode)
  Serial.printf("[WIFI] Connecting to '%s' with password '%s'...\n", WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Ждем подключения 10 секунд
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    Serial.printf("[%d]", WiFi.status()); // Показать код статуса
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    // Успешно подключились к роутеру
    Serial.println("\n[WIFI] ✅ Connected to Rams_WIFI!");
    Serial.print("[WIFI] IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("[WIFI] Signal: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    // Не удалось подключиться - запускаем AP Mode (резервный)
    Serial.println("\n[WIFI] ❌ Failed to connect to Rams_WIFI");
    Serial.printf("[WIFI] Final status code: %d\n", WiFi.status());
    Serial.println("[WIFI] Status codes: 0=IDLE, 1=NO_SSID_AVAIL, 3=CONNECTED, 4=CONNECT_FAILED, 6=DISCONNECTED");
    Serial.println("[WIFI] Starting AP Mode (backup)...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("[WIFI] AP: ");
    Serial.print(AP_SSID);
    Serial.print(" IP: ");
    Serial.println(WiFi.softAPIP());
  }

  // Запуск mDNS responder
  if (MDNS.begin("rams-esp32")) {
    Serial.println("[WIFI] mDNS responder started: http://rams-esp32.local");
    MDNS.addService("http", "tcp", 80);
  } else {
    Serial.println("[WIFI] Error setting up MDNS responder!");
  }

  // ===== CORS ЗАГОЛОВКИ =====
  // Обработка OPTIONS preflight запросов для CORS
  server.on("/api/status", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);  // No Content
  });

  server.on("/api/block", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/stop", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/color", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/effect", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/testled", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  // Web Server - Отдаем красивый инженерный пульт со всеми кнопками и LED-тестером
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "identity");
    server.send_P(200, "text/html", LED_TESTER_HTML);
  });

  server.on("/led-tester.html", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "identity");
    server.send_P(200, "text/html", LED_TESTER_HTML);
  });

  // Browser-based OTA Web Update (Beautiful Dark interface)
  server.on("/update", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", 
      "<div style='background:#07070c;color:#f8fafc;font-family:sans-serif;min-height:100vh;display:flex;flex-direction:column;justify-content:center;align-items:center;margin:0;padding:20px;box-sizing:border-box;'>"
      "<div style='background:rgba(19,19,30,0.7);border:1px solid rgba(255,255,255,0.08);border-radius:16px;padding:30px;box-shadow:0 8px 32px rgba(0,0,0,0.37);text-align:center;max-width:400px;width:100%;'>"
      "<h2 style='background:linear-gradient(135deg,#00f2fe,#4facfe);-webkit-background-clip:text;-webkit-text-fill-color:transparent;margin-bottom:20px;'>⚡ RAMS OTA UPDATE</h2>"
      "<form method='POST' action='/update' enctype='multipart/form-data' style='display:flex;flex-direction:column;gap:16px;'>"
      "<input type='file' name='update' style='background:rgba(0,0,0,0.4);border:1px solid rgba(255,255,255,0.08);border-radius:8px;padding:12px;color:#f8fafc;outline:none;font-size:0.9rem;cursor:pointer;width:100%;'>"
      "<input type='submit' value='Upload Firmware' style='background:linear-gradient(135deg,#00f2fe,#4facfe);border:none;border-radius:8px;padding:12px 20px;color:#040814;font-weight:bold;cursor:pointer;font-size:0.95rem;box-shadow:0 4px 15px rgba(0,242,254,0.3);transition:transform 0.2s;width:100%;'>"
      "</form>"
      "</div>"
      "</div>"
    );
  });

  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", 
      "<div style='background:#07070c;color:#f8fafc;font-family:sans-serif;min-height:100vh;display:flex;flex-direction:column;justify-content:center;align-items:center;margin:0;padding:20px;box-sizing:border-box;'>"
      "<div style='background:rgba(19,19,30,0.7);border:1px solid " + String(Update.hasError() ? "rgba(239,68,68,0.3)" : "rgba(16,185,129,0.3)") + ";border-radius:16px;padding:30px;box-shadow:0 8px 32px rgba(0,0,0,0.37);text-align:center;max-width:400px;width:100%;'>"
      + String(Update.hasError() ? 
        "<h2 style='color:#ef4444;margin-bottom:14px;'>❌ UPDATE FAILED</h2><p style='color:#94a3b8;font-size:0.9rem;'>" + String(Update.errorString()) + "</p>" : 
        "<h2 style='color:#10b981;margin-bottom:14px;'>✅ UPDATE SUCCESSFUL</h2><p style='color:#94a3b8;font-size:0.9rem;'>ESP32 is rebooting. Please wait 5 seconds and reload the main page.</p>") +
      "</div>"
      "</div>"
    );
    delay(1000);
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // Start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.on("/api/reboot", HTTP_ANY, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    if (server.method() == HTTP_OPTIONS) {
      server.send(204);
      return;
    }
    server.send(200, "text/plain", "Rebooting ESP32...");
    delay(500);
    ESP.restart();
  });

  server.on("/api/status", HTTP_GET, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    String json = "{\"active\":" + String(activeBlocksCount) + 
                  ",\"mega1Alive\":" + String(mega1Alive ? "true" : "false") +
                  ",\"mega2Alive\":" + String(mega2Alive ? "true" : "false") +
                  ",\"blocks\":[";
    bool first = true;
    for (int i = 1; i <= TOTAL_BLOCKS; i++) {
      if (blockStates[i].isActive) {
        if (!first) json += ",";
        json += String(i);
        first = false;
      }
    }
    json += "]}";
    server.send(200, "application/json", json);
  });

  server.on("/api/block", HTTP_POST, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    int blockNum = server.arg("num").toInt();
    String action = server.arg("action");
    int duration = server.arg("duration").toInt();

    if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
      server.send(400, "text/plain", "ERROR:Invalid block");
      return;
    }

    if (duration <= 0) {
      if (action == "UP") {
        duration = 4000;
      } else if (action == "DOWN") {
        duration = 5000;
      } else {
        duration = DEFAULT_DURATION_MS;
      }
    }

    // Лимит активных блоков
    if ((action == "UP" || action == "DOWN") && activeBlocksCount >= MAX_ACTIVE_BLOCKS && !blockStates[blockNum].isActive) {
      server.send(429, "text/plain", "ERROR:Max active");
      return;
    }

    // Формат команды: BLOCK:5:UP:10000
    String cmd = "BLOCK:" + String(blockNum) + ":" + action + ":" + String(duration);

    // Роутинг через общий конфиг
    const BlockConfig* cfg = getBlockConfig(blockNum);
    if (cfg->megaNum == 1) {
      Mega1Serial.println(cmd);
      Serial.println("[MEGA1 TX] " + cmd);
    } else {
      Mega2Serial.println(cmd);
      Serial.println("[MEGA2 TX] " + cmd);
    }

    // Обновить состояние
    blockStates[blockNum].isActive = (action != "STOP");
    blockStates[blockNum].startTime = millis();
    blockStates[blockNum].duration = duration;

    // Пересчитать активные
    activeBlocksCount = 0;
    for (int i = 1; i <= TOTAL_BLOCKS; i++) {
      if (blockStates[i].isActive) activeBlocksCount++;
    }

    // ===== LED УПРАВЛЕНИЕ =====
    if (action == "UP") {
      // Включить LED зону для этого блока
      ledStates[blockNum] = true;   // ✅ LED ВКЛ
      lightUpBlock(blockNum);
    } else if (action == "DOWN") {
      // Fade LED зоны
      ledStates[blockNum] = false;  // ❌ LED ВЫКЛ
      fadeBlock(blockNum);
    } else if (action == "STOP") {
      // Выключить LED зону
      ledStates[blockNum] = false;  // ❌ LED ВЫКЛ
      turnOffBlock(blockNum);
    }

    Serial.printf("[BLOCK] %d %s %dms (active: %d/%d)\n", blockNum, action.c_str(), duration, activeBlocksCount, MAX_ACTIVE_BLOCKS);
    server.send(200, "text/plain", "OK");
  });

  // Вспомогательный API для ручного тестирования каждого отдельного актуатора
  // Пример: /api/actuator?block=3&act=1&action=UP&duration=4000
  // Пример: /api/actuator?block=3&act=2&action=DOWN&duration=4000
  // Пример: /api/actuator?block=3&act=1&action=STOP
  server.on("/api/actuator", HTTP_ANY, []() {
    lastRequestTime = millis();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    if (server.method() == HTTP_OPTIONS) {
      server.send(204);
      return;
    }

    int blockNum = server.arg("block").toInt();
    int actNum = server.arg("act").toInt(); // 1, 2 или 3
    String action = server.arg("action");
    int duration = server.arg("duration").toInt();

    if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
      server.send(400, "text/plain", "ERROR:Invalid block");
      return;
    }
    if (actNum < 1 || actNum > 3) {
      server.send(400, "text/plain", "ERROR:Invalid actuator index (1-3)");
      return;
    }
    if (duration <= 0) {
      if (action == "UP") {
        duration = 4000;
      } else if (action == "DOWN") {
        duration = 5000;
      } else {
        duration = DEFAULT_DURATION_MS;
      }
    }

    // Формат команды: ACTUATOR:3:2:UP:4000
    String cmd = "ACTUATOR:" + String(blockNum) + ":" + String(actNum) + ":" + action + ":" + String(duration);

    // Роутинг через общий конфиг
    const BlockConfig* cfg = getBlockConfig(blockNum);
    if (cfg->megaNum == 1) {
      Mega1Serial.println(cmd);
      Serial.println("[MEGA1 TX MANUAL ACT] " + cmd);
    } else {
      Mega2Serial.println(cmd);
      Serial.println("[MEGA2 TX MANUAL ACT] " + cmd);
    }

    Serial.printf("[MANUAL ACTUATOR] Block %d Act %d %s %dms\n", blockNum, actNum, action.c_str(), duration);
    server.send(200, "text/plain", "OK");
  });


  server.on("/api/stop", HTTP_POST, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    Serial.println("[API] STOP ALL");

    Mega1Serial.println("ALL:STOP");
    Mega2Serial.println("ALL:STOP");

    for (int i = 1; i <= TOTAL_BLOCKS; i++) {
      blockStates[i].isActive = false;
      ledStates[i] = false;  // ❌ Выключить все LED
      fadeStates[i].isActive = false;  // ❌ Отменить fade анимации
    }
    activeBlocksCount = 0;

    memset(leds, 0, sizeof(leds));
    showLEDs();

    server.send(200, "text/plain", "OK");
  });

  server.on("/api/color", HTTP_ANY, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    // Получить RGB параметры из query string
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();

    // Валидация (0-255)
    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;

    // Обновить глобальные переменные
    targetR = r;
    targetG = g;
    targetB = b;
    testMode = false;  // Выходим из тест-режима
    autoMode = false;  // Отключаем авторежим при ручном выборе цвета

    Serial.printf("[API] LED color set to RGB(%d, %d, %d)\n", r, g, b);

    server.send(200, "text/plain", "OK");
  });

  // Эндпоинт для индивидуальной проверки диодов
  // Пример: /api/testled?strip=7&led=144&r=255&g=0&b=0
  server.on("/api/testled", HTTP_ANY, []() {
    lastRequestTime = millis();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    if (!server.hasArg("strip")) {
      server.send(400, "text/plain", "ERROR: strip is required");
      return;
    }

    int stripIdx = server.arg("strip").toInt();
    int r = server.hasArg("r") ? server.arg("r").toInt() : 255;
    int g = server.hasArg("g") ? server.arg("g").toInt() : 255;
    int b = server.hasArg("b") ? server.arg("b").toInt() : 255;

    if (stripIdx < 0 || stripIdx >= NUM_STRIPS) {
      server.send(400, "text/plain", "ERROR: Invalid strip index");
      return;
    }

    testMode = true; // Блокируем авто-анимации

    if (server.hasArg("led")) {
      int ledIdx = server.arg("led").toInt();
      if (ledIdx < 0 || ledIdx >= MAX_LEDS) {
        server.send(400, "text/plain", "ERROR: Invalid led index");
        return;
      }
      leds[stripIdx][ledIdx] = CRGB(r, g, b);
      Serial.printf("[API TESTLED] Strip %d LED %d set to RGB(%d,%d,%d)\n", stripIdx, ledIdx, r, g, b);
    } else {
      // Если индекс светодиода не передан, заливаем всю ленту целиком
      for (int i = 0; i < PIN_LEDS[stripIdx]; i++) {
        leds[stripIdx][i] = CRGB(r, g, b);
      }
      Serial.printf("[API TESTLED] Filled entire strip %d with RGB(%d,%d,%d)\n", stripIdx, r, g, b);
    }

    showLEDs();
    server.send(200, "text/plain", "OK");
  });

  server.on("/api/effect", HTTP_ANY, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    // Получить ID эффекта и скорость (поддерживаем id/v и speed/spd)
    int id = server.hasArg("id") ? server.arg("id").toInt() : (server.hasArg("v") ? server.arg("v").toInt() : 0);
    int speed = server.hasArg("speed") ? server.arg("speed").toInt() : (server.hasArg("spd") ? server.arg("spd").toInt() : -1);

    // Валидация
    if (id < 0) id = 0;
    if (id > 12) id = 12;

    if (speed >= 0 && speed <= 255) {
      gSpd = speed;
    }

    // Обновить эффект
    testMode = false;  // Выходим из тест-режима
    autoMode = false;  // Отключаем авторежим при ручном выборе эффекта

    // Очистить heat buffer при переключении на Fire
    if (id == 6) {
      memset(heat, 0, sizeof(heat));
    }

    Serial.printf("[API] LED effect set to %d (speed: %d)\n", id, gSpd);

    server.send(200, "text/plain", "OK");
  });

  server.on("/api/auto", HTTP_ANY, []() {
    lastRequestTime = millis();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    if (server.method() == HTTP_OPTIONS) {
      server.send(204);
      return;
    }

    autoMode = true;
    testMode = false;

    // Сразу применить первый эффект и цвет из списка, чтобы не ждать минуту
    CRGB nextColor = AUTO_COLORS[autoColorIndex];
    targetR = nextColor.r;
    targetG = nextColor.g;
    targetB = nextColor.b;
    gFx = AUTO_EFFECTS[autoEffectIndex];

    // Сдвинуть индексы
    autoColorIndex = (autoColorIndex + 1) % AUTO_COLORS_COUNT;
    autoEffectIndex = (autoEffectIndex + 1) % AUTO_EFFECTS_COUNT;
    lastAutoChange = millis();

    Serial.printf("[API] AutoMode enabled: Effect %d, target color RGB(%d,%d,%d)\n", gFx, targetR, targetG, targetB);
    server.send(200, "text/plain", "OK");
  });

  // OPTIONS для /api/bri
  server.on("/api/bri", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/bri", HTTP_ANY, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    int v = server.arg("v").toInt();
    if (v < 0) v = 0;
    if (v > 255) v = 255;

    gBri = v;
    showLEDs();
    testMode = false;  // Выходим из тест-режима

    Serial.printf("[API] LED brightness set to %d\n", gBri);
    server.send(200, "text/plain", "OK");
  });

  // OPTIONS для /api/spd
  server.on("/api/spd", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/spd", HTTP_ANY, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    int v = server.arg("v").toInt();
    if (v < 0) v = 0;
    if (v > 255) v = 255;

    gSpd = v;

    Serial.printf("[API] LED speed set to %d\n", gSpd);
    server.send(200, "text/plain", "OK");
  });

  // OPTIONS для /api/zones
  server.on("/api/zones", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.on("/api/zones", HTTP_POST, []() {
    lastRequestTime = millis();
    // CORS заголовки
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    // Зоны пока не используются, но эндпоинт нужен для совместимости
    int m = server.arg("m").toInt();

    Serial.printf("[API] LED zones mask set to %d (not implemented)\n", m);
    server.send(200, "text/plain", "OK");
  });

  // ===== POWER CONTROL API (ВРЕМЕННО ОТКЛЮЧЕНО) =====
  /*
  server.on("/api/power/on", HTTP_POST, []() {
    Serial.println("[POWER] Main power ON");
    digitalWrite(RELAY_MAIN_POWER, HIGH);
    mainPowerOn = true;
    server.send(200, "text/plain", "Power ON");
  });

  server.on("/api/power/off", HTTP_POST, []() {
    Serial.println("[POWER] Main power OFF - stopping all blocks first");
    Mega1Serial.println("ALL:STOP");
    Mega2Serial.println("ALL:STOP");
    memset(leds, 0, sizeof(leds));
    showLEDs();
    delay(500);
    digitalWrite(RELAY_MAIN_POWER, LOW);
    mainPowerOn = false;
    for (int i = 1; i <= TOTAL_BLOCKS; i++) {
      blockStates[i].isActive = false;
    }
    activeBlocksCount = 0;
    server.send(200, "text/plain", "Power OFF");
  });

  server.on("/api/power/status", HTTP_GET, []() {
    String json = "{\"power\":" + String(mainPowerOn ? "true" : "false") + "}";
    server.send(200, "application/json", json);
  });
  */

  // ===== TEST API (для отладки лент) =====
  // GET /api/test?strip=0&from=0&to=32&r=255&g=0&b=0
  // strip  — индекс ленты (0-9)
  // from   — начальный адрес LED (0-149)
  // to     — конечный адрес LED (0-149)
  // r,g,b  — цвет (0-255)
  server.on("/api/test", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });
  server.on("/api/test", HTTP_GET, []() {
    lastRequestTime = millis();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    String stripArg = server.arg("strip");
    bool allStrips = (stripArg == "all" || stripArg == "-1");
    int strip = allStrips ? 0 : stripArg.toInt();
    int from  = server.arg("from").toInt();
    int to    = server.arg("to").toInt();
    int r     = server.arg("r").toInt();
    int g     = server.arg("g").toInt();
    int b     = server.arg("b").toInt();

    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;

    // Validate if not allStrips
    if (!allStrips && (strip < 0 || strip >= NUM_STRIPS)) {
      server.send(400, "text/plain", "ERROR: strip must be 0-8 or 'all'");
      return;
    }

    // Включаем тест-режим чтобы loop() не перезаписал результат
    testMode = true;

    bool noclear = server.arg("noclear") == "1";
    if (!noclear) {
      memset(leds, 0, sizeof(leds));
    }

    if (allStrips) {
      for (int s = 0; s < NUM_STRIPS; s++) {
        uint16_t maxLed = PIN_LEDS[s];
        int s_from = server.hasArg("from") ? server.arg("from").toInt() : 0;
        int s_to = server.hasArg("to") ? server.arg("to").toInt() : (maxLed - 1);
        if (s_from < 0) s_from = 0;
        if (s_to >= (int)maxLed) s_to = maxLed - 1;
        for (int j = s_from; j <= s_to; j++) {
          leds[s][j] = CRGB(r, g, b);
        }
      }
    } else {
      uint16_t maxLed = PIN_LEDS[strip];
      if (from < 0)         from = 0;
      if (to   < 0)         to   = 0;
      if (from >= (int)maxLed) from = maxLed - 1;
      if (to   >= (int)maxLed) to   = maxLed - 1;
      if (from > to) { int tmp = from; from = to; to = tmp; }
      for (int j = from; j <= to; j++) {
        leds[strip][j] = CRGB(r, g, b);
      }
    }
    showLEDs();
    delay(1);  // WS2815 latch: минимум 280µs тишины чтобы кадр залатчился

    if (allStrips) {
      Serial.printf("[TEST] ALL STRIPS RGB(%d,%d,%d)\n", r, g, b);
      server.send(200, "application/json", "{\"ok\":true,\"strip\":\"all\"}");
    } else {
      uint16_t maxLed = PIN_LEDS[strip];
      Serial.printf("[TEST] strip=%d gpio=%d addr=%d-%d RGB(%d,%d,%d)\n",
                    strip, PIN_GPIO[strip], from, to, r, g, b);
      String json = "{\"ok\":true,\"strip\":" + String(strip) +
                    ",\"gpio\":" + String(PIN_GPIO[strip]) +
                    ",\"from\":" + String(from) +
                    ",\"to\":"   + String(to) +
                    ",\"leds\":" + String(maxLed) + "}";
      server.send(200, "application/json", json);
    }
  });

  // GET /api/clear — выключить все ленты
  server.on("/api/clear", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });
  server.on("/api/clear", HTTP_GET, []() {
    lastRequestTime = millis();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    testMode = false;  // Выходим из тест-режима
    memset(leds, 0, sizeof(leds));
    showLEDs();
    Serial.println("[TEST] All LEDs cleared");
    server.send(200, "application/json", "{\"ok\":true}");
  });

  // GET /api/info — информация о конфигурации лент
  server.on("/api/info", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });
  server.on("/api/info", HTTP_GET, []() {
    lastRequestTime = millis();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    String json = "{\"strips\":[";
    const char* names[] = {"Big Circle","Ray 1","Ray 2","Ray 3","Ray 4","Ray 5","Short Line","Inner Circle","Ray 6"};
    for (int i = 0; i < NUM_STRIPS; i++) {
      if (i > 0) json += ",";
      json += "{\"idx\":" + String(i) +
              ",\"gpio\":" + String(PIN_GPIO[i]) +
              ",\"leds\":" + String(PIN_LEDS[i]) +
              ",\"name\":\"" + names[i] + "\"}";
    }
    json += "]}";
    server.send(200, "application/json", json);
  });

  server.onNotFound([]() {
    // Add CORS headers to avoid preflight issues blocking custom route diagnostics
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    
    if (server.method() == HTTP_OPTIONS) {
      server.send(204);
      return;
    }

    String methodStr = "UNKNOWN";
    if (server.method() == HTTP_GET) methodStr = "GET";
    else if (server.method() == HTTP_POST) methodStr = "POST";
    else if (server.method() == HTTP_OPTIONS) methodStr = "OPTIONS";
    else if (server.method() == HTTP_PUT) methodStr = "PUT";
    else if (server.method() == HTTP_DELETE) methodStr = "DELETE";

    Serial.printf("[SERVER] NOT FOUND: %s %s\n", methodStr.c_str(), server.uri().c_str());
    server.send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("[SERVER] Started on port 80");

  // ===== OTA (Over-The-Air) Setup =====
  ArduinoOTA.setHostname("RAMS-ESP32");
  ArduinoOTA.setPassword("rams2026");

  ArduinoOTA.onStart([]() {
    Serial.println("[OTA] Update Start");
    // Гасим все светодиоды для безопасности перед прошивкой
    for (int s = 0; s < NUM_STRIPS; s++) {
      memset(leds[s], 0, MAX_LEDS * sizeof(CRGB));
    }
    showLEDs();
    
    // Отправляем команду СТОП на обе Mega платы
    Mega1Serial.println("ALL:STOP");
    Mega2Serial.println("ALL:STOP");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\n[OTA] Update Complete!");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA] Error[%u]: ", error);
    if      (error == OTA_AUTH_ERROR)    Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)     Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("[OTA] Ready");

  // Инициализируем стартовый эффект и цвет для авторежима сразу при загрузке
  if (autoMode) {
    CRGB startColor = AUTO_COLORS[autoColorIndex];
    gR = startColor.r;
    gG = startColor.g;
    gB = startColor.b;
    targetR = startColor.r;
    targetG = startColor.g;
    targetB = startColor.b;
    gFx = AUTO_EFFECTS[autoEffectIndex];
    
    Serial.printf("[BOOT] Started in AutoMode: Effect %d, Color RGB(%d,%d,%d)\n", gFx, gR, gG, gB);
    
    autoColorIndex = (autoColorIndex + 1) % AUTO_COLORS_COUNT;
    autoEffectIndex = (autoEffectIndex + 1) % AUTO_EFFECTS_COUNT;
    lastAutoChange = millis();
  }

  Serial.println("[READY] System initialized!\n");
}

// ============================================================================
// LED УПРАВЛЕНИЕ ДЛЯ БЛОКОВ
// ============================================================================

void setSegmentColor(uint8_t stripIdx, uint16_t start, uint16_t end, CRGB color) {
  if (stripIdx >= NUM_STRIPS) return;
  uint16_t limit = PIN_LEDS[stripIdx];
  for (uint16_t i = start; i <= end && i < limit; i++) {
    leds[stripIdx][i] = color;
  }
}

void updateBlockLEDs(int blockNum, CRGB color) {
  switch (blockNum) {
    case 1: // NOMAD
      setSegmentColor(8, 60, 109, color);
      setSegmentColor(6, 0, 48, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 29, 102, color);
      }
      break;
    case 2: // GRANDE VIE
      setSegmentColor(6, 0, 48, color);
      setSegmentColor(2, 60, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 0, 28, color);
        setSegmentColor(0, 496, 530, color);
      }
      break;
    case 3: // RAMS CITY ALMATY
      setSegmentColor(8, 0, 59, color);
      setSegmentColor(2, 0, 59, color);
      setSegmentColor(7, 0, 18, color);
      setSegmentColor(7, 128, 145, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 0, 102, color);
        setSegmentColor(0, 496, 530, color);
      }
      break;
    case 4: // KERUEN CITY
      setSegmentColor(2, 60, 109, color);
      setSegmentColor(5, 60, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 430, 495, color);
      }
      break;
    case 5: // HYATT REGENCY
      setSegmentColor(5, 0, 59, color);
      setSegmentColor(2, 0, 59, color);
      setSegmentColor(7, 109, 127, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 430, 495, color);
      }
      break;
    case 6: // RAMS GARDEN BAHCELIEVLER
      setSegmentColor(5, 60, 109, color);
      setSegmentColor(3, 60, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 363, 429, color);
      }
      break;
    case 7: // BAITEREK SCHOOL
      setSegmentColor(3, 0, 59, color);
      setSegmentColor(5, 0, 59, color);
      setSegmentColor(7, 91, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 363, 429, color);
      }
      break;
    case 8: // RAMS RESORT BODRUM
      setSegmentColor(3, 60, 109, color);
      setSegmentColor(4, 60, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 303, 363, color);
      }
      break;
    case 9: // RAMS CITY GAZIANTEP
      setSegmentColor(3, 0, 59, color);
      setSegmentColor(4, 0, 59, color);
      setSegmentColor(7, 74, 91, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 303, 363, color);
      }
      break;
    case 10: // RAMS CITY HALIC 2
      setSegmentColor(4, 60, 109, color);
      setSegmentColor(1, 60, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 225, 303, color);
      }
      break;
    case 11: // RAMS CITY HALIC 1
      setSegmentColor(7, 53, 74, color);
      setSegmentColor(1, 0, 59, color);
      setSegmentColor(4, 0, 59, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 225, 303, color);
      }
      break;
    case 12: // PARK HOUSE MASLAK
      setSegmentColor(1, 60, 109, color);
      setSegmentColor(8, 60, 109, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 103, 225, color);
      }
      break;
    case 13: // SAKURA
      setSegmentColor(1, 0, 59, color);
      setSegmentColor(8, 0, 59, color);
      setSegmentColor(7, 19, 52, color);
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS) {
        setSegmentColor(0, 103, 225, color);
      }
      break;
    default:
      break;
  }
}

/**
 * Проверяет, принадлежит ли конкретный светодиод (stripIdx, pixelIdx) к LED зоне указанного блока.
 */
bool isPixelInBlock(int blockNum, uint8_t stripIdx, uint16_t pixelIdx) {
  switch (blockNum) {
    case 1: // NOMAD
      if (stripIdx == 8 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (stripIdx == 6 && pixelIdx >= 0 && pixelIdx <= 48) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 29 && pixelIdx <= 102) return true;
      break;
    case 2: // GRANDE VIE
      if (stripIdx == 6 && pixelIdx >= 0 && pixelIdx <= 48) return true;
      if (stripIdx == 2 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && ( (pixelIdx >= 0 && pixelIdx <= 28) || (pixelIdx >= 496 && pixelIdx <= 530) )) return true;
      break;
    case 3: // RAMS CITY ALMATY
      if (stripIdx == 8 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 2 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 7 && ( (pixelIdx >= 0 && pixelIdx <= 18) || (pixelIdx >= 128 && pixelIdx <= 145) )) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && ( (pixelIdx >= 0 && pixelIdx <= 102) || (pixelIdx >= 496 && pixelIdx <= 530) )) return true;
      break;
    case 4: // KERUEN CITY
      if (stripIdx == 2 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (stripIdx == 5 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 430 && pixelIdx <= 495) return true;
      break;
    case 5: // HYATT REGENCY
      if (stripIdx == 5 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 2 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 7 && pixelIdx >= 109 && pixelIdx <= 127) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 430 && pixelIdx <= 495) return true;
      break;
    case 6: // RAMS GARDEN BAHCELIEVLER
      if (stripIdx == 5 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (stripIdx == 3 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 363 && pixelIdx <= 429) return true;
      break;
    case 7: // BAITEREK SCHOOL
      if (stripIdx == 3 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 5 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 7 && pixelIdx >= 91 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 363 && pixelIdx <= 429) return true;
      break;
    case 8: // RAMS RESORT BODRUM
      if (stripIdx == 3 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (stripIdx == 4 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 303 && pixelIdx <= 363) return true;
      break;
    case 9: // RAMS CITY GAZIANTEP
      if (stripIdx == 3 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 4 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 7 && pixelIdx >= 74 && pixelIdx <= 91) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 303 && pixelIdx <= 363) return true;
      break;
    case 10: // RAMS CITY HALIC 2
      if (stripIdx == 4 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (stripIdx == 1 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 225 && pixelIdx <= 303) return true;
      break;
    case 11: // RAMS CITY HALIC 1
      if (stripIdx == 7 && pixelIdx >= 53 && pixelIdx <= 74) return true;
      if (stripIdx == 1 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 4 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 225 && pixelIdx <= 303) return true;
      break;
    case 12: // PARK HOUSE MASLAK
      if (stripIdx == 1 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (stripIdx == 8 && pixelIdx >= 60 && pixelIdx <= 109) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 103 && pixelIdx <= 225) return true;
      break;
    case 13: // SAKURA
      if (stripIdx == 1 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 8 && pixelIdx >= 0 && pixelIdx <= 59) return true;
      if (stripIdx == 7 && pixelIdx >= 19 && pixelIdx <= 52) return true;
      if (ENABLE_BIG_CIRCLE_ON_BLOCKS && stripIdx == 0 && pixelIdx >= 103 && pixelIdx <= 225) return true;
      break;
  }
  return false;
}

/**
 * Плавное нарастание LED зоны (UP) — 4 секунды от 0 до макс. яркости
 */
void lightUpBlock(int blockNum) {
  if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
    Serial.printf("[LED] Block %d - invalid block number\n", blockNum);
    return;
  }
  // Отменить любой текущий fade для этого блока
  fadeStates[blockNum].isActive = false;

  // Запустить fade-IN анимацию
  fadeStates[blockNum].isActive  = true;
  fadeStates[blockNum].fadeIn    = true;
  fadeStates[blockNum].startTime = millis();
  fadeStates[blockNum].duration  = LED_FADE_DURATION_MS;

  Serial.printf("[LED] Block %d FADE-IN started (%dms)\n", blockNum, LED_FADE_DURATION_MS);
}

/**
 * Плавное угасание LED зоны (DOWN) — 4 секунды от макс. яркости до 0
 */
void fadeBlock(int blockNum) {
  if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
    return;
  }
  // Отменить любой текущий fade для этого блока
  fadeStates[blockNum].isActive = false;

  // Запустить fade-OUT анимацию
  fadeStates[blockNum].isActive  = true;
  fadeStates[blockNum].fadeIn    = false;
  fadeStates[blockNum].startTime = millis();
  fadeStates[blockNum].duration  = LED_FADE_DURATION_MS;

  Serial.printf("[LED] Block %d FADE-OUT started (%dms)\n", blockNum, LED_FADE_DURATION_MS);
}

/**
 * Мгновенное выключение LED зоны для блока (STOP)
 */
void turnOffBlock(int blockNum) {
  if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
    return;
  }
  fadeStates[blockNum].isActive = false;  // Отменить любой fade
  updateBlockLEDs(blockNum, CRGB::Black);
  showLEDs();
  Serial.printf("[LED] Block %d OFF\n", blockNum);
}

// ============================================================================
// LED ЭФФЕКТЫ (из svetdiod-project)
// ============================================================================

/**
 * Эффект 1: Пульсация
 */
void fxPulse() {
  CRGB c(gR, gG, gB);
  c.nscale8(beatsin8(map(gSpd, 0, 255, 8, 60), 15, 255));

  bool anyActiveLed = false;
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (ledStates[i]) {
      anyActiveLed = true;
      break;
    }
  }

  if (!anyActiveLed) {
    for (int s = 0; s < NUM_STRIPS; s++) {
      if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
      fill_solid(leds[s], PIN_LEDS[s], c);
    }
    return;
  }

  // Очищаем все светодиоды перед выводом активных зон
  for (int s = 0; s < NUM_STRIPS; s++) {
    fill_solid(leds[s], PIN_LEDS[s], CRGB::Black);
  }

  // Применяем пульсацию только к светодиодам активных блоков (без fade-out)
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (!ledStates[i]) continue;
    if (fadeStates[i].isActive) continue; // Пропускаем те, что в fade
    updateBlockLEDs(i, c);
  }
}

/**
 * Эффект 2: Радуга
 */
void fxRainbow() {
  static uint8_t hue = 0;
  hue += map(gSpd, 0, 255, 1, 5);

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    for (uint16_t j = 0; j < PIN_LEDS[s]; j++) {
      leds[s][j] = CHSV(hue + j * 3 + s * 25, 255, 255);
    }
  }

  bool anyActiveLed = false;
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (ledStates[i]) {
      anyActiveLed = true;
      break;
    }
  }

  if (!anyActiveLed) {
    return; // Если нет активных блоков, радуга идет по всем лентам
  }

  // Применить маску активных блоков
  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    for (uint16_t j = 0; j < PIN_LEDS[s]; j++) {
      bool inActiveBlock = false;
      for (int i = 1; i <= TOTAL_BLOCKS; i++) {
        if (!ledStates[i]) continue;
        if (fadeStates[i].isActive) continue; // Пропускаем те, что в fade

        if (isPixelInBlock(i, s, j)) {
          inActiveBlock = true;
          break;
        }
      }
      if (!inActiveBlock) {
        leds[s][j] = CRGB::Black;
      }
    }
  }
}

/**
 * Эффект 3: Бегущая точка
 */
void fxChase() {
  static uint16_t pos = 0;
  static uint32_t last = 0;
  uint32_t now = millis();

  if (now - last >= (uint32_t)map(gSpd, 0, 255, 150, 20)) {
    last = now;
    pos++;
  }

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    uint16_t n = PIN_LEDS[s];
    fill_solid(leds[s], n, CRGB::Black);
    uint16_t p = pos % n;
    leds[s][p] = CRGB(gR, gG, gB);

    // Хвост
    for (int t = 1; t <= 6 && t < (int)n; t++) {
      int tp = ((int)p - t + (int)n) % (int)n;
      leds[s][tp].setRGB(gR, gG, gB);
      leds[s][tp].nscale8(255 - t * 40);
    }
  }
}

/**
 * Эффект 4: Искры
 */
void fxSparkle() {
  uint8_t rate = map(gSpd, 0, 255, 30, 180);

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    for (uint16_t j = 0; j < PIN_LEDS[s]; j++) {
      leds[s][j].nscale8(170);
    }
    if (random8() < rate) {
      uint16_t p = random16() % PIN_LEDS[s];
      leds[s][p] = CRGB(gR, gG, gB);
    }
  }
}

/**
 * Эффект 5: Волна
 */
void fxWave() {
  static uint16_t phase = 0;
  phase += map(gSpd, 0, 255, 50, 600);

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    for (uint16_t j = 0; j < PIN_LEDS[s]; j++) {
      leds[s][j].setRGB(gR, gG, gB);
      leds[s][j].nscale8(sin8((uint8_t)(j * 255 / PIN_LEDS[s]) + (phase >> 8) + s * 40));
    }
  }
}

/**
 * Эффект 6: Огонь
 */
void fxFire() {
  uint8_t cool = map(gSpd, 0, 255, 20, 80);
  uint8_t spark = map(gSpd, 0, 255, 60, 200);

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    uint16_t n = PIN_LEDS[s];

    // Cooling
    for (uint16_t j = 0; j < n; j++) {
      heat[s][j] = qsub8(heat[s][j], random8(0, ((cool * 10) / n) + 2));
    }

    // Heat diffusion
    for (int j = n - 1; j >= 2; j--) {
      heat[s][j] = ((uint16_t)heat[s][j-1] + heat[s][j-2] + heat[s][j-2]) / 3;
    }

    // Sparks
    if (random8() < spark) {
      uint8_t y = random8(min((uint16_t)4, n));
      heat[s][y] = qadd8(heat[s][y], random8(160, 255));
    }

    // Convert heat to color
    for (uint16_t j = 0; j < n; j++) {
      leds[s][j] = HeatColor(heat[s][j]);
    }
  }
}

/**
 * Эффект 7: Метеор
 */
void fxMeteor() {
  static uint16_t pos = 0;
  static uint32_t last = 0;
  uint32_t now = millis();

  if (now - last >= (uint32_t)map(gSpd, 0, 255, 40, 2)) {
    last = now;
    pos++;
  }

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    uint16_t n = PIN_LEDS[s];

    // Fade
    for (uint16_t j = 0; j < n; j++) {
      if (random8() < 100) leds[s][j].nscale8(140);
    }

    // Meteor head
    uint16_t head = pos % (n * 2);
    if (head < n) {
      leds[s][head] = CRGB(gR, gG, gB);
      if (head > 0) {
        leds[s][head-1].setRGB(gR, gG, gB);
        leds[s][head-1].nscale8(180);
      }
    }
  }
}

/**
 * Эффект 8: Строб (Strobe)
 */
void fxStrobe() {
  static bool strobeOn = false;
  static uint32_t lastStrobe = 0;
  uint32_t now = millis();
  uint32_t period = map(gSpd, 0, 255, 200, 30);

  if (now - lastStrobe >= period) {
    lastStrobe = now;
    strobeOn = !strobeOn;
  }

  CRGB c = strobeOn ? CRGB(gR, gG, gB) : CRGB::Black;
  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    fill_solid(leds[s], PIN_LEDS[s], c);
  }
}

/**
 * Эффект 9: Цветное заполнение (Color Wipe)
 */
void fxColorWipe() {
  static uint32_t pos = 0;
  static uint32_t last = 0;
  uint32_t now = millis();

  if (now - last >= (uint32_t)map(gSpd, 0, 255, 50, 5)) {
    last = now;
    pos++;
  }

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    uint16_t n = PIN_LEDS[s];
    uint32_t cur = pos % (n * 2);
    for (uint16_t j = 0; j < n; j++) {
      leds[s][j] = (j <= cur % n) ? CRGB(gR, gG, gB) : CRGB::Black;
    }
  }
}

/**
 * Эффект 10: Мерцание звёзд (Twinkle)
 */
void fxTwinkle() {
  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    for (uint16_t j = 0; j < PIN_LEDS[s]; j++) {
      leds[s][j].nscale8(230);
    }
  }
  uint8_t count = map(gSpd, 0, 255, 1, 8);
  for (int k = 0; k < count; k++) {
    int s = random8(NUM_STRIPS);
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    uint16_t p = random16() % PIN_LEDS[s];
    leds[s][p] = CRGB(gR, gG, gB);
    leds[s][p].nscale8(random8(100, 255));
  }
}

/**
 * Эффект 11: Рябь — волна из центра
 */
void fxRipple() {
  static uint16_t phase = 0;
  phase += map(gSpd, 0, 255, 100, 2000);

  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    uint16_t n = PIN_LEDS[s];
    uint16_t center = n / 2;
    for (uint16_t j = 0; j < n; j++) {
      int dist = abs((int)j - (int)center);
      uint8_t wave = sin8((uint8_t)(dist * 255 / (center + 1)) + (uint8_t)(phase >> 8));
      leds[s][j].setRGB(gR, gG, gB);
      leds[s][j].nscale8(wave);
    }
  }
}

/**
 * Эффект 12: Дыхание (Breathing) — плавный вдох/выдох всех лент
 */
void fxBreathing() {
  uint8_t bpm = map(gSpd, 0, 255, 5, 30);
  uint8_t brightness = beatsin8(bpm, 5, 255);
  CRGB c(gR, gG, gB);
  c.nscale8(brightness);
  for (int s = 0; s < NUM_STRIPS; s++) {
    if (s == S_OUTER && !ENABLE_BIG_CIRCLE_ON_EFFECTS) continue;
    fill_solid(leds[s], PIN_LEDS[s], c);
  }
}

// ============================================================================
// MAIN LOOP - СТИЛЬ DroneControl.ino
// ============================================================================

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  static bool pendingShow = false;

  // ===== АВТОМАТИЧЕСКАЯ СМЕНА ЭФФЕКТОВ И ЦВЕТОВ =====
  if (autoMode) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastAutoChange > AUTO_CHANGE_INTERVAL) {
      lastAutoChange = currentMillis;
      
      // Переключаем цвет
      CRGB nextColor = AUTO_COLORS[autoColorIndex];
      targetR = nextColor.r;
      targetG = nextColor.g;
      targetB = nextColor.b;
      
      // Переключаем эффект
      gFx = AUTO_EFFECTS[autoEffectIndex];
      
      Serial.printf("[AUTO] Switched to effect %d, target color RGB(%d,%d,%d)\n", gFx, targetR, targetG, targetB);
      
      // Переходим к следующим индексам
      autoColorIndex = (autoColorIndex + 1) % AUTO_COLORS_COUNT;
      autoEffectIndex = (autoEffectIndex + 1) % AUTO_EFFECTS_COUNT;
    }
  }

  // ===== ПЛАВНЫЙ ФЕЙД ЦВЕТОВ (SMOOTH COLOR TRANSITION) =====
  static unsigned long lastColorStep = 0;
  if (millis() - lastColorStep >= 10) { // шаг каждые 10 мс
    lastColorStep = millis();
    bool colorChanged = false;
    
    if (gR < targetR) { gR++; colorChanged = true; }
    else if (gR > targetR) { gR--; colorChanged = true; }
    
    if (gG < targetG) { gG++; colorChanged = true; }
    else if (gG > targetG) { gG--; colorChanged = true; }
    
    if (gB < targetB) { gB++; colorChanged = true; }
    else if (gB > targetB) { gB--; colorChanged = true; }
    
    if (colorChanged && gFx == 0) {
      // Если мы в режиме статики (gFx == 0), перерисовываем активные блоки новым промежуточным цветом
      for (int i = 1; i <= TOTAL_BLOCKS; i++) {
        if (ledStates[i] && !fadeStates[i].isActive) {
          updateBlockLEDs(i, CRGB(gR, gG, gB));
        }
      }
      pendingShow = true;
    }
  }

  // ===== WiFi Reconnection and Auto-Restart Monitoring =====
  static unsigned long lastWifiCheck = 0;
  static unsigned long disconnectTime = 0;
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastWifiCheck > 5000) { // Check every 5 seconds
    lastWifiCheck = currentMillis;
    if (WiFi.getMode() == WIFI_STA) {
      if (WiFi.status() != WL_CONNECTED) {
        if (disconnectTime == 0) {
          disconnectTime = currentMillis;
          Serial.println("[WIFI] Connection lost! Monitoring for reconnect...");
        } else if (currentMillis - disconnectTime > 30000) { // If disconnected for over 30 seconds
          Serial.println("[WIFI] Reconnection failed for 30s. Auto-restarting ESP32 to clear networking stack...");
          delay(500);
          ESP.restart();
        }
      } else {
        disconnectTime = 0; // Connection is healthy
      }
    }
  }

  // Тест-режим: немедленный выход из loop() чтобы
  // ничто не перезаписало LED, установленные через /api/test
  if (testMode) {
    return;
  }

  // Если недавно был веб-запрос, пропускаем шаг анимации и вывод на светодиоды.
  // (Отключено для идеальной плавности при использовании преобразователей уровней и RMT)
  /*
  if (millis() - lastRequestTime < 50) {
    return;
  }
  */

  // ===== ЧТЕНИЕ ОТВЕТОВ ОТ MEGA =====
  if (Mega1Serial.available()) {
    mega1Response = Mega1Serial.readStringUntil('\n');
    mega1Response.trim();
    if (mega1Response.length() > 0) {
      Serial.println("[MEGA1 RX] " + mega1Response);

      if (mega1Response == CMD_PONG) {
        mega1Alive = true;
      }
    }
  }

  if (Mega2Serial.available()) {
    mega2Response = Mega2Serial.readStringUntil('\n');
    mega2Response.trim();
    if (mega2Response.length() > 0) {
      Serial.println("[MEGA2 RX] " + mega2Response);

      if (mega2Response == CMD_PONG) {
        mega2Alive = true;
      }
    }
  }

  // ===== ТАЙМАУТЫ БЛОКОВ =====
  // ВАЖНО: LED НЕ выключается по timeout!
  // LED остается включенным пока не придет команда STOP или DOWN
  // Таймаут нужен только для безопасности актуаторов (автостоп после движения)
  unsigned long now = millis();

  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (blockStates[i].isActive) {
      unsigned long elapsed = now - blockStates[i].startTime;

      if (elapsed >= (unsigned long)blockStates[i].duration) {
        Serial.printf("[TIMEOUT] Block %d - actuator stopped, LED stays ON\n", i);

        // Блок больше не активен (актуатор остановился)
        blockStates[i].isActive = false;

        // Отправляем команду STOP на Mega с duration=0
        // (Mega обработает это как STOP независимо от длительности)
        String stopCmd = "BLOCK:" + String(i) + ":STOP:0";
        const BlockConfig* cfg = getBlockConfig(i);
        if (cfg->megaNum == 1) {
          Mega1Serial.println(stopCmd);
          Serial.println("[MEGA1 TX TIMEOUT] " + stopCmd);
        } else {
          Mega2Serial.println(stopCmd);
          Serial.println("[MEGA2 TX TIMEOUT] " + stopCmd);
        }

        // Пересчитать активные
        activeBlocksCount = 0;
        for (int k = 1; k <= TOTAL_BLOCKS; k++) {
          if (blockStates[k].isActive) activeBlocksCount++;
        }
      }
    }
  }

  // ===== ФИЗИЧЕСКАЯ КНОПКА POWER (ВРЕМЕННО ОТКЛЮЧЕНО) =====
  /*
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(POWER_BUTTON);

  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);
    if (digitalRead(POWER_BUTTON) == LOW) {
      mainPowerOn = !mainPowerOn;
      digitalWrite(RELAY_MAIN_POWER, mainPowerOn ? HIGH : LOW);
      if (mainPowerOn) {
        Serial.println("[BUTTON] Power ON");
      } else {
        Serial.println("[BUTTON] Power OFF - stopping all blocks");
        Mega1Serial.println("ALL:STOP");
        Mega2Serial.println("ALL:STOP");
        memset(leds, 0, sizeof(leds));
        showLEDs();
        for (int i = 1; i <= TOTAL_BLOCKS; i++) {
          blockStates[i].isActive = false;
        }
        activeBlocksCount = 0;
      }
    }
  }
  lastButtonState = buttonState;
  */

  // ===== HEARTBEAT (PING каждые 2 сек) =====
  if (now - lastHeartbeat > HEARTBEAT_INTERVAL) {
    mega1Alive = false;
    mega2Alive = false;
    Mega1Serial.println(CMD_PING);
    Mega2Serial.println(CMD_PING);
    lastHeartbeat = now;
  }

  // ===== ПЛАВНОЕ НАРАСТАНИЕ (UP) И УГАСАНИЕ (DOWN) LED =====
  // pendingShow уже объявлена в начале loop()

  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (!fadeStates[i].isActive) continue;

    unsigned long elapsed = now - fadeStates[i].startTime;

    if (elapsed >= (unsigned long)fadeStates[i].duration) {
      // Анимация завершена
      fadeStates[i].isActive = false;
      if (fadeStates[i].fadeIn) {
        // Fade-IN завершен → зафиксировать макс. яркость
        CRGB fullColor = CRGB(gR, gG, gB);
        updateBlockLEDs(i, fullColor);
        Serial.printf("[LED] Block %d FADE-IN complete (full brightness)\n", i);
      } else {
        // Fade-OUT завершен → выключить полностью
        updateBlockLEDs(i, CRGB::Black);
        Serial.printf("[LED] Block %d FADE-OUT complete (off)\n", i);
      }
      pendingShow = true;
    } else {
      // Вычисляем прогресс 0.0 → 1.0
      float progress = (float)elapsed / (float)fadeStates[i].duration;

      uint8_t brightness;
      if (fadeStates[i].fadeIn) {
        // Нарастание: 0 → 255, с плавной ease-in кривой (quadratic)
        brightness = (uint8_t)(255.0f * progress * progress);
      } else {
        // Угасание: 255 → 0, с плавной ease-out кривой
        float inv = 1.0f - progress;
        brightness = (uint8_t)(255.0f * inv * inv);
      }

      CRGB fadeColor = CRGB(gR, gG, gB);
      fadeColor.nscale8(brightness);
      updateBlockLEDs(i, fadeColor);
      pendingShow = true;
    }
  }

  // ===== LED ЭФФЕКТЫ =====
  static uint32_t lastEffectFrame = 0;

  // В тест-режиме не перезаписываем LED
  if (testMode) return;

  if (gFx == 0) {
    // Статика - ничего не делаем, LED обновляются в lightUpBlock()
  } else {
    // Анимированные эффекты - обновляем с FPS
    if (now - lastEffectFrame >= (1000 / FPS)) {
      lastEffectFrame = now;

      switch (gFx) {
        case 1: fxPulse();    break;
        case 2: fxRainbow();  break;
        case 3: fxChase();    break;
        case 4: fxSparkle();  break;
        case 5: fxWave();     break;
        case 6: fxFire();     break;
        case 7: fxMeteor();   break;
        case 8: fxStrobe();   break;
        case 9: fxColorWipe();break;
        case 10: fxTwinkle(); break;
        case 11: fxRipple();  break;
        case 12: fxBreathing();break;
      }

      pendingShow = true;
    }
  }

  // ===== ВЫВОД НА СВЕТОДИОДЫ (С ОГРАНИЧЕНИЕМ ЧАСТОТЫ ДО 50 FPS) =====
  static unsigned long lastShowTime = 0;
  if (pendingShow) {
    // Не отправляем данные чаще чем раз в 20 мс (~50 FPS)
    if (now - lastShowTime >= 20) {
      showLEDs();
      lastShowTime = now;
      pendingShow = false;
    }
  }
}
