void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 ALIVE!");
}

void loop() {
  Serial.println("Heartbeat...");
  delay(1000);
}
