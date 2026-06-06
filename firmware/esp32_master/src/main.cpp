/**
 * RAMS Kinetic Table — ESP32 Master Firmware v2.1
 *
 * Режим: AP+STA (dual WiFi)
 *   - Создаёт свою точку доступа RAMS-ESP32 (192.168.4.1) — всегда доступен
 *   - Одновременно подключается к роутеру Rams_WIFI — для удобства
 *
 * Как подключить приложение:
 *   Вариант A (рекомендуется): компьютер подключается к Rams_WIFI,
 *             в админке вводишь IP который ESP32 получил от роутера
 *             (смотри Serial Monitor при загрузке: "[STA] IP: 192.168.x.x")
 *   Вариант B (резерв): компьютер подключается к RAMS-ESP32,
 *             в админке IP = 192.168.4.1 (уже стоит по умолчанию)
 *
 * HTTP API:
 *   GET  /api/status              → JSON статус + оба IP
 *   GET  /api/state               → текущее состояние LED (r,g,b,bri,spd,fx,zm)
 *   POST /api/block?num=N&action=up/down&duration=D → актуатор
 *   POST /api/all?action=down     → все вниз
 *   POST /api/stop                → экстренная остановка
 *   POST /api/led?mode=RAINBOW    → режим LED (legacy)
 *   POST /api/effect?id=0-7&speed=0-255 → эффект по ID
 *   POST /api/color?r=R&g=G&b=B  → RGB цвет
 *   POST /api/bri?v=0-255         → яркость
 *   POST /api/spd?v=0-255         → скорость анимации
 *   POST /api/zones?m=bitmask     → зоны LED
 *
 * Подключение:
 *   Serial1 (TX=25, RX=26) → Mega #1 (Blocks 1–8)
 *   Serial2 (TX=16, RX=17) → Mega #2 (Blocks 9–15)
 *   GPIO 23                → WS2812B LED Data
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include "protocol.h"

// ===================== AP CONFIG (собственная точка доступа) =====================
const char* AP_SSID     = "RAMS-ESP32";
const char* AP_PASSWORD = "rams2024";
// AP IP = 192.168.4.1 (всегда фиксированный)

// ===================== STA CONFIG (подключение к роутеру) =====================
const char* STA_SSID     = "Rams_WIFI";
const char* STA_PASSWORD = "Rams2021";

// ===================== HARDWARE CONFIG =====================
#define LED_PIN        23
#define NUM_LEDS       900
#define LED_BRIGHTNESS 200

#define MEGA1_TX 25
#define MEGA1_RX 26
#define MEGA2_TX 16
#define MEGA2_RX 17

// ===================== GLOBALS =====================
WebServer server(80);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

enum BlockState { STATE_STOP = 0, STATE_UP = 1, STATE_DOWN = -1 };
BlockState blockStates[TOTAL_BLOCKS + 1];

// Active block tracking (max 2 simultaneous)
int activeBlockCount = 0;

// Block timers — auto-stop after duration
unsigned long blockStopTime[TOTAL_BLOCKS + 1];

// LED — 8 effects + OFF (IDs match UI: 0=Static,1=Pulse,2=Rainbow,3=Chase,4=Sparkle,5=Wave,6=Fire,7=Meteor)
enum LedMode { LED_STATIC=0, LED_PULSE=1, LED_RAINBOW=2, LED_CHASE=3, LED_SPARKLE=4, LED_WAVE=5, LED_FIRE=6, LED_METEOR=7, LED_OFF=8 };
LedMode currentLedMode = LED_RAINBOW;
uint32_t ledBaseColor   = 0x0000FF;
uint8_t  ledSpeed       = 128;       // animation speed 0-255
unsigned long lastLedUpdate = 0;
uint16_t animCounter        = 0;

// Auto-cycle: rotate effects every N seconds
bool     autoCycleEnabled   = false;
uint32_t autoCycleInterval  = 60000;  // ms (default 1 minute)
unsigned long lastAutoCycle = 0;

// Mega heartbeat
unsigned long lastHeartbeatMega1 = 0;
unsigned long lastHeartbeatMega2 = 0;
bool mega1Alive = false;
bool mega2Alive = false;

// LED segments per block
struct LedSegment { int start; int count; };
LedSegment blockLeds[TOTAL_BLOCKS + 1] = {
  {0, 0},
  {0,  55}, {55,  55}, {110, 55}, {165, 55},
  {220,55}, {275, 55}, {330, 55},
  {385,55}, {440, 50}, {490, 50}, {540, 50},
  {590,50}, {640, 50}, {690, 50},
  {740,60},
};

// ===================== FORWARD DECLARATIONS =====================
void routeToMega(int blockId, String action);
void sendAllStop();
void sendAllDown();
void checkBlockTimers();
void checkMegaResponses();
void checkSafety();
void updateLeds();
void ledRainbow();
void ledPulse();
void ledWave();
void ledChase();
void ledSparkle();
void ledFire();
void ledMeteor();
void highlightBlock(int blockId, uint32_t color);
void setupRoutes();

// ===================== HTTP HELPERS =====================
void sendJson(int code, JsonDocument& doc) {
  String body;
  serializeJson(doc, body);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.send(code, "application/json", body);
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

// ===================== ROUTE HANDLERS =====================

// GET /api/status
void handleStatus() {
  JsonDocument doc;
  doc["ok"]            = true;
  doc["mega1"]         = mega1Alive ? "ok" : "dead";
  doc["mega2"]         = mega2Alive ? "ok" : "dead";
  doc["activeBlocks"]  = activeBlockCount;

  // AP info (всегда доступен)
  doc["apIP"]          = WiFi.softAPIP().toString();
  doc["apClients"]     = WiFi.softAPgetStationNum();

  // STA info (роутер)
  doc["staIP"]         = (WiFi.status() == WL_CONNECTED)
                           ? WiFi.localIP().toString()
                           : "not connected";
  doc["staSSID"]       = (WiFi.status() == WL_CONNECTED) ? STA_SSID : "";
  doc["staConnected"]  = (WiFi.status() == WL_CONNECTED);

  JsonArray blocks = doc["blocks"].to<JsonArray>();
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    const char* state = "stop";
    if (blockStates[i] == STATE_UP)   state = "up";
    if (blockStates[i] == STATE_DOWN) state = "down";
    blocks.add(state);
  }
  sendJson(200, doc);
}

// POST /api/block?num=N&action=up/down&duration=D
void handleBlock() {
  if (!server.hasArg("num") || !server.hasArg("action")) {
    JsonDocument err;
    err["error"] = "num and action required";
    sendJson(400, err);
    return;
  }

  int blockNum = server.arg("num").toInt();
  String action = server.arg("action");
  action.toUpperCase();
  unsigned long duration = server.hasArg("duration")
    ? (unsigned long)server.arg("duration").toInt()
    : 10000UL;

  if (blockNum < 1 || blockNum > TOTAL_BLOCKS) {
    JsonDocument err;
    err["error"] = "invalid block number";
    sendJson(400, err);
    return;
  }

  // Enforce max 2 simultaneous blocks
  if (action == ACTION_UP && blockStates[blockNum] != STATE_UP) {
    if (activeBlockCount >= 2) {
      JsonDocument err;
      err["error"]   = "max 2 blocks active";
      err["active"]  = activeBlockCount;
      sendJson(429, err);
      return;
    }
    activeBlockCount++;
    blockStopTime[blockNum] = millis() + duration;
  }
  else if (action == ACTION_DOWN && blockStates[blockNum] == STATE_UP) {
    activeBlockCount = max(0, activeBlockCount - 1);
    blockStopTime[blockNum] = 0;
  }

  if      (action == ACTION_UP)   blockStates[blockNum] = STATE_UP;
  else if (action == ACTION_DOWN) blockStates[blockNum] = STATE_DOWN;
  else if (action == ACTION_STOP) blockStates[blockNum] = STATE_STOP;

  routeToMega(blockNum, action);

  JsonDocument doc;
  doc["ok"]       = true;
  doc["block"]    = blockNum;
  doc["action"]   = action;
  doc["duration"] = duration;
  sendJson(200, doc);

  Serial.printf("[API] block %d → %s (dur=%lums)\n", blockNum, action.c_str(), duration);
}

// POST /api/all?action=down
void handleAll() {
  String action = server.hasArg("action") ? server.arg("action") : "stop";
  action.toUpperCase();

  if (action == ACTION_DOWN) {
    sendAllDown();
  } else {
    sendAllStop();
  }

  JsonDocument doc;
  doc["ok"]     = true;
  doc["action"] = action;
  sendJson(200, doc);
}

// POST /api/stop  (emergency stop)
void handleStop() {
  sendAllStop();
  JsonDocument doc;
  doc["ok"] = true;
  doc["action"] = "emergency_stop";
  sendJson(200, doc);
  Serial.println("[API] EMERGENCY STOP");
}

// POST /api/led?mode=RAINBOW|PULSE|WAVE|STATIC|CHASE|SPARKLE|FIRE|METEOR|OFF
//              &color=FF0000
//              &brightness=200
void handleLed() {
  if (server.hasArg("mode")) {
    String mode = server.arg("mode");
    mode.toUpperCase();
    if      (mode == "STATIC")  currentLedMode = LED_STATIC;
    else if (mode == "PULSE")   currentLedMode = LED_PULSE;
    else if (mode == "RAINBOW") currentLedMode = LED_RAINBOW;
    else if (mode == "CHASE")   currentLedMode = LED_CHASE;
    else if (mode == "SPARKLE") currentLedMode = LED_SPARKLE;
    else if (mode == "WAVE")    currentLedMode = LED_WAVE;
    else if (mode == "FIRE")    currentLedMode = LED_FIRE;
    else if (mode == "METEOR")  currentLedMode = LED_METEOR;
    else if (mode == "OFF")     currentLedMode = LED_OFF;
  }
  if (server.hasArg("color")) {
    ledBaseColor = (uint32_t)strtol(server.arg("color").c_str(), NULL, 16);
    currentLedMode = LED_STATIC;
  }
  if (server.hasArg("brightness")) {
    int b = server.arg("brightness").toInt();
    if (b >= 0 && b <= 255) strip.setBrightness(b);
  }

  JsonDocument doc;
  doc["ok"] = true;
  sendJson(200, doc);
}

// POST /api/effect?id=0-7&speed=0-255
void handleEffect() {
  if (!server.hasArg("id")) {
    JsonDocument err;
    err["error"] = "id required";
    sendJson(400, err);
    return;
  }
  int id = server.arg("id").toInt();
  if (id >= 0 && id <= 7) {
    currentLedMode = (LedMode)id;
  }
  if (server.hasArg("speed")) {
    ledSpeed = constrain(server.arg("speed").toInt(), 0, 255);
  }
  Serial.printf("[LED] Effect → %d, speed → %d\n", id, ledSpeed);
  JsonDocument doc;
  doc["ok"] = true;
  doc["effect"] = id;
  doc["speed"] = ledSpeed;
  sendJson(200, doc);
}

// POST /api/color?r=0-255&g=0-255&b=0-255
void handleColor() {
  int r = server.hasArg("r") ? constrain(server.arg("r").toInt(), 0, 255) : 0;
  int g = server.hasArg("g") ? constrain(server.arg("g").toInt(), 0, 255) : 0;
  int b = server.hasArg("b") ? constrain(server.arg("b").toInt(), 0, 255) : 0;
  ledBaseColor = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  currentLedMode = LED_STATIC;
  Serial.printf("[LED] Color → RGB(%d,%d,%d)\n", r, g, b);
  JsonDocument doc;
  doc["ok"] = true;
  sendJson(200, doc);
}

// POST /api/bri?v=0-255
void handleBrightness() {
  int v = server.hasArg("v") ? constrain(server.arg("v").toInt(), 0, 255) : 200;
  strip.setBrightness(v);
  Serial.printf("[LED] Brightness → %d\n", v);
  JsonDocument doc;
  doc["ok"] = true;
  sendJson(200, doc);
}

// POST /api/spd?v=0-255
void handleSpeed() {
  ledSpeed = server.hasArg("v") ? constrain(server.arg("v").toInt(), 0, 255) : 128;
  Serial.printf("[LED] Speed → %d\n", ledSpeed);
  JsonDocument doc;
  doc["ok"] = true;
  sendJson(200, doc);
}

// GET /api/state
void handleState() {
  JsonDocument doc;
  doc["r"]   = (ledBaseColor >> 16) & 0xFF;
  doc["g"]   = (ledBaseColor >>  8) & 0xFF;
  doc["b"]   =  ledBaseColor        & 0xFF;
  doc["bri"] = strip.getBrightness();
  doc["spd"] = ledSpeed;
  doc["fx"]  = (int)currentLedMode;
  doc["zm"]  = 0xFFFF; // all zones active
  doc["autoCycle"] = autoCycleEnabled;
  doc["autoCycleInterval"] = autoCycleInterval / 1000;
  sendJson(200, doc);
}

// POST /api/zones?m=bitmask
void handleZones() {
  // Zone support — placeholder, all LEDs treated as one zone for now
  JsonDocument doc;
  doc["ok"] = true;
  sendJson(200, doc);
}

// POST /api/autocycle?enabled=1&interval=60
// enabled: 0/1, interval: seconds (default 60)
void handleAutoCycle() {
  if (server.hasArg("enabled")) {
    autoCycleEnabled = server.arg("enabled").toInt() != 0;
    lastAutoCycle = millis(); // reset timer on toggle
  }
  if (server.hasArg("interval")) {
    int sec = constrain(server.arg("interval").toInt(), 5, 3600);
    autoCycleInterval = (uint32_t)sec * 1000;
  }
  Serial.printf("[LED] AutoCycle: %s, interval=%lus\n",
    autoCycleEnabled ? "ON" : "OFF", autoCycleInterval / 1000);
  JsonDocument doc;
  doc["ok"] = true;
  doc["autoCycle"] = autoCycleEnabled;
  doc["interval"] = autoCycleInterval / 1000;
  sendJson(200, doc);
}

// GET /api/autocycle — get current auto-cycle state
void handleGetAutoCycle() {
  JsonDocument doc;
  doc["autoCycle"] = autoCycleEnabled;
  doc["interval"] = autoCycleInterval / 1000;
  doc["currentEffect"] = (int)currentLedMode;
  sendJson(200, doc);
}

// ===================== SETUP ROUTES =====================
void setupRoutes() {
  server.on("/api/status", HTTP_GET,  handleStatus);
  server.on("/api/state",  HTTP_GET,  handleState);
  server.on("/api/block",  HTTP_POST, handleBlock);
  server.on("/api/all",    HTTP_POST, handleAll);
  server.on("/api/stop",   HTTP_POST, handleStop);
  server.on("/api/led",    HTTP_POST, handleLed);
  server.on("/api/effect", HTTP_POST, handleEffect);
  server.on("/api/color",  HTTP_POST, handleColor);
  server.on("/api/bri",    HTTP_POST, handleBrightness);
  server.on("/api/spd",    HTTP_POST, handleSpeed);
  server.on("/api/zones",     HTTP_POST, handleZones);
  server.on("/api/autocycle", HTTP_POST, handleAutoCycle);
  server.on("/api/autocycle", HTTP_GET,  handleGetAutoCycle);

  // CORS preflight for all endpoints
  const char* endpoints[] = {"/api/block", "/api/all", "/api/stop", "/api/led",
                             "/api/effect", "/api/color", "/api/bri", "/api/spd",
                             "/api/zones", "/api/state", "/api/status", "/api/autocycle"};
  for (const char* ep : endpoints) {
    server.on(ep, HTTP_OPTIONS, handleOptions);
  }

  // 404
  server.onNotFound([]() {
    JsonDocument doc;
    doc["error"] = "not found";
    doc["uri"]   = server.uri();
    sendJson(404, doc);
  });
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD, SERIAL_8N1, MEGA1_RX, MEGA1_TX);
  Serial2.begin(SERIAL_BAUD, SERIAL_8N1, MEGA2_RX, MEGA2_TX);

  Serial.println("\n=== RAMS ESP32 Master v2.0 ===");

  // Initialize block states
  for (int i = 0; i <= TOTAL_BLOCKS; i++) {
    blockStates[i]   = STATE_STOP;
    blockStopTime[i] = 0;
  }

  // LED init
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  strip.show();
  Serial.println("[LED] Initialized");

  // ===== WiFi AP+STA dual mode =====
  WiFi.mode(WIFI_AP_STA);

  // 1) Поднимаем точку доступа (всегда на 192.168.4.1)
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.printf("[AP]  SSID: %s | IP: %s\n", AP_SSID, WiFi.softAPIP().toString().c_str());

  // 2) Подключаемся к роутеру
  Serial.printf("[STA] Connecting to '%s'", STA_SSID);
  WiFi.begin(STA_SSID, STA_PASSWORD);

  // Ждём до 10 сек (не блокируем надолго — AP уже работает)
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(250);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[STA] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[STA] >>> Введи этот IP в Admin → Hardware: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[STA] Not connected to router — AP mode still works at 192.168.4.1");
  }
  // ===== End WiFi =====

  // HTTP server (слушает на обоих интерфейсах)
  setupRoutes();
  server.begin();
  Serial.println("[HTTP] Server started on port 80");

  // ===== OTA (Over-The-Air) Setup =====
  ArduinoOTA.setHostname("RAMS-ESP32");
  ArduinoOTA.setPassword("rams2024");

  ArduinoOTA.onStart([]() {
    Serial.println("[OTA] Update Start");
    // Stop LED updates during OTA
    currentLedMode = LED_OFF;
    strip.clear();
    strip.show();
    // Stop all blocks for safety
    sendAllStop();
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
  // ===== End OTA =====

  // Ping Megas
  Serial1.println("PING");
  Serial2.println("PING");
  lastHeartbeatMega1 = millis();
  lastHeartbeatMega2 = millis();

  Serial.println("[RAMS] Ready!");
  Serial.printf("[RAMS] AP:  http://%s/api/status\n", WiFi.softAPIP().toString().c_str());
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[RAMS] STA: http://%s/api/status\n", WiFi.localIP().toString().c_str());
    Serial.println("[OTA] Upload: platformio run -t upload --upload-port RAMS-ESP32.local");
  }
}

// ===================== LOOP =====================
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  checkBlockTimers();
  checkMegaResponses();
  checkSafety();

  // Auto-cycle effects
  if (autoCycleEnabled && millis() - lastAutoCycle >= autoCycleInterval) {
    lastAutoCycle = millis();
    // Cycle through effects 0-7, skip OFF(8)
    int next = ((int)currentLedMode + 1) % 8;
    currentLedMode = (LedMode)next;
    Serial.printf("[AutoCycle] Switched to effect %d\n", next);
  }

  if (millis() - lastLedUpdate > 33) {
    updateLeds();
    lastLedUpdate = millis();
  }

  // Heartbeat to Megas every 2 seconds
  static unsigned long lastHB = 0;
  if (millis() - lastHB > HEARTBEAT_INTERVAL) {
    Serial1.println("PING");
    Serial2.println("PING");
    lastHB = millis();
  }
}

// ===================== BLOCK AUTO-STOP TIMERS =====================
void checkBlockTimers() {
  unsigned long now = millis();
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (blockStates[i] == STATE_UP && blockStopTime[i] > 0 && now >= blockStopTime[i]) {
      blockStates[i]   = STATE_STOP;
      blockStopTime[i] = 0;
      activeBlockCount = max(0, activeBlockCount - 1);
      routeToMega(i, ACTION_STOP);
      Serial.printf("[Timer] Block %d auto-stopped\n", i);
    }
  }
}

// ===================== SERIAL ROUTING =====================
void routeToMega(int blockId, String action) {
  String msg = "BLOCK:" + String(blockId) + ":" + action + "\n";
  if (blockId >= MEGA1_BLOCK_START && blockId <= MEGA1_BLOCK_END) {
    Serial1.print(msg);
  } else if (blockId >= MEGA2_BLOCK_START && blockId <= MEGA2_BLOCK_END) {
    Serial2.print(msg);
  }
}

void sendAllStop() {
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    blockStates[i]   = STATE_STOP;
    blockStopTime[i] = 0;
  }
  activeBlockCount = 0;
  Serial1.println("ALL:STOP");
  Serial2.println("ALL:STOP");
  Serial.println("[ALL] STOP");
}

void sendAllDown() {
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (blockStates[i] == STATE_UP) {
      blockStates[i]   = STATE_DOWN;
      blockStopTime[i] = 0;
      routeToMega(i, ACTION_DOWN);
      delay(STAGGER_DELAY_MS);
    }
  }
  activeBlockCount = 0;
  Serial.println("[ALL] DOWN");
}

// ===================== MEGA RESPONSES =====================
void checkMegaResponses() {
  while (Serial1.available()) {
    String line = Serial1.readStringUntil('\n');
    line.trim();
    if (line == "PONG") { mega1Alive = true; lastHeartbeatMega1 = millis(); }
  }
  while (Serial2.available()) {
    String line = Serial2.readStringUntil('\n');
    line.trim();
    if (line == "PONG") { mega2Alive = true; lastHeartbeatMega2 = millis(); }
  }
}

// ===================== SAFETY =====================
void checkSafety() {
  unsigned long now = millis();

  if (now - lastHeartbeatMega1 > HEARTBEAT_INTERVAL * 3 && mega1Alive) {
    Serial.println("[SAFETY] Mega#1 heartbeat lost! Stopping blocks 1-8");
    mega1Alive = false;
    Serial1.println("ALL:STOP");
    for (int i = MEGA1_BLOCK_START; i <= MEGA1_BLOCK_END; i++) {
      if (blockStates[i] == STATE_UP) activeBlockCount = max(0, activeBlockCount - 1);
      blockStates[i] = STATE_STOP;
    }
  }

  if (now - lastHeartbeatMega2 > HEARTBEAT_INTERVAL * 3 && mega2Alive) {
    Serial.println("[SAFETY] Mega#2 heartbeat lost! Stopping blocks 9-15");
    mega2Alive = false;
    Serial2.println("ALL:STOP");
    for (int i = MEGA2_BLOCK_START; i <= MEGA2_BLOCK_END; i++) {
      if (blockStates[i] == STATE_UP) activeBlockCount = max(0, activeBlockCount - 1);
      blockStates[i] = STATE_STOP;
    }
  }
}

// ===================== LED ANIMATION =====================
// Speed factor: higher ledSpeed = faster animation
static float speedFactor() { return 0.5f + (ledSpeed / 255.0f) * 3.0f; }

void updateLeds() {
  animCounter++;
  switch (currentLedMode) {
    case LED_STATIC:  strip.fill(ledBaseColor, 0, NUM_LEDS); break;
    case LED_PULSE:   ledPulse();   break;
    case LED_RAINBOW: ledRainbow(); break;
    case LED_CHASE:   ledChase();   break;
    case LED_SPARKLE: ledSparkle(); break;
    case LED_WAVE:    ledWave();    break;
    case LED_FIRE:    ledFire();    break;
    case LED_METEOR:  ledMeteor();  break;
    case LED_OFF:     strip.clear(); break;
  }
  // Soft highlight for DOWN blocks only (UP blocks don't interfere with effects)
  for (int i = 1; i <= TOTAL_BLOCKS; i++) {
    if (blockStates[i] == STATE_DOWN) {
      // Soft orange overlay (30% opacity) - doesn't kill the effect
      LedSegment seg = blockLeds[i];
      for (int j = seg.start; j < seg.start + seg.count && j < NUM_LEDS; j++) {
        uint32_t current = strip.getPixelColor(j);
        uint8_t r = ((current >> 16) & 0xFF);
        uint8_t g = ((current >>  8) & 0xFF);
        uint8_t b = ( current        & 0xFF);
        // Blend with soft orange (255,80,0) at 30%
        r = (r * 7 + 255 * 3) / 10;
        g = (g * 7 +  80 * 3) / 10;
        b = (b * 7 +   0 * 3) / 10;
        strip.setPixelColor(j, strip.Color(r, g, b));
      }
    }
  }
  strip.show();
}

void ledRainbow() {
  float spd = speedFactor();
  for (int i = 0; i < NUM_LEDS; i++) {
    uint16_t hue = (i * 65536L / NUM_LEDS + (uint32_t)(animCounter * spd) * 256) & 0xFFFF;
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
  }
}

void ledPulse() {
  float spd = speedFactor();
  uint8_t brightness = (sin(animCounter * 0.05f * spd) + 1.0f) * 127;
  uint8_t r = ((ledBaseColor >> 16) & 0xFF) * brightness / 255;
  uint8_t g = ((ledBaseColor >>  8) & 0xFF) * brightness / 255;
  uint8_t b = ( ledBaseColor        & 0xFF) * brightness / 255;
  strip.fill(strip.Color(r, g, b), 0, NUM_LEDS);
}

void ledWave() {
  float spd = speedFactor();
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = (sin((i + animCounter * spd) * 0.1f) + 1.0f) * 127;
    uint8_t r = ((ledBaseColor >> 16) & 0xFF) * brightness / 255;
    uint8_t g = ((ledBaseColor >>  8) & 0xFF) * brightness / 255;
    uint8_t b = ( ledBaseColor        & 0xFF) * brightness / 255;
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

void ledChase() {
  float spd = speedFactor();
  int chaseLen = 20;  // length of lit segment
  strip.clear();
  int pos = ((int)(animCounter * spd * 2)) % NUM_LEDS;
  uint8_t r = (ledBaseColor >> 16) & 0xFF;
  uint8_t g = (ledBaseColor >>  8) & 0xFF;
  uint8_t b =  ledBaseColor        & 0xFF;
  for (int i = 0; i < chaseLen; i++) {
    int idx = (pos + i) % NUM_LEDS;
    // Fade tail
    uint8_t fade = 255 - (i * 255 / chaseLen);
    strip.setPixelColor(idx, strip.Color(r * fade / 255, g * fade / 255, b * fade / 255));
  }
}

void ledSparkle() {
  // Dim all pixels slightly
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.getPixelColor(i);
    uint8_t r = ((c >> 16) & 0xFF) * 220 / 256;
    uint8_t g = ((c >>  8) & 0xFF) * 220 / 256;
    uint8_t b = ( c        & 0xFF) * 220 / 256;
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  // Add random sparkles — more sparkles at higher speed
  int numSparkles = 1 + ledSpeed / 32;
  uint8_t cr = (ledBaseColor >> 16) & 0xFF;
  uint8_t cg = (ledBaseColor >>  8) & 0xFF;
  uint8_t cb =  ledBaseColor        & 0xFF;
  for (int s = 0; s < numSparkles; s++) {
    int idx = random(NUM_LEDS);
    strip.setPixelColor(idx, strip.Color(cr, cg, cb));
  }
}

void ledFire() {
  for (int i = 0; i < NUM_LEDS; i++) {
    // Random heat variation
    uint8_t heat = random(80, 255);
    // Map heat to fire colors (red → orange → yellow)
    uint8_t r, g, b;
    if (heat < 170) {
      r = heat;
      g = heat / 3;
      b = 0;
    } else {
      r = 255;
      g = heat - 80;
      b = (heat - 170) / 2;
    }
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

void ledMeteor() {
  float spd = speedFactor();
  int meteorLen = 30;
  // Fade all pixels
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.getPixelColor(i);
    uint8_t r = ((c >> 16) & 0xFF) * 200 / 256;
    uint8_t g = ((c >>  8) & 0xFF) * 200 / 256;
    uint8_t b = ( c        & 0xFF) * 200 / 256;
    // Random decay for organic look
    if (random(10) > 5) {
      strip.setPixelColor(i, strip.Color(r, g, b));
    }
  }
  // Draw meteor head
  int pos = ((int)(animCounter * spd * 3)) % (NUM_LEDS + meteorLen);
  uint8_t cr = (ledBaseColor >> 16) & 0xFF;
  uint8_t cg = (ledBaseColor >>  8) & 0xFF;
  uint8_t cb =  ledBaseColor        & 0xFF;
  for (int i = 0; i < meteorLen; i++) {
    int idx = pos - i;
    if (idx >= 0 && idx < NUM_LEDS) {
      uint8_t fade = 255 - (i * 255 / meteorLen);
      strip.setPixelColor(idx, strip.Color(cr * fade / 255, cg * fade / 255, cb * fade / 255));
    }
  }
}

void highlightBlock(int blockId, uint32_t color) {
  if (blockId < 1 || blockId > TOTAL_BLOCKS) return;
  LedSegment seg = blockLeds[blockId];
  for (int i = seg.start; i < seg.start + seg.count && i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
}
