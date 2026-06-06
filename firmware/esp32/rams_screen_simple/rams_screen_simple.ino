/**
 * RAMS Screen - Simple Test Version
 *
 * Упрощенная версия для быстрого тестирования
 * Одна линия LED для проверки концепции
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <FastLED.h>

// LED Configuration
#define LED_PIN           16
#define NUM_LEDS          200
#define LED_TYPE          WS2811
#define COLOR_ORDER       RGB
#define BRIGHTNESS        180

// WiFi Configuration
const char* WIFI_SSID = "RAMS_Screen_Test";
const char* WIFI_PASSWORD = "RamsTest2026";

// Globals
CRGB leds[NUM_LEDS];
AsyncWebServer server(80);
String systemMode = "idle";

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("  RAMS SCREEN - Test Version");
  Serial.println("========================================");

  // Init FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  Serial.println("[LED] FastLED initialized ✓");

  // Start WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("[WIFI] AP IP: ");
  Serial.println(IP);

  // Setup web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<h1>RAMS Screen Test</h1>";
    html += "<p><a href='/test'>Test LEDs</a></p>";
    html += "<p><a href='/clear'>Clear</a></p>";
    request->send(200, "text/html", html);
  });

  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Running LED test...");
    testLEDs();
  });

  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request) {
    FastLED.clear();
    FastLED.show();
    request->send(200, "text/plain", "Cleared");
  });

  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("r") && request->hasParam("g") && request->hasParam("b")) {
      int r = request->getParam("r")->value().toInt();
      int g = request->getParam("g")->value().toInt();
      int b = request->getParam("b")->value().toInt();

      fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
      FastLED.show();

      request->send(200, "text/plain", "Color set");
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });

  server.begin();
  Serial.println("[SERVER] Web server started ✓");

  // Startup animation
  startupAnimation();

  Serial.println("\n[READY] System ready!");
  Serial.println("========================================\n");
}

void loop() {
  delay(10);
}

void testLEDs() {
  Serial.println("[TEST] Starting LED test...");

  // Red
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(500);

  // Green
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(500);

  // Blue
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(500);

  // White
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
  delay(500);

  // Clear
  FastLED.clear();
  FastLED.show();

  Serial.println("[TEST] Complete");
}

void startupAnimation() {
  Serial.println("[ANIMATION] Startup...");

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    if (i % 10 == 0) {
      FastLED.show();
      delay(5);
    }
  }

  FastLED.show();
  delay(200);

  FastLED.clear();
  FastLED.show();

  Serial.println("[ANIMATION] Done");
}
