/**
 * RAMS Simple Controller - ESP32
 *
 * Простой текстовый протокол для управления актуаторами
 * Основано на DroneControl.ino - без JSON, быстро и надёжно
 *
 * WiFi: RAMS_Controller / rams2026
 * IP: 192.168.4.1
 *
 * HTTP API:
 * - POST /api/block?num=5&action=up&duration=10000
 * - POST /api/stop
 * - GET /api/status
 *
 * Serial протокол (ESP32 → Mega):
 * - BLOCK:5:UP:10000\n
 * - BLOCK:3:DOWN:10000\n
 * - ALL:STOP\n
 * - PING\n
 *
 * Serial ответы (Mega → ESP32):
 * - ACK:5:UP\n
 * - PONG\n
 * - DONE:5\n
 */

#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>

// ============================================================================
// КОНФИГУРАЦИЯ WiFi
// ============================================================================
#define AP_SSID     "RAMS_Controller"
#define AP_PASS     "rams2026"

// ============================================================================
// LED КОНФИГУРАЦИЯ
// ============================================================================
#define NUM_STRIPS  10
#define MAX_LEDS    144
#define NUM_ZONES   15

// GPIO pins для LED strips
static const uint8_t  PIN_GPIO[NUM_STRIPS] = {  5, 21, 22, 23, 27, 13, 14, 33,   4, 32 };
static const uint16_t PIN_LEDS[NUM_STRIPS] = { 64, 33, 33, 33, 33, 33, 33, 33, 144, 33 };

static CRGB leds[NUM_STRIPS][MAX_LEDS];

// Глобальные LED параметры
static uint8_t gR = 0, gG = 150, gB = 255;  // Cyan по умолчанию
static uint8_t gBri = 200;

// ============================================================================
// MEGA SERIAL КОНФИГУРАЦИЯ
// ============================================================================
#define MEGA1_RX 26  // GPIO26 RX ← TX1 Mega #1
#define MEGA1_TX 25  // GPIO25 TX → RX1 Mega #1
HardwareSerial Mega1Serial(1);

#define MEGA2_RX 35  // GPIO35 RX ← TX1 Mega #2
#define MEGA2_TX 12  // GPIO12 TX → RX1 Mega #2
HardwareSerial Mega2Serial(2);

// ============================================================================
// БЛОКИ И ЗОНЫ
// ============================================================================
struct BlockZone {
  uint8_t blockNum;
  uint8_t ledStrip;
  uint16_t ledStart;
  uint16_t ledCount;
  CRGB currentColor;
  uint8_t currentBri;
  bool isActive;
  unsigned long startTime;
  int duration;
  bool isFading;
};

BlockZone blockZones[NUM_ZONES];

// ============================================================================
// СОСТОЯНИЕ СИСТЕМЫ
// ============================================================================
struct BlockState {
  bool isActive;          // Блок работает?
  unsigned long startTime; // Когда начал
  int duration;           // Длительность (мс)
  String action;          // "up" или "down"
};

BlockState blockStates[16];  // 0 не используется, 1-15 блоки
int activeBlocksCount = 0;   // Счётчик активных блоков

// Heartbeat
bool mega1Alive = false;
bool mega2Alive = false;
unsigned long lastPongMega1 = 0;
unsigned long lastPongMega2 = 0;

// ============================================================================
// WEB SERVER
// ============================================================================
WebServer server(80);

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n========================================");
  Serial.println("  RAMS SIMPLE CONTROLLER v3.0");
  Serial.println("  Text Protocol - Fast & Reliable");
  Serial.println("========================================");

  // Инициализация LED
  setupLEDs();

  // Инициализация зон
  setupBlockZones();

  // Инициализация Serial связи с Mega
  setupMegaSerial();

  // Инициализация состояний блоков
  for (int i = 0; i <= 15; i++) {
    blockStates[i].isActive = false;
    blockStates[i].startTime = 0;
    blockStates[i].duration = 0;
    blockStates[i].action = "";
  }

  // Запуск WiFi AP
  setupWiFi();

  // Запуск веб-сервера
  setupWebServer();

  Serial.println("\n[READY] System initialized!");
  Serial.println("========================================\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop() {
  server.handleClient();

  // Обработка активных блоков (таймауты)
  processActiveBlocks();

  // Обновление LED эффектов
  updateLEDEffects();

  // Чтение ответов от Mega
  readFromMega();

  // Heartbeat
  static unsigned long lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 2000) {
    sendToMega1("PING");
    sendToMega2("PING");
    lastHeartbeat = millis();
  }

  delay(1);
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ LED
// ============================================================================
void setupLEDs() {
  Serial.println("[LED] Initializing FastLED...");

  FastLED.addLeds<WS2812B,  5, GRB>(leds[0], PIN_LEDS[0]);
  FastLED.addLeds<WS2812B, 21, GRB>(leds[1], PIN_LEDS[1]);
  FastLED.addLeds<WS2812B, 22, GRB>(leds[2], PIN_LEDS[2]);
  FastLED.addLeds<WS2812B, 23, GRB>(leds[3], PIN_LEDS[3]);
  FastLED.addLeds<WS2812B, 27, GRB>(leds[4], PIN_LEDS[4]);
  FastLED.addLeds<WS2812B, 13, GRB>(leds[5], PIN_LEDS[5]);
  FastLED.addLeds<WS2812B, 14, GRB>(leds[6], PIN_LEDS[6]);
  FastLED.addLeds<WS2812B, 33, GRB>(leds[7], PIN_LEDS[7]);
  FastLED.addLeds<WS2812B,  4, GRB>(leds[8], PIN_LEDS[8]);
  FastLED.addLeds<WS2812B, 32, GRB>(leds[9], PIN_LEDS[9]);

  FastLED.setBrightness(gBri);
  FastLED.clear(true);

  Serial.println("[LED] FastLED initialized (10 strips, 538 LEDs total) ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ ЗОН БЛОКОВ
// ============================================================================
void setupBlockZones() {
  Serial.println("[ZONES] Configuring block → LED zones mapping...");

  // Блоки 1-8: по одному лучу на блок
  uint8_t rayStrips[] = {1, 2, 3, 4, 5, 6, 7, 9};
  for (int i = 0; i < 8; i++) {
    blockZones[i].blockNum = i + 1;
    blockZones[i].ledStrip = rayStrips[i];
    blockZones[i].ledStart = 0;
    blockZones[i].ledCount = 33;
    blockZones[i].currentColor = CRGB::Black;
    blockZones[i].currentBri = 0;
    blockZones[i].isActive = false;
    blockZones[i].isFading = false;
  }

  // Блоки 9-12: внутренний круг (strip 0)
  for (int i = 0; i < 4; i++) {
    blockZones[8 + i].blockNum = 9 + i;
    blockZones[8 + i].ledStrip = 0;
    blockZones[8 + i].ledStart = i * 16;
    blockZones[8 + i].ledCount = 16;
    blockZones[8 + i].currentColor = CRGB::Black;
    blockZones[8 + i].currentBri = 0;
    blockZones[8 + i].isActive = false;
    blockZones[8 + i].isFading = false;
  }

  // Блоки 13-15: средний круг (strip 8)
  for (int i = 0; i < 3; i++) {
    blockZones[12 + i].blockNum = 13 + i;
    blockZones[12 + i].ledStrip = 8;
    blockZones[12 + i].ledStart = i * 48;
    blockZones[12 + i].ledCount = 48;
    blockZones[12 + i].currentColor = CRGB::Black;
    blockZones[12 + i].currentBri = 0;
    blockZones[12 + i].isActive = false;
    blockZones[12 + i].isFading = false;
  }

  Serial.println("[ZONES] ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ MEGA SERIAL
// ============================================================================
void setupMegaSerial() {
  Serial.println("[MEGA] Initializing serial communication...");

  Mega1Serial.begin(115200, SERIAL_8N1, MEGA1_RX, MEGA1_TX);
  delay(50);
  Serial.println("[MEGA] Mega #1 ready on GPIO25/26 ✓");

  Mega2Serial.begin(115200, SERIAL_8N1, MEGA2_RX, MEGA2_TX);
  delay(50);
  Serial.println("[MEGA] Mega #2 ready on GPIO12/35 ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ WiFi
// ============================================================================
void setupWiFi() {
  Serial.println("[WIFI] Starting Access Point...");

  WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress IP = WiFi.softAPIP();

  Serial.print("[WIFI] AP Started: ");
  Serial.println(AP_SSID);
  Serial.print("[WIFI] IP: ");
  Serial.println(IP);
  Serial.println("[WIFI] ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ WEB SERVER
// ============================================================================
void setupWebServer() {
  Serial.println("[SERVER] Starting web server...");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/block", HTTP_POST, handleBlockCommand);
  server.on("/api/stop", HTTP_POST, handleStopAll);

  server.begin();
  Serial.println("[SERVER] Web server started on port 80 ✓");
}

// ============================================================================
// WEB HANDLERS
// ============================================================================
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>RAMS Simple Controller</title></head>";
  html += "<body style='background:#111;color:#fff;font-family:Arial;padding:20px'>";
  html += "<h1 style='color:#0ff'>RAMS SIMPLE CONTROLLER v3.0</h1>";
  html += "<p>Active Blocks: <span id='active'>0</span> / 2 MAX</p>";
  html += "<p>Mega #1: <span id='m1'>?</span> | Mega #2: <span id='m2'>?</span></p>";
  html += "<button onclick='stopAll()' style='background:#f00;color:#fff;padding:10px'>STOP ALL</button>";
  html += "<h2>Test Blocks</h2>";

  for (int i = 1; i <= 15; i++) {
    html += "<button onclick='up(" + String(i) + ")' style='background:#0f0;color:#000;padding:8px;margin:2px'>Block " + String(i) + " UP</button>";
  }

  html += "<script>";
  html += "function up(b){fetch('/api/block?num='+b+'&action=up&duration=10000',{method:'POST'});}";
  html += "function stopAll(){fetch('/api/stop',{method:'POST'});}";
  html += "setInterval(()=>{fetch('/api/status').then(r=>r.text()).then(t=>console.log(t));},2000);";
  html += "</script></body></html>";

  server.send(200, "text/html", html);
}

void handleStatus() {
  // Простой текстовый ответ
  String status = "Active:" + String(activeBlocksCount);
  status += ",M1:" + String(mega1Alive ? "OK" : "ERR");
  status += ",M2:" + String(mega2Alive ? "OK" : "ERR");

  server.send(200, "text/plain", status);
}

void handleBlockCommand() {
  // Параметры из query string
  int blockNum = server.arg("num").toInt();
  String action = server.arg("action");
  int duration = server.arg("duration").toInt();

  if (blockNum < 1 || blockNum > 15) {
    server.send(400, "text/plain", "ERROR:Invalid block (1-15)");
    return;
  }

  if (action != "up" && action != "down") {
    server.send(400, "text/plain", "ERROR:Invalid action (up/down)");
    return;
  }

  if (duration <= 0) {
    duration = 10000;  // По умолчанию 10 секунд
  }

  // ВАЖНО: Проверка на максимум 2 активных блока
  if (action == "up" && activeBlocksCount >= 2 && !blockStates[blockNum].isActive) {
    Serial.println("[BLOCK] REJECTED: Maximum 2 blocks active!");
    server.send(429, "text/plain", "ERROR:Maximum 2 blocks active");
    return;
  }

  // Отправить команду на Mega
  bool sent = sendBlockCommand(blockNum, action, duration);

  if (sent) {
    // Обновить состояние блока
    blockStates[blockNum].isActive = true;
    blockStates[blockNum].startTime = millis();
    blockStates[blockNum].duration = duration;
    blockStates[blockNum].action = action;

    // Обновить счётчик активных блоков
    recountActiveBlocks();

    // Управление LED
    if (action == "up") {
      turnOnBlockLED(blockNum);
    } else if (action == "down") {
      fadeBlockLED(blockNum);
    }

    Serial.printf("[BLOCK] %d %s (%dms) - Active: %d/2\n", blockNum, action.c_str(), duration, activeBlocksCount);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(500, "text/plain", "ERROR:Failed to send");
  }
}

void handleStopAll() {
  Serial.println("[API] STOP ALL BLOCKS");

  sendToMega1("ALL:STOP");
  sendToMega2("ALL:STOP");

  // Сбросить все состояния
  for (int i = 1; i <= 15; i++) {
    blockStates[i].isActive = false;
  }
  activeBlocksCount = 0;

  // Выключить все LED
  turnOffAllLEDs();

  server.send(200, "text/plain", "OK");
}

// ============================================================================
// ОТПРАВКА КОМАНД НА MEGA (ТЕКСТОВЫЙ ПРОТОКОЛ)
// ============================================================================
bool sendBlockCommand(int blockNum, String action, int duration) {
  // Формат: BLOCK:5:UP:10000\n
  action.toUpperCase();  // Преобразовать в верхний регистр
  String cmd = "BLOCK:" + String(blockNum) + ":" + action + ":" + String(duration);

  if (blockNum >= 1 && blockNum <= 8) {
    return sendToMega1(cmd);
  } else if (blockNum >= 9 && blockNum <= 15) {
    return sendToMega2(cmd);
  }

  return false;
}

bool sendToMega1(String cmd) {
  Mega1Serial.println(cmd);
  Serial.println("[MEGA1 TX] " + cmd);
  return true;
}

bool sendToMega2(String cmd) {
  Mega2Serial.println(cmd);
  Serial.println("[MEGA2 TX] " + cmd);
  return true;
}

void readFromMega() {
  // Читаем от Mega #1
  if (Mega1Serial.available()) {
    String response = Mega1Serial.readStringUntil('\n');
    response.trim();
    if (response.length() > 0) {
      Serial.println("[MEGA1 RX] " + response);

      if (response == "PONG") {
        mega1Alive = true;
        lastPongMega1 = millis();
      }
      // Парсинг других ответов: ACK:5:UP, DONE:5 и т.д.
    }
  }

  // Читаем от Mega #2
  if (Mega2Serial.available()) {
    String response = Mega2Serial.readStringUntil('\n');
    response.trim();
    if (response.length() > 0) {
      Serial.println("[MEGA2 RX] " + response);

      if (response == "PONG") {
        mega2Alive = true;
        lastPongMega2 = millis();
      }
    }
  }
}

// ============================================================================
// ОБРАБОТКА АКТИВНЫХ БЛОКОВ (ТАЙМАУТЫ)
// ============================================================================
void processActiveBlocks() {
  unsigned long now = millis();

  for (int i = 1; i <= 15; i++) {
    if (!blockStates[i].isActive) continue;

    unsigned long elapsed = now - blockStates[i].startTime;

    // Проверка таймаута
    if (elapsed >= blockStates[i].duration) {
      Serial.printf("[BLOCK] %d timeout (%s)\n", i, blockStates[i].action.c_str());

      // Если это был DOWN - выключить LED
      if (blockStates[i].action == "down") {
        turnOffBlockLED(i);
      }

      // Деактивировать блок
      blockStates[i].isActive = false;
      recountActiveBlocks();
    }
  }
}

void recountActiveBlocks() {
  int count = 0;
  for (int i = 1; i <= 15; i++) {
    if (blockStates[i].isActive) count++;
  }
  activeBlocksCount = count;
}

// ============================================================================
// УПРАВЛЕНИЕ LED ЗОНАМИ
// ============================================================================
void turnOnBlockLED(int blockNum) {
  for (int i = 0; i < NUM_ZONES; i++) {
    if (blockZones[i].blockNum == blockNum) {
      blockZones[i].isActive = true;
      blockZones[i].isFading = false;
      blockZones[i].currentColor = CRGB(gR, gG, gB);
      blockZones[i].currentBri = 255;

      setBlockZoneLEDs(i, CRGB(gR, gG, gB));
      FastLED.show();

      Serial.printf("[LED] Block %d ON\n", blockNum);
      break;
    }
  }
}

void fadeBlockLED(int blockNum) {
  for (int i = 0; i < NUM_ZONES; i++) {
    if (blockZones[i].blockNum == blockNum) {
      blockZones[i].isActive = true;
      blockZones[i].isFading = true;
      blockZones[i].currentBri = 255;

      Serial.printf("[LED] Block %d FADING\n", blockNum);
      break;
    }
  }
}

void turnOffBlockLED(int blockNum) {
  for (int i = 0; i < NUM_ZONES; i++) {
    if (blockZones[i].blockNum == blockNum) {
      blockZones[i].isActive = false;
      blockZones[i].isFading = false;
      blockZones[i].currentBri = 0;

      setBlockZoneLEDs(i, CRGB::Black);
      FastLED.show();

      Serial.printf("[LED] Block %d OFF\n", blockNum);
      break;
    }
  }
}

void turnOffAllLEDs() {
  for (int i = 0; i < NUM_ZONES; i++) {
    blockZones[i].isActive = false;
    blockZones[i].isFading = false;
    blockZones[i].currentBri = 0;
    setBlockZoneLEDs(i, CRGB::Black);
  }
  FastLED.show();
  Serial.println("[LED] All zones OFF");
}

void setBlockZoneLEDs(int zoneIndex, CRGB color) {
  uint8_t strip = blockZones[zoneIndex].ledStrip;
  uint16_t start = blockZones[zoneIndex].ledStart;
  uint16_t count = blockZones[zoneIndex].ledCount;

  for (uint16_t j = 0; j < count; j++) {
    leds[strip][start + j] = color;
  }
}

// ============================================================================
// ОБНОВЛЕНИЕ LED ЭФФЕКТОВ
// ============================================================================
void updateLEDEffects() {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  if (now - lastUpdate < 20) return;  // 50 FPS
  lastUpdate = now;

  bool needShow = false;

  for (int i = 0; i < NUM_ZONES; i++) {
    if (!blockZones[i].isActive) continue;

    // Fade эффект при DOWN
    if (blockZones[i].isFading && blockZones[i].currentBri > 0) {
      blockZones[i].currentBri -= 5;
      if (blockZones[i].currentBri < 0) blockZones[i].currentBri = 0;

      CRGB fadedColor = blockZones[i].currentColor;
      fadedColor.nscale8(blockZones[i].currentBri);

      setBlockZoneLEDs(i, fadedColor);
      needShow = true;
    }
  }

  if (needShow) {
    FastLED.show();
  }
}
