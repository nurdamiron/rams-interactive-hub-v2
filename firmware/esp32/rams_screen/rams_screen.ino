/**
 * RAMS Screen - Main Controller
 *
 * Кинетическая радиальная инсталляция с зональной подсветкой
 *
 * Оборудование:
 * - ESP32 (контроллер)
 * - 10 линий WS2811 (12V) - 8 лучей + 2 круга
 * - 31 линейный актуатор через H-мосты
 * - 15 релейных модулей (60 каналов = 15x4)
 *
 * Архитектура:
 * - 15 логических зон (секторы между лучами и кругами)
 * - Каждая зона = набор сегментов на разных физических линиях
 * - Управление через WiFi (HTTP API)
 *
 * @version 2.0
 * @date 2026-02-04
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#include "ZONE_CONFIG.h"
#include "ZONE_CONTROLLER.h"

// ============================================================================
// КОНФИГУРАЦИЯ WIFI
// ============================================================================

const char* WIFI_SSID = "RAMS_Screen";
const char* WIFI_PASSWORD = "RamsScreen2026";

IPAddress local_IP(192, 168, 4, 1);     // Статический IP
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// ============================================================================
// ГЛОБАЛЬНЫЕ ОБЪЕКТЫ
// ============================================================================

AsyncWebServer server(80);

// Состояние системы
String systemMode = "idle";              // idle, active, screensaver, test
int8_t activeZone = -1;                  // Текущая активная зона (-1 = нет)
unsigned long lastUpdate = 0;
uint8_t rainbowHue = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("    RAMS SCREEN - Kinetic Installation");
  Serial.println("    Zone-Based LED Controller v2.0");
  Serial.println("========================================");

  // 1. Инициализация LED зон
  ZoneController::init();
  printZoneConfiguration();

  // 2. Инициализация актуаторов
  ActuatorController::init();

  // 3. Подключение к WiFi
  setupWiFi();

  // 4. Запуск веб-сервера
  setupWebServer();

  // 5. Тестовая анимация запуска
  startupAnimation();

  Serial.println("\n[READY] System initialized!");
  Serial.println("========================================\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Обновление актуаторов (автостоп по таймеру)
  ActuatorController::update();

  // Обновление эффектов
  if (millis() - lastUpdate > 30) {  // ~30 FPS
    updateEffects();
    lastUpdate = millis();
  }

  delay(1);  // Стабильность
}

// ============================================================================
// WIFI SETUP
// ============================================================================

void setupWiFi() {
  Serial.println("[WIFI] Starting Access Point...");
  Serial.print("[WIFI] SSID: ");
  Serial.println(WIFI_SSID);

  // Режим точки доступа
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("[WIFI] AP IP address: ");
  Serial.println(IP);
  Serial.println("[WIFI] Access Point started ✓");
}

// ============================================================================
// WEB SERVER SETUP
// ============================================================================

void setupWebServer() {
  Serial.println("[SERVER] Starting web server...");

  // CORS headers
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  // OPTIONS для CORS preflight
  server.on("/*", HTTP_OPTIONS, [](AsyncWebServerRequest *request){
    request->send(200);
  });

  // GET / - Главная страница
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<h1>RAMS Screen Controller</h1>";
    html += "<p>Zone-Based LED System v2.0</p>";
    html += "<p>Zones: " + String(NUM_ZONES) + "</p>";
    html += "<p>Actuators: " + String(NUM_ACTUATORS) + "</p>";
    html += "<p><a href='/status'>Status</a> | <a href='/test'>Test Zones</a></p>";
    request->send(200, "text/html", html);
  });

  // GET /status - Статус системы
  server.on("/status", HTTP_GET, handleGetStatus);

  // POST /zone - Управление зоной
  server.on("/zone", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handlePostZone);

  // POST /actuator - Управление актуатором
  server.on("/actuator", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handlePostActuator);

  // GET /test - Тест всех зон
  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Starting zone test...");
    ZoneController::testAllZones(800);
  });

  // GET /clear - Выключить всё
  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request) {
    ZoneController::clearAll();
    ActuatorController::stopAll();
    activeZone = -1;
    systemMode = "idle";
    request->send(200, "text/plain", "All cleared");
  });

  // 404
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("[SERVER] Web server started on port 80 ✓");
}

// ============================================================================
// HTTP HANDLERS
// ============================================================================

void handleGetStatus(AsyncWebServerRequest *request) {
  JsonDocument doc;

  doc["connected"] = WiFi.softAPgetStationNum();  // Количество подключенных клиентов
  doc["ip"] = WiFi.softAPIP().toString();
  doc["uptime"] = millis();
  doc["mode"] = systemMode;
  doc["activeZone"] = activeZone;
  doc["numZones"] = NUM_ZONES;
  doc["numActuators"] = NUM_ACTUATORS;

  String response;
  serializeJson(doc, response);
  request->send(200, "application/json", response);
}

void handlePostZone(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, data, len);

  if (error) {
    request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  String action = doc["action"] | "set";
  uint8_t zoneId = doc["zoneId"] | 255;

  if (zoneId >= NUM_ZONES) {
    request->send(400, "application/json", "{\"error\":\"Invalid zone ID\"}");
    return;
  }

  // Выполнить действие
  if (action == "set") {
    String colorHex = doc["color"] | "#FFFFFF";
    uint8_t brightness = doc["brightness"] | BRIGHTNESS;
    String effect = doc["effect"] | "instant";

    CRGB color = parseHexColor(colorHex);

    if (effect == "fade") {
      ZoneController::fadeInZone(zoneId, color, 500);
    } else {
      ZoneController::setZoneColor(zoneId, color, brightness);
    }

    activeZone = zoneId;
    systemMode = "active";

    request->send(200, "application/json", "{\"success\":true}");
  }
  else if (action == "clear") {
    ZoneController::fadeOutZone(zoneId, 300);
    if (activeZone == zoneId) {
      activeZone = -1;
    }
    request->send(200, "application/json", "{\"success\":true}");
  }
  else {
    request->send(400, "application/json", "{\"error\":\"Unknown action\"}");
  }
}

void handlePostActuator(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, data, len);

  if (error) {
    request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  uint8_t actuatorId = doc["id"] | 255;
  String direction = doc["direction"] | "stop";
  uint16_t duration = doc["duration"] | 0;

  if (actuatorId >= NUM_ACTUATORS) {
    request->send(400, "application/json", "{\"error\":\"Invalid actuator ID\"}");
    return;
  }

  if (direction == "forward") {
    ActuatorController::moveForward(actuatorId, duration);
  } else if (direction == "backward") {
    ActuatorController::moveBackward(actuatorId, duration);
  } else {
    ActuatorController::stopActuator(actuatorId);
  }

  request->send(200, "application/json", "{\"success\":true}");
}

// ============================================================================
// ЭФФЕКТЫ
// ============================================================================

void updateEffects() {
  if (systemMode == "screensaver") {
    // Радужный эффект для всех зон
    for (uint8_t i = 0; i < NUM_ZONES; i++) {
      ZoneController::rainbowZone(i, rainbowHue);
    }
    rainbowHue++;
  }
  else if (systemMode == "active") {
    // Можно добавить динамические эффекты для активной зоны
    // Например, пульсация
    if (activeZone >= 0) {
      Zone* zone = getZone(activeZone);
      if (zone && zone->effect == "pulse") {
        ZoneController::pulseZone(activeZone, zone->currentColor);
      }
    }
  }
}

void startupAnimation() {
  Serial.println("[ANIMATION] Starting startup sequence...");

  // Быстрая пробежка по всем зонам
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    ZoneController::setZoneColor(i, CRGB::Blue);
    delay(50);
  }

  delay(200);
  ZoneController::clearAll();
  delay(100);

  // Двойная вспышка
  for (uint8_t flash = 0; flash < 2; flash++) {
    for (uint8_t i = 0; i < NUM_ZONES; i++) {
      ZoneController::setZoneColor(i, CRGB::White);
    }
    delay(100);
    ZoneController::clearAll();
    delay(100);
  }

  Serial.println("[ANIMATION] Startup complete");
}

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================

void printSystemInfo() {
  Serial.println("\n--- SYSTEM INFO ---");
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" sec");
  Serial.println("-------------------\n");
}
