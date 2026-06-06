/**
 * RAMS Interactive Hub - ESP32 Hardware Controller
 *
 * Управляет:
 * - WS2812B LED лентами (подсветка проектов)
 * - Реле (12В нагрузка)
 * - Актуаторами (линейные приводы)
 *
 * Связь: WiFi (HTTP Server)
 * Протокол: JSON команды
 *
 * @version 1.0
 * @author RAMS Global Team
 * @date 2026-01-27
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <FastLED.h>

// ============================================================================
// КОНФИГУРАЦИЯ
// ============================================================================

// WiFi Settings
const char* WIFI_SSID = "RAMS_Hub";              // Название WiFi сети
const char* WIFI_PASSWORD = "RamsInteractive2026";  // Пароль
IPAddress local_IP(192, 168, 1, 100);            // Статический IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// LED Configuration
#define LED_PIN           16        // GPIO для WS2812B Data
#define NUM_LEDS          600       // Общее количество LED
#define LED_TYPE          WS2812B
#define COLOR_ORDER       GRB
#define BRIGHTNESS        200       // Яркость по умолчанию (0-255)
#define FRAMES_PER_SECOND 120

// Pin Configuration
#define STATUS_LED_PIN    2         // Встроенный LED ESP32
#define RELAY_1_PIN       17        // Реле #1
#define RELAY_2_PIN       18        // Реле #2
#define RELAY_3_PIN       19        // Реле #3
#define ACTUATOR_UP_PIN   21        // Актуатор вверх
#define ACTUATOR_DOWN_PIN 22        // Актуатор вниз
#define BUTTON_PIN        0         // Кнопка для тестирования

// System Configuration
#define MAX_PROJECTS      28        // Максимальное количество проектов
#define EFFECT_UPDATE_MS  20        // Частота обновления эффектов

// Serial Communication с Arduino Mega
// MEGA #1: блоки 1-8
#define MEGA1_RX 16  // GPIO16 для RX (подключаем к TX1 Mega #1)
#define MEGA1_TX 17  // GPIO17 для TX (подключаем к RX1 Mega #1)
HardwareSerial Mega1Serial(2); // Используем Serial2

// MEGA #2: блоки 9-15
#define MEGA2_RX 26  // GPIO26 для RX (подключаем к TX1 Mega #2)
#define MEGA2_TX 25  // GPIO25 для TX (подключаем к RX1 Mega #2)
HardwareSerial Mega2Serial(1); // Используем Serial1

// ============================================================================
// СТРУКТУРЫ ДАННЫХ
// ============================================================================

// Структура проекта
struct Project {
  int id;              // ID проекта (0-27)
  int ledStart;        // Начальный LED
  int ledCount;        // Количество LED
  CRGB color;          // Текущий цвет
  uint8_t brightness;  // Яркость (0-255)
  bool isActive;       // Активен ли проект
  String effect;       // Текущий эффект
};

// Структура актуатора
struct Actuator {
  String name;         // Имя (roof, lift, etc)
  int pinUp;          // Pin для движения вверх
  int pinDown;        // Pin для движения вниз
  String state;       // Текущее состояние (up, down, stop)
  unsigned long moveStartTime;  // Время начала движения
  int moveDuration;   // Длительность движения (мс)
};

// ============================================================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// ============================================================================

// FastLED
CRGB leds[NUM_LEDS];

// Web Server
AsyncWebServer server(80);

// Projects Configuration
Project projects[MAX_PROJECTS];
int currentProjectId = -1;  // Текущий активный проект

// Actuators
Actuator actuators[3] = {
  {"roof", ACTUATOR_UP_PIN, ACTUATOR_DOWN_PIN, "stop", 0, 5000},
  {"lift", RELAY_2_PIN, RELAY_2_PIN, "stop", 0, 3000},
  {"landscape", RELAY_3_PIN, RELAY_3_PIN, "stop", 0, 2000}
};

// System State
String systemMode = "idle";  // idle, highlight, screensaver, demo
bool isWiFiConnected = false;
unsigned long lastEffectUpdate = 0;
uint8_t rainbowHue = 0;

// ============================================================================
// SETUP - ИНИЦИАЛИЗАЦИЯ
// ============================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("  RAMS Interactive Hub - ESP32");
  Serial.println("  Hardware Controller v1.0");
  Serial.println("========================================");

  // Инициализация пинов
  setupPins();

  // Инициализация LED
  setupLEDs();

  // Инициализация Serial связи с Mega
  setupMegaSerial();

  // Инициализация проектов
  setupProjects();

  // Подключение к WiFi
  setupWiFi();

  // Запуск веб-сервера
  setupWebServer();

  // Готово
  Serial.println("\n[READY] System initialized successfully!");
  Serial.println("========================================\n");

  // Индикация готовности
  blinkStatusLED(3, 200);
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Обработка актуаторов
  updateActuators();

  // Обновление эффектов
  if (millis() - lastEffectUpdate > EFFECT_UPDATE_MS) {
    updateEffects();
    lastEffectUpdate = millis();
  }

  // Чтение ответов от Mega
  readFromMega();

  // Обновление статуса WiFi
  if (WiFi.status() != WL_CONNECTED && isWiFiConnected) {
    Serial.println("[WIFI] Connection lost! Reconnecting...");
    isWiFiConnected = false;
    setupWiFi();
  }

  delay(1);  // Small delay for stability
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ ПИНОВ
// ============================================================================

void setupPins() {
  Serial.println("[SETUP] Configuring GPIO pins...");

  // Status LED
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  // Relays (Active LOW)
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  digitalWrite(RELAY_1_PIN, HIGH);  // OFF
  digitalWrite(RELAY_2_PIN, HIGH);  // OFF
  digitalWrite(RELAY_3_PIN, HIGH);  // OFF

  // Actuators
  pinMode(ACTUATOR_UP_PIN, OUTPUT);
  pinMode(ACTUATOR_DOWN_PIN, OUTPUT);
  digitalWrite(ACTUATOR_UP_PIN, LOW);
  digitalWrite(ACTUATOR_DOWN_PIN, LOW);

  // Button (optional)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("[SETUP] GPIO pins configured ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ LED
// ============================================================================

void setupLEDs() {
  Serial.println("[SETUP] Initializing FastLED...");

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  Serial.print("[SETUP] FastLED initialized: ");
  Serial.print(NUM_LEDS);
  Serial.println(" LEDs ✓");

  // Test animation
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    if (i % 10 == 0) delay(5);  // Slower startup animation
  }
  FastLED.clear();
  FastLED.show();
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ ПРОЕКТОВ
// ============================================================================

void setupProjects() {
  Serial.println("[SETUP] Configuring projects...");

  // Пример конфигурации проектов
  // Можно настроить под конкретную установку

  int ledOffset = 0;
  int ledsPerProject = NUM_LEDS / MAX_PROJECTS;  // Равномерное распределение

  for (int i = 0; i < MAX_PROJECTS; i++) {
    projects[i].id = i;
    projects[i].ledStart = ledOffset;
    projects[i].ledCount = ledsPerProject;
    projects[i].color = CRGB::White;
    projects[i].brightness = BRIGHTNESS;
    projects[i].isActive = false;
    projects[i].effect = "none";

    ledOffset += ledsPerProject;
  }

  // Кастомные конфигурации для ключевых проектов
  // RAMS BEYOND (id=0)
  projects[0].ledCount = 50;
  projects[0].color = CRGB::Gold;

  // RAMS GARDEN (id=9)
  projects[9].ledStart = 200;
  projects[9].ledCount = 60;
  projects[9].color = CRGB::Green;

  // RAMS SIGNATURE (id=11)
  projects[11].ledStart = 300;
  projects[11].ledCount = 50;
  projects[11].color = CRGB::Blue;

  Serial.print("[SETUP] Configured ");
  Serial.print(MAX_PROJECTS);
  Serial.println(" projects ✓");
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ SERIAL СВЯЗИ С MEGA
// ============================================================================

void setupMegaSerial() {
  Serial.println("[SETUP] Initializing Mega serial communication...");

  // Mega #1 - Serial2 (блоки 1-8)
  Mega1Serial.begin(115200, SERIAL_8N1, MEGA1_RX, MEGA1_TX);
  delay(50);
  Serial.println("[SETUP] Mega #1 serial initialized on GPIO16(RX)/GPIO17(TX) ✓");

  // Mega #2 - Serial1 (блоки 9-15)
  Mega2Serial.begin(115200, SERIAL_8N1, MEGA2_RX, MEGA2_TX);
  delay(50);
  Serial.println("[SETUP] Mega #2 serial initialized on GPIO26(RX)/GPIO25(TX) ✓");
}

// Отправка команды на Arduino Mega
bool sendToMega(int blockNum, String action, int duration) {
  StaticJsonDocument<256> doc;
  doc["block"] = blockNum;
  doc["action"] = action;
  doc["duration"] = duration;

  String jsonString;
  serializeJson(doc, jsonString);

  // Выбираем нужную Mega по номеру блока
  if (blockNum >= 1 && blockNum <= 8) {
    // Блоки 1-8 → Mega #1
    Mega1Serial.println(jsonString);
    Serial.print("[MEGA #1] Sent: ");
    Serial.println(jsonString);
    return true;
  }
  else if (blockNum >= 9 && blockNum <= 15) {
    // Блоки 9-15 → Mega #2
    Mega2Serial.println(jsonString);
    Serial.print("[MEGA #2] Sent: ");
    Serial.println(jsonString);
    return true;
  }
  else if (blockNum == 0 && action == "stop") {
    // Остановить все блоки на ОБЕИХ платах
    Mega1Serial.println(jsonString);
    Mega2Serial.println(jsonString);
    Serial.println("[MEGA ALL] Sent STOP to both Megas");
    return true;
  }

  Serial.print("[ERROR] Invalid block number: ");
  Serial.println(blockNum);
  return false;
}

// Чтение ответов от обеих Mega
void readFromMega() {
  // Читаем от Mega #1
  if (Mega1Serial.available()) {
    String response = Mega1Serial.readStringUntil('\n');
    response.trim();

    if (response.length() > 0) {
      Serial.print("[MEGA #1] Response: ");
      Serial.println(response);

      // Можно парсить ответ и обрабатывать статус
      // StaticJsonDocument<256> doc;
      // deserializeJson(doc, response);
      // String status = doc["status"];
    }
  }

  // Читаем от Mega #2
  if (Mega2Serial.available()) {
    String response = Mega2Serial.readStringUntil('\n');
    response.trim();

    if (response.length() > 0) {
      Serial.print("[MEGA #2] Response: ");
      Serial.println(response);
    }
  }
}

// ============================================================================
// ПОДКЛЮЧЕНИЕ К WIFI
// ============================================================================

void setupWiFi() {
  Serial.println("[WIFI] Connecting to WiFi...");
  Serial.print("[WIFI] SSID: ");
  Serial.println(WIFI_SSID);

  // Установка статического IP
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("[WIFI] Failed to configure static IP");
  }

  // Подключение
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Ожидание подключения
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));  // Blink
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    isWiFiConnected = true;
    Serial.println("\n[WIFI] Connected successfully! ✓");
    Serial.print("[WIFI] IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    digitalWrite(STATUS_LED_PIN, HIGH);
  } else {
    Serial.println("\n[WIFI] Connection failed!");
    Serial.println("[WIFI] Starting in offline mode...");
    digitalWrite(STATUS_LED_PIN, LOW);
  }
}

// ============================================================================
// НАСТРОЙКА ВЕБ-СЕРВЕРА
// ============================================================================

void setupWebServer() {
  Serial.println("[SERVER] Starting web server...");

  // CORS headers
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  // GET /ping - Health check
  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"status\":\"ok\",\"uptime\":" + String(millis()) + "}");
  });

  // GET /status - System status
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleGetStatus(request);
  });

  // POST /command - Execute hardware command
  server.on("/command", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      handlePostCommand(request, data, len);
    }
  );

  // GET / - Root page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html",
      "<h1>RAMS Interactive Hub</h1>"
      "<p>ESP32 Hardware Controller v1.0</p>"
      "<p>Status: <b>Online</b></p>"
      "<p><a href='/status'>System Status</a></p>"
    );
  });

  // 404 Not Found
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("[SERVER] Web server started on port 80 ✓");
}

// ============================================================================
// ОБРАБОТЧИКИ КОМАНД
// ============================================================================

void handleGetStatus(AsyncWebServerRequest *request) {
  StaticJsonDocument<512> doc;

  doc["connected"] = isWiFiConnected;
  doc["ip"] = WiFi.localIP().toString();
  doc["uptime"] = millis();
  doc["mode"] = systemMode;
  doc["activeProject"] = currentProjectId;
  doc["ledCount"] = NUM_LEDS;
  doc["brightness"] = BRIGHTNESS;

  String response;
  serializeJson(doc, response);
  request->send(200, "application/json", response);
}

void handlePostCommand(AsyncWebServerRequest *request, uint8_t *data, size_t len) {
  // Parse JSON
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, data, len);

  if (error) {
    Serial.print("[ERROR] JSON parse failed: ");
    Serial.println(error.c_str());
    request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  // Extract command
  String action = doc["action"] | "";
  int projectId = doc["projectId"] | -1;
  String target = doc["target"] | "";
  String state = doc["state"] | "";

  Serial.print("[CMD] Action: ");
  Serial.print(action);
  if (projectId >= 0) {
    Serial.print(" | Project: ");
    Serial.print(projectId);
  }
  Serial.println();

  // Execute command
  bool success = executeCommand(action, projectId, target, state, doc);

  if (success) {
    request->send(200, "application/json", "{\"success\":true}");
  } else {
    request->send(500, "application/json", "{\"success\":false,\"error\":\"Command failed\"}");
  }
}

// ============================================================================
// ВЫПОЛНЕНИЕ КОМАНД
// ============================================================================

bool executeCommand(String action, int projectId, String target, String state, JsonDocument& data) {

  // HIGHLIGHT - Подсветить проект
  if (action == "highlight" || action == "select_project") {
    if (projectId < 0 || projectId >= MAX_PROJECTS) return false;
    return highlightProject(projectId, data);
  }

  // OFF - Выключить проект
  else if (action == "off") {
    if (projectId >= 0) {
      return turnOffProject(projectId);
    } else {
      return turnOffAll();
    }
  }

  // RESET - Сброс всего
  else if (action == "reset") {
    return resetAll();
  }

  // SCREENSAVER - Режим заставки
  else if (action == "screensaver") {
    return startScreensaver();
  }

  // ACTUATOR - Управление актуатором
  else if (action == "actuator") {
    // Проверяем, это команда для Mega или локальная
    if (target.startsWith("block_")) {
      // Извлекаем номер блока из target (например "block_1" -> 1)
      int blockNum = target.substring(6).toInt();
      int duration = data["data"]["duration"] | 12000;

      // Отправляем на Mega
      return sendToMega(blockNum, state, duration);
    } else {
      // Локальное управление через GPIO ESP32
      return controlActuator(target, state);
    }
  }

  // TOGGLE_ELEMENT - Переключение элемента
  else if (action == "toggle_element") {
    return toggleElement(target, state == "true" || state == "1");
  }

  else {
    Serial.print("[ERROR] Unknown action: ");
    Serial.println(action);
    return false;
  }
}

// ============================================================================
// КОМАНДЫ УПРАВЛЕНИЯ ПРОЕКТАМИ
// ============================================================================

bool highlightProject(int projectId, JsonDocument& data) {
  if (projectId < 0 || projectId >= MAX_PROJECTS) return false;

  // Выключить предыдущий проект
  if (currentProjectId >= 0 && currentProjectId != projectId) {
    turnOffProject(currentProjectId);
  }

  // Получить параметры
  uint8_t brightness = data["data"]["brightness"] | BRIGHTNESS;
  String colorHex = data["data"]["color"] | "#FFFFFF";
  String effect = data["data"]["effect"] | "fade";

  // Парсинг цвета
  CRGB color = parseColor(colorHex);

  // Установка параметров
  projects[projectId].color = color;
  projects[projectId].brightness = brightness;
  projects[projectId].effect = effect;
  projects[projectId].isActive = true;

  currentProjectId = projectId;
  systemMode = "highlight";

  // Применение эффекта
  if (effect == "fade") {
    fadeInProject(projectId);
  } else if (effect == "instant") {
    setProjectColor(projectId, color, brightness);
  } else {
    fadeInProject(projectId);  // По умолчанию fade
  }

  Serial.print("[PROJECT] Highlighted project #");
  Serial.print(projectId);
  Serial.print(" with color ");
  Serial.println(colorHex);

  return true;
}

bool turnOffProject(int projectId) {
  if (projectId < 0 || projectId >= MAX_PROJECTS) return false;

  projects[projectId].isActive = false;
  projects[projectId].effect = "none";

  fadeOutProject(projectId);

  if (currentProjectId == projectId) {
    currentProjectId = -1;
    systemMode = "idle";
  }

  Serial.print("[PROJECT] Turned off project #");
  Serial.println(projectId);

  return true;
}

bool turnOffAll() {
  Serial.println("[SYSTEM] Turning off all projects...");

  for (int i = 0; i < MAX_PROJECTS; i++) {
    projects[i].isActive = false;
    projects[i].effect = "none";
  }

  FastLED.clear();
  FastLED.show();

  currentProjectId = -1;
  systemMode = "idle";

  return true;
}

bool resetAll() {
  Serial.println("[SYSTEM] Resetting all hardware...");

  // Выключить LED
  turnOffAll();

  // Выключить реле
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);

  // Остановить актуаторы
  stopAllActuators();

  return true;
}

bool startScreensaver() {
  Serial.println("[SYSTEM] Starting screensaver mode...");

  systemMode = "screensaver";
  currentProjectId = -1;

  // Все проекты получают rainbow эффект
  for (int i = 0; i < MAX_PROJECTS; i++) {
    projects[i].isActive = true;
    projects[i].effect = "rainbow";
  }

  return true;
}

// ============================================================================
// ЭФФЕКТЫ LED
// ============================================================================

void setProjectColor(int projectId, CRGB color, uint8_t brightness) {
  int start = projects[projectId].ledStart;
  int count = projects[projectId].ledCount;

  for (int i = 0; i < count; i++) {
    leds[start + i] = color;
    leds[start + i].fadeLightBy(255 - brightness);
  }

  FastLED.show();
}

void fadeInProject(int projectId) {
  int start = projects[projectId].ledStart;
  int count = projects[projectId].ledCount;
  CRGB color = projects[projectId].color;

  for (int brightness = 0; brightness <= projects[projectId].brightness; brightness += 5) {
    for (int i = 0; i < count; i++) {
      leds[start + i] = color;
      leds[start + i].fadeLightBy(255 - brightness);
    }
    FastLED.show();
    delay(10);
  }
}

void fadeOutProject(int projectId) {
  int start = projects[projectId].ledStart;
  int count = projects[projectId].ledCount;
  uint8_t currentBrightness = projects[projectId].brightness;

  for (int brightness = currentBrightness; brightness >= 0; brightness -= 5) {
    for (int i = 0; i < count; i++) {
      leds[start + i].fadeLightBy(255 - brightness);
    }
    FastLED.show();
    delay(10);
  }

  // Полностью выключить
  for (int i = 0; i < count; i++) {
    leds[start + i] = CRGB::Black;
  }
  FastLED.show();
}

void updateEffects() {
  for (int i = 0; i < MAX_PROJECTS; i++) {
    if (!projects[i].isActive) continue;

    if (projects[i].effect == "pulse") {
      updatePulseEffect(i);
    } else if (projects[i].effect == "rainbow") {
      updateRainbowEffect(i);
    } else if (projects[i].effect == "sparkle") {
      updateSparkleEffect(i);
    }
  }

  FastLED.show();
}

void updatePulseEffect(int projectId) {
  static uint8_t pulseValue = 0;
  static int8_t pulseDirection = 1;

  int start = projects[projectId].ledStart;
  int count = projects[projectId].ledCount;
  CRGB color = projects[projectId].color;

  for (int i = 0; i < count; i++) {
    leds[start + i] = color;
    leds[start + i].fadeLightBy(255 - pulseValue);
  }

  pulseValue += pulseDirection * 5;
  if (pulseValue >= 255 || pulseValue <= 50) {
    pulseDirection *= -1;
  }
}

void updateRainbowEffect(int projectId) {
  int start = projects[projectId].ledStart;
  int count = projects[projectId].ledCount;

  for (int i = 0; i < count; i++) {
    leds[start + i] = CHSV(rainbowHue + (i * 10), 255, projects[projectId].brightness);
  }

  rainbowHue++;
}

void updateSparkleEffect(int projectId) {
  int start = projects[projectId].ledStart;
  int count = projects[projectId].ledCount;

  // Random sparkles
  if (random(0, 10) < 3) {
    int pos = random(0, count);
    leds[start + pos] = CRGB::White;
  }

  // Fade all
  for (int i = 0; i < count; i++) {
    leds[start + i].fadeToBlackBy(20);
  }
}

// ============================================================================
// УПРАВЛЕНИЕ АКТУАТОРАМИ
// ============================================================================

bool controlActuator(String name, String state) {
  for (int i = 0; i < 3; i++) {
    if (actuators[i].name == name) {
      return moveActuator(i, state);
    }
  }

  Serial.print("[ERROR] Actuator not found: ");
  Serial.println(name);
  return false;
}

bool moveActuator(int index, String state) {
  if (index < 0 || index >= 3) return false;

  Actuator* act = &actuators[index];

  // Stop
  if (state == "stop") {
    digitalWrite(act->pinUp, LOW);
    digitalWrite(act->pinDown, LOW);
    act->state = "stop";
    Serial.print("[ACTUATOR] Stopped: ");
    Serial.println(act->name);
    return true;
  }

  // Up
  if (state == "up") {
    digitalWrite(act->pinDown, LOW);
    digitalWrite(act->pinUp, HIGH);
    act->state = "up";
    act->moveStartTime = millis();
    Serial.print("[ACTUATOR] Moving UP: ");
    Serial.println(act->name);
    return true;
  }

  // Down
  if (state == "down") {
    digitalWrite(act->pinUp, LOW);
    digitalWrite(act->pinDown, HIGH);
    act->state = "down";
    act->moveStartTime = millis();
    Serial.print("[ACTUATOR] Moving DOWN: ");
    Serial.println(act->name);
    return true;
  }

  return false;
}

void updateActuators() {
  unsigned long now = millis();

  for (int i = 0; i < 3; i++) {
    Actuator* act = &actuators[i];

    // Check if actuator should stop (timeout)
    if (act->state != "stop") {
      if (now - act->moveStartTime >= act->moveDuration) {
        moveActuator(i, "stop");
      }
    }
  }
}

void stopAllActuators() {
  for (int i = 0; i < 3; i++) {
    moveActuator(i, "stop");
  }
}

// ============================================================================
// УПРАВЛЕНИЕ РЕЛЕ
// ============================================================================

bool toggleElement(String target, bool state) {
  int pin = -1;

  if (target == "light1") pin = RELAY_1_PIN;
  else if (target == "light2") pin = RELAY_2_PIN;
  else if (target == "light3") pin = RELAY_3_PIN;

  if (pin == -1) {
    Serial.print("[ERROR] Unknown relay target: ");
    Serial.println(target);
    return false;
  }

  // Реле активно LOW
  digitalWrite(pin, state ? LOW : HIGH);

  Serial.print("[RELAY] ");
  Serial.print(target);
  Serial.print(" -> ");
  Serial.println(state ? "ON" : "OFF");

  return true;
}

// ============================================================================
// УТИЛИТЫ
// ============================================================================

CRGB parseColor(String hexColor) {
  // Remove # if present
  if (hexColor.startsWith("#")) {
    hexColor = hexColor.substring(1);
  }

  // Parse hex
  long number = strtol(hexColor.c_str(), NULL, 16);
  int r = number >> 16;
  int g = (number >> 8) & 0xFF;
  int b = number & 0xFF;

  return CRGB(r, g, b);
}

void blinkStatusLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(delayMs);
  }
}
