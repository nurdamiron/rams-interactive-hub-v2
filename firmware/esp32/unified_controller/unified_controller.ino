/**
 * RAMS Unified Controller - ESP32
 *
 * Управляет:
 * - 15 актуаторными блоками (через 2x Arduino Mega)
 * - WS2812B LED лентами (10 strips, 538 LEDs total)
 *
 * Архитектура:
 * - ESP32 → Serial2 → Mega #1 (блоки 1-8)
 * - ESP32 → Serial1 → Mega #2 (блоки 9-15)
 * - ESP32 → GPIO pins → WS2812B LEDs
 *
 * Логика:
 * - Каждый блок (1-15) → своя LED зона
 * - Блок UP → LED зона включается (яркий цвет)
 * - Блок DOWN → LED зона гаснет (fade out)
 * - Последовательная работа: блок N down → блок N+1 up
 *
 * @version 2.0
 * @author RAMS Global Team
 * @date 2026-02-09
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FastLED.h>

// ============================================================================
// КОНФИГУРАЦИЯ WiFi
// ============================================================================
#define AP_SSID     "RAMS_Controller"
#define AP_PASS     "rams2026"

// ============================================================================
// LED КОНФИГУРАЦИЯ (из svetdiod-project)
// ============================================================================
#define NUM_STRIPS  10
#define MAX_LEDS    144
#define NUM_ZONES   15     // 15 зон для 15 блоков актуаторов

// idx:   0    1    2    3    4    5    6    7    8    9
// pin:   5   21   22   23   27   13   14   33    4   32
static const uint8_t  PIN_GPIO[NUM_STRIPS] = {  5, 21, 22, 23, 27, 13, 14, 33,   4, 32 };
static const uint16_t PIN_LEDS[NUM_STRIPS] = { 64, 33, 33, 33, 33, 33, 33, 33, 144, 33 };

// LED данные
static CRGB    leds[NUM_STRIPS][MAX_LEDS];
static bool    zoneMask[NUM_STRIPS][MAX_LEDS];

// Глобальные LED параметры
static uint8_t  gR = 0, gG = 150, gB = 255;    // Cyan по умолчанию
static uint8_t  gBri = 200;
static uint8_t  gFx = 0;  // 0=solid, 1=pulse, 2=rainbow

// ============================================================================
// MEGA SERIAL КОНФИГУРАЦИЯ
// ============================================================================
// MEGA #1: блоки 1-8 (100% SAFE PINS - избегаем 0,1,2,3,6-11,15,16,17,18,19!)
#define MEGA1_RX 26  // GPIO26 RX ← TX1 Mega #1 (pin 18) ✓ SAFE
#define MEGA1_TX 25  // GPIO25 TX → RX1 Mega #1 (pin 19) ✓ SAFE
HardwareSerial Mega1Serial(1); // Serial1

// MEGA #2: блоки 9-15 (100% SAFE PINS - используем input-only GPIO для RX)
#define MEGA2_RX 35  // GPIO35 RX ← TX1 Mega #2 (pin 18) ✓ SAFE (input-only)
#define MEGA2_TX 12  // GPIO12 TX → RX1 Mega #2 (pin 19) ✓ SAFE
HardwareSerial Mega2Serial(2); // Serial2

// ============================================================================
// МАППИНГ БЛОКОВ → LED ЗОН
// ============================================================================

// Структура LED зоны для блока
struct BlockZone {
  uint8_t blockNum;         // Номер блока (1-15)
  uint8_t ledStrip;         // Номер LED ленты (0-9)
  uint16_t ledStart;        // Начальный LED
  uint16_t ledCount;        // Количество LED
  CRGB currentColor;        // Текущий цвет
  uint8_t currentBri;       // Текущая яркость (0-255)
  bool isActive;            // Активна ли зона
};

// Массив зон для 15 блоков
BlockZone blockZones[NUM_ZONES];

// ============================================================================
// СИСТЕМА ОЧЕРЕДИ БЛОКОВ (для последовательной работы)
// ============================================================================

struct BlockQueueItem {
  uint8_t blockNum;
  String action;      // "up" или "down"
  int duration;       // Длительность в мс
  unsigned long startTime;
  bool isActive;
};

#define QUEUE_SIZE 15
BlockQueueItem blockQueue[QUEUE_SIZE];
int queueHead = 0;
int queueTail = 0;
int queueCount = 0;

BlockQueueItem* currentBlock = nullptr;  // Текущий работающий блок

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

  Serial.println("\n\n========================================");
  Serial.println("  RAMS UNIFIED CONTROLLER v2.0");
  Serial.println("  ESP32 + 2x Arduino Mega + WS2812B");
  Serial.println("========================================");

  // Инициализация LED
  setupLEDs();

  // Инициализация зон
  setupBlockZones();

  // Инициализация Serial связи с Mega
  setupMegaSerial();

  // Инициализация очереди
  initQueue();

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

  // Обработка очереди блоков
  processBlockQueue();

  // Обновление LED эффектов
  updateLEDEffects();

  // Чтение ответов от Mega
  readFromMega();

  delay(1);
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ LED
// ============================================================================

void setupLEDs() {
  Serial.println("[LED] Initializing FastLED...");

  // Добавляем все 10 LED strips
  FastLED.addLeds<WS2812B,  5, GRB>(leds[0], PIN_LEDS[0]);  // Inner circle
  FastLED.addLeds<WS2812B, 21, GRB>(leds[1], PIN_LEDS[1]);  // Ray 1
  FastLED.addLeds<WS2812B, 22, GRB>(leds[2], PIN_LEDS[2]);  // Ray 2
  FastLED.addLeds<WS2812B, 23, GRB>(leds[3], PIN_LEDS[3]);  // Ray 3
  FastLED.addLeds<WS2812B, 27, GRB>(leds[4], PIN_LEDS[4]);  // Ray 4
  FastLED.addLeds<WS2812B, 13, GRB>(leds[5], PIN_LEDS[5]);  // Ray 5
  FastLED.addLeds<WS2812B, 14, GRB>(leds[6], PIN_LEDS[6]);  // Ray 6
  FastLED.addLeds<WS2812B, 33, GRB>(leds[7], PIN_LEDS[7]);  // Ray 7
  FastLED.addLeds<WS2812B,  4, GRB>(leds[8], PIN_LEDS[8]);  // Mid circle
  FastLED.addLeds<WS2812B, 32, GRB>(leds[9], PIN_LEDS[9]);  // Ray 8

  FastLED.setBrightness(gBri);
  FastLED.clear(true);

  Serial.println("[LED] FastLED initialized (10 strips, 538 LEDs total) ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ ЗОН БЛОКОВ
// ============================================================================

void setupBlockZones() {
  Serial.println("[ZONES] Configuring block → LED zones mapping...");

  // Распределяем LED равномерно между блоками
  // Блоки 1-8 → лучи 1-8 (strips 1-7, 9)
  // Блоки 9-15 → внутренний круг + средний круг

  // Блоки 1-8: по одному лучу на блок
  uint8_t rayStrips[] = {1, 2, 3, 4, 5, 6, 7, 9};  // Индексы strip для лучей
  for (int i = 0; i < 8; i++) {
    blockZones[i].blockNum = i + 1;
    blockZones[i].ledStrip = rayStrips[i];
    blockZones[i].ledStart = 0;
    blockZones[i].ledCount = 33;  // Весь луч
    blockZones[i].currentColor = CRGB::Black;
    blockZones[i].currentBri = 0;
    blockZones[i].isActive = false;
  }

  // Блоки 9-15: делим внутренний (64) + средний (144) круги
  // Внутренний круг (strip 0): 64 LED / 4 = 16 LED на блок (блоки 9-12)
  for (int i = 0; i < 4; i++) {
    blockZones[8 + i].blockNum = 9 + i;
    blockZones[8 + i].ledStrip = 0;  // Inner circle
    blockZones[8 + i].ledStart = i * 16;
    blockZones[8 + i].ledCount = 16;
    blockZones[8 + i].currentColor = CRGB::Black;
    blockZones[8 + i].currentBri = 0;
    blockZones[8 + i].isActive = false;
  }

  // Средний круг (strip 8): 144 LED / 3 = 48 LED на блок (блоки 13-15)
  for (int i = 0; i < 3; i++) {
    blockZones[12 + i].blockNum = 13 + i;
    blockZones[12 + i].ledStrip = 8;  // Mid circle
    blockZones[12 + i].ledStart = i * 48;
    blockZones[12 + i].ledCount = 48;
    blockZones[12 + i].currentColor = CRGB::Black;
    blockZones[12 + i].currentBri = 0;
    blockZones[12 + i].isActive = false;
  }

  Serial.println("[ZONES] Block zones configured:");
  for (int i = 0; i < NUM_ZONES; i++) {
    Serial.printf("  Block %d → Strip %d, LEDs %d-%d (%d total)\n",
      blockZones[i].blockNum,
      blockZones[i].ledStrip,
      blockZones[i].ledStart,
      blockZones[i].ledStart + blockZones[i].ledCount - 1,
      blockZones[i].ledCount
    );
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
  Serial.println("[MEGA] Mega #1 ready on GPIO25/26 ✓ SAFE");

  Mega2Serial.begin(115200, SERIAL_8N1, MEGA2_RX, MEGA2_TX);
  delay(50);
  Serial.println("[MEGA] Mega #2 ready on GPIO12/35 ✓ SAFE");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ ОЧЕРЕДИ
// ============================================================================

void initQueue() {
  for (int i = 0; i < QUEUE_SIZE; i++) {
    blockQueue[i].blockNum = 0;
    blockQueue[i].action = "";
    blockQueue[i].duration = 0;
    blockQueue[i].startTime = 0;
    blockQueue[i].isActive = false;
  }
  queueHead = 0;
  queueTail = 0;
  queueCount = 0;
  currentBlock = nullptr;

  Serial.println("[QUEUE] Block queue initialized ✓");
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

  // Главная страница
  server.on("/", HTTP_GET, handleRoot);

  // API endpoints
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/block", HTTP_POST, handleBlockCommand);
  server.on("/api/blocks/stop", HTTP_POST, handleStopAll);
  server.on("/api/led/color", HTTP_POST, handleLEDColor);
  server.on("/api/led/brightness", HTTP_POST, handleLEDBrightness);

  server.begin();
  Serial.println("[SERVER] Web server started on port 80 ✓");
}

// ============================================================================
// ОБРАБОТЧИКИ WEB SERVER
// ============================================================================

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>RAMS</title></head><body style='background:#111;color:#fff;font-family:Arial;padding:20px'>";
  html += "<h1 style='color:#0ff'>RAMS CONTROLLER</h1>";
  html += "<p>Status: <span id='s'>OK</span> | Active: <span id='b'>-</span> | Queue: <span id='q'>0</span></p>";
  html += "<button onclick='stopAll()' style='background:#f00;color:#fff;padding:10px;margin:10px 0'>STOP ALL</button>";
  html += "<h2>LED</h2>";
  html += "<p>Brightness: <input type='range' id='bri' min='0' max='255' value='200' oninput='setBri(this.value)'>";
  html += " <span id='briV'>200</span></p>";
  html += "<p>Color: <input type='color' id='col' value='#0096ff' onchange='setCol(this.value)'></p>";
  html += "<h2>Blocks</h2>";

  for(int i = 1; i <= 15; i++) {
    html += "<div style='background:#222;padding:10px;margin:5px 0;border-radius:5px'>";
    html += "<b>Block " + String(i) + "</b> ";
    html += "<button onclick='cmd(" + String(i) + ",\"up\")' style='background:#0f0;color:#000;padding:8px;margin:2px'>UP</button>";
    html += "<button onclick='cmd(" + String(i) + ",\"down\")' style='background:#f60;color:#fff;padding:8px;margin:2px'>DOWN</button>";
    html += "<button onclick='cmd(" + String(i) + ",\"stop\")' style='background:#fa0;color:#000;padding:8px;margin:2px'>STOP</button>";
    html += "</div>";
  }

  html += "<script>";
  html += "async function cmd(b,a){fetch('/api/block',{method:'POST',headers:{'Content-Type':'application/json'},";
  html += "body:JSON.stringify({block:b,action:a,duration:5000})}).then(r=>r.json()).then(()=>upd()).catch(e=>alert(e));}";
  html += "async function stopAll(){fetch('/api/blocks/stop',{method:'POST'}).then(()=>upd()).catch(e=>alert(e));}";
  html += "async function setBri(v){document.getElementById('briV').textContent=v;";
  html += "fetch('/api/led/brightness',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({value:parseInt(v)})});}";
  html += "async function setCol(c){const r=parseInt(c.substr(1,2),16);const g=parseInt(c.substr(3,2),16);const b=parseInt(c.substr(5,2),16);";
  html += "fetch('/api/led/color',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({r:r,g:g,b:b})});}";
  html += "async function upd(){fetch('/api/status').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('s').textContent='OK';";
  html += "document.getElementById('b').textContent=d.currentBlock||'-';";
  html += "document.getElementById('q').textContent=d.queueSize;}).catch(()=>document.getElementById('s').textContent='ERR');}";
  html += "setInterval(upd,2000);upd();</script></body></html>";

  server.send(200, "text/html", html);
}

void handleStatus() {
  StaticJsonDocument<512> doc;

  doc["connected"] = WiFi.softAPgetStationNum();
  doc["ip"] = WiFi.softAPIP().toString();
  doc["uptime"] = millis();
  doc["queueSize"] = queueCount;
  doc["currentBlock"] = currentBlock ? currentBlock->blockNum : 0;
  doc["ledBrightness"] = gBri;

  JsonObject ledColor = doc.createNestedObject("ledColor");
  ledColor["r"] = gR;
  ledColor["g"] = gG;
  ledColor["b"] = gB;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleBlockCommand() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  int blockNum = doc["block"] | 0;
  String action = doc["action"] | "";
  int duration = doc["duration"] | 5000;

  if (blockNum < 1 || blockNum > 15) {
    server.send(400, "application/json", "{\"error\":\"Invalid block (1-15)\"}");
    return;
  }

  if (action != "up" && action != "down" && action != "stop") {
    server.send(400, "application/json", "{\"error\":\"Invalid action\"}");
    return;
  }

  // Добавить в очередь
  bool added = addToQueue(blockNum, action, duration);

  if (added) {
    server.send(200, "application/json", "{\"success\":true,\"queued\":true}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Queue full\"}");
  }
}

void handleStopAll() {
  Serial.println("[API] STOP ALL BLOCKS command");

  // Очистить очередь
  initQueue();

  // Отправить STOP на обе Mega
  sendStopAllToMega();

  // Выключить все LED зоны
  turnOffAllLEDZones();

  server.send(200, "application/json", "{\"success\":true}");
}

void handleLEDColor() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }

  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  gR = constrain(doc["r"] | 255, 0, 255);
  gG = constrain(doc["g"] | 255, 0, 255);
  gB = constrain(doc["b"] | 255, 0, 255);

  Serial.printf("[LED] Color changed to RGB(%d,%d,%d)\n", gR, gG, gB);

  server.send(200, "application/json", "{\"success\":true}");
}

void handleLEDBrightness() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }

  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  gBri = constrain(doc["value"] | 200, 0, 255);
  FastLED.setBrightness(gBri);

  Serial.printf("[LED] Brightness changed to %d\n", gBri);

  server.send(200, "application/json", "{\"success\":true}");
}

// ============================================================================
// УПРАВЛЕНИЕ ОЧЕРЕДЬЮ БЛОКОВ
// ============================================================================

bool addToQueue(int blockNum, String action, int duration) {
  if (queueCount >= QUEUE_SIZE) {
    Serial.println("[QUEUE] Queue full!");
    return false;
  }

  blockQueue[queueTail].blockNum = blockNum;
  blockQueue[queueTail].action = action;
  blockQueue[queueTail].duration = duration;
  blockQueue[queueTail].startTime = 0;
  blockQueue[queueTail].isActive = false;

  queueTail = (queueTail + 1) % QUEUE_SIZE;
  queueCount++;

  Serial.printf("[QUEUE] Added: Block %d %s (%dms) [Queue: %d]\n",
    blockNum, action.c_str(), duration, queueCount);

  return true;
}

void processBlockQueue() {
  // Если есть текущий блок - проверить таймер
  if (currentBlock != nullptr) {
    unsigned long elapsed = millis() - currentBlock->startTime;

    if (elapsed >= currentBlock->duration) {
      // Блок завершил работу
      Serial.printf("[QUEUE] Block %d finished (%s)\n",
        currentBlock->blockNum, currentBlock->action.c_str());

      // Если это был DOWN - выключить LED
      if (currentBlock->action == "down") {
        turnOffBlockLED(currentBlock->blockNum);
      }

      currentBlock = nullptr;
    }

    return;  // Ждём завершения текущего блока
  }

  // Если нет текущего блока и очередь не пуста - запустить следующий
  if (queueCount > 0) {
    currentBlock = &blockQueue[queueHead];
    currentBlock->startTime = millis();
    currentBlock->isActive = true;

    queueHead = (queueHead + 1) % QUEUE_SIZE;
    queueCount--;

    // Отправить команду на Mega
    sendToMega(currentBlock->blockNum, currentBlock->action, currentBlock->duration);

    // Управление LED
    if (currentBlock->action == "up") {
      turnOnBlockLED(currentBlock->blockNum);
    } else if (currentBlock->action == "down") {
      fadeBlockLED(currentBlock->blockNum);
    }

    Serial.printf("[QUEUE] Started: Block %d %s (%dms) [Remaining: %d]\n",
      currentBlock->blockNum, currentBlock->action.c_str(),
      currentBlock->duration, queueCount);
  }
}

// ============================================================================
// ОТПРАВКА КОМАНД НА MEGA
// ============================================================================

bool sendToMega(int blockNum, String action, int duration) {
  StaticJsonDocument<256> doc;
  doc["block"] = blockNum;
  doc["action"] = action;
  doc["duration"] = duration;

  String jsonString;
  serializeJson(doc, jsonString);

  // Выбираем Mega по номеру блока
  if (blockNum >= 1 && blockNum <= 8) {
    Mega1Serial.println(jsonString);
    Serial.print("[MEGA #1] → ");
    Serial.println(jsonString);
    return true;
  }
  else if (blockNum >= 9 && blockNum <= 15) {
    Mega2Serial.println(jsonString);
    Serial.print("[MEGA #2] → ");
    Serial.println(jsonString);
    return true;
  }

  return false;
}

void sendStopAllToMega() {
  StaticJsonDocument<128> doc;
  doc["block"] = 0;
  doc["action"] = "stop";

  String jsonString;
  serializeJson(doc, jsonString);

  Mega1Serial.println(jsonString);
  Mega2Serial.println(jsonString);

  Serial.println("[MEGA] STOP ALL sent to both Megas");
}

void readFromMega() {
  // Читаем от Mega #1
  if (Mega1Serial.available()) {
    String response = Mega1Serial.readStringUntil('\n');
    response.trim();
    if (response.length() > 0) {
      Serial.print("[MEGA #1] ← ");
      Serial.println(response);
    }
  }

  // Читаем от Mega #2
  if (Mega2Serial.available()) {
    String response = Mega2Serial.readStringUntil('\n');
    response.trim();
    if (response.length() > 0) {
      Serial.print("[MEGA #2] ← ");
      Serial.println(response);
    }
  }
}

// ============================================================================
// УПРАВЛЕНИЕ LED ЗОНАМИ
// ============================================================================

void turnOnBlockLED(int blockNum) {
  for (int i = 0; i < NUM_ZONES; i++) {
    if (blockZones[i].blockNum == blockNum) {
      blockZones[i].isActive = true;
      blockZones[i].currentColor = CRGB(gR, gG, gB);
      blockZones[i].currentBri = 255;

      // Включить LED немедленно
      setBlockZoneLEDs(i, CRGB(gR, gG, gB));
      FastLED.show();

      Serial.printf("[LED] Block %d zone ON (strip %d)\n", blockNum, blockZones[i].ledStrip);
      break;
    }
  }
}

void fadeBlockLED(int blockNum) {
  // При DOWN начинаем fade (будет обрабатываться в updateLEDEffects)
  for (int i = 0; i < NUM_ZONES; i++) {
    if (blockZones[i].blockNum == blockNum) {
      blockZones[i].isActive = true;  // Активна для fade
      blockZones[i].currentBri = 255; // Начинаем с макс яркости

      Serial.printf("[LED] Block %d zone FADING (strip %d)\n", blockNum, blockZones[i].ledStrip);
      break;
    }
  }
}

void turnOffBlockLED(int blockNum) {
  for (int i = 0; i < NUM_ZONES; i++) {
    if (blockZones[i].blockNum == blockNum) {
      blockZones[i].isActive = false;
      blockZones[i].currentColor = CRGB::Black;
      blockZones[i].currentBri = 0;

      // Выключить LED немедленно
      setBlockZoneLEDs(i, CRGB::Black);
      FastLED.show();

      Serial.printf("[LED] Block %d zone OFF (strip %d)\n", blockNum, blockZones[i].ledStrip);
      break;
    }
  }
}

void turnOffAllLEDZones() {
  for (int i = 0; i < NUM_ZONES; i++) {
    blockZones[i].isActive = false;
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

  // Обновляем 50 раз в секунду
  if (now - lastUpdate < 20) return;
  lastUpdate = now;

  bool needShow = false;

  // Обработка эффектов для каждой зоны
  for (int i = 0; i < NUM_ZONES; i++) {
    if (!blockZones[i].isActive) continue;

    // Если блок в режиме DOWN - делаем fade out
    if (currentBlock != nullptr &&
        currentBlock->blockNum == blockZones[i].blockNum &&
        currentBlock->action == "down") {

      // Постепенное затухание
      if (blockZones[i].currentBri > 0) {
        blockZones[i].currentBri -= 5;
        if (blockZones[i].currentBri < 0) blockZones[i].currentBri = 0;

        CRGB fadedColor = blockZones[i].currentColor;
        fadedColor.nscale8(blockZones[i].currentBri);

        setBlockZoneLEDs(i, fadedColor);
        needShow = true;
      }
    }
    // Если блок в режиме UP - эффект пульса (опционально)
    else if (gFx == 1 && currentBlock != nullptr &&
             currentBlock->blockNum == blockZones[i].blockNum) {

      // Пульсация
      uint8_t pulse = beatsin8(30, 150, 255);
      CRGB pulsedColor = blockZones[i].currentColor;
      pulsedColor.nscale8(pulse);

      setBlockZoneLEDs(i, pulsedColor);
      needShow = true;
    }
  }

  if (needShow) {
    FastLED.show();
  }
}
