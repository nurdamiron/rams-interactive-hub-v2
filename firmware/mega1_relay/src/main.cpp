/**
 * RAMS Kinetic Table — Arduino Mega #1 Firmware
 * Blocks 1–8 (Outer Ring 1–7 + Inner Block 8)
 * Serial1 (RX=19, TX=18) ← ESP32
 * Pins 22–53: 16 actuators (2 per block), each H-Bridge uses 2 pins
 * Total: 8 blocks × 2 actuators × 2 pins = 32 pins
 */

#include <Arduino.h>
#include "protocol.h"

struct Actuator {
  int pinUp, pinDown, state;
  unsigned long actionTime;
};

struct Block {
  int id;
  Actuator act1, act2;  // 2 актуатора на блок
};

// Пины 22-53 (32 пина)
Block blocks[8] = {
  // Block 1: pins 22-25
  {1, {22, 23, 0, 0}, {24, 25, 0, 0}},
  // Block 2: pins 26-29
  {2, {26, 27, 0, 0}, {28, 29, 0, 0}},
  // Block 3: pins 30-33
  {3, {30, 31, 0, 0}, {32, 33, 0, 0}},
  // Block 4: pins 34-37
  {4, {34, 35, 0, 0}, {36, 37, 0, 0}},
  // Block 5: pins 38-41 (SWAPPED with Block 7)
  {5, {38, 39, 0, 0}, {40, 41, 0, 0}},
  // Block 6: pins 50-53
  {6, {50, 51, 0, 0}, {52, 53, 0, 0}},
  // Block 7: pins 42-45 (SWAPPED with Block 5)
  {7, {42, 43, 0, 0}, {44, 45, 0, 0}},
  // Block 8: pins 46-49
  {8, {46, 47, 0, 0}, {48, 49, 0, 0}},
};
const int NUM_BLOCKS = 8;

void stopActuator(Actuator &act) {
  digitalWrite(act.pinUp, RELAY_OFF);
  digitalWrite(act.pinDown, RELAY_OFF);
  act.state = 0;
}

void stopBlock(int idx) {
  stopActuator(blocks[idx].act1);
  stopActuator(blocks[idx].act2);
}

void moveActuatorUp(Actuator &act) {
  if (act.state == 1) return;
  stopActuator(act);
  delay(DEADTIME_MS);
  digitalWrite(act.pinUp, RELAY_ON);
  act.state = 1;
  act.actionTime = millis();
}

void moveActuatorDown(Actuator &act) {
  if (act.state == -1) return;
  stopActuator(act);
  delay(DEADTIME_MS);
  digitalWrite(act.pinDown, RELAY_ON);
  act.state = -1;
  act.actionTime = millis();
}

void moveUp(int idx) {
  moveActuatorUp(blocks[idx].act1);
  delay(50);  // Небольшая задержка между актуаторами
  moveActuatorUp(blocks[idx].act2);
}

void moveDown(int idx) {
  moveActuatorDown(blocks[idx].act1);
  delay(50);
  moveActuatorDown(blocks[idx].act2);
}

void stopAll() {
  for (int i = 0; i < NUM_BLOCKS; i++) stopBlock(i);
}

int findBlock(int id) {
  for (int i = 0; i < NUM_BLOCKS; i++) if (blocks[i].id == id) return i;
  return -1;
}

void processCommand(String cmd) {
  // DEBUG: показываем что команда получена
  Serial.print("CMD: ");
  Serial.println(cmd);

  if (cmd == "PING") {
    Serial1.println("PONG");
    Serial.println("PONG");
    return;
  }
  if (cmd == "ALL:STOP" || cmd == "ESTOP") {
    stopAll();
    Serial1.println("ACK:ALL:STOP");
    Serial.println("ACK:ALL:STOP");
    return;
  }
  if (cmd.startsWith("ALL:")) {
    String action = cmd.substring(4);
    for (int i = 0; i < NUM_BLOCKS; i++) {
      if (action == ACTION_UP) moveUp(i);
      else if (action == ACTION_DOWN) moveDown(i);
      delay(STAGGER_DELAY_MS);
    }
    Serial1.println("ACK:" + cmd);
    Serial.println("ACK:" + cmd);
    return;
  }
  if (!cmd.startsWith("BLOCK:")) return;
  int first = cmd.indexOf(':');
  int second = cmd.indexOf(':', first + 1);
  if (second == -1) return;
  int blockId = cmd.substring(first + 1, second).toInt();
  String action = cmd.substring(second + 1);
  int idx = findBlock(blockId);
  if (idx == -1) {
    Serial1.println("ERR:UNKNOWN_BLOCK:" + String(blockId));
    Serial.println("ERR:UNKNOWN_BLOCK:" + String(blockId));
    return;
  }

  Serial.print("Moving block ");
  Serial.print(blockId);
  Serial.print(" (idx=");
  Serial.print(idx);
  Serial.print(") ");
  Serial.println(action);

  if (action == ACTION_UP) moveUp(idx);
  else if (action == ACTION_DOWN) moveDown(idx);
  else if (action == ACTION_STOP) stopBlock(idx);

  Serial1.println("ACK:BLOCK:" + String(blockId) + ":" + action);
  Serial.println("ACK:BLOCK:" + String(blockId) + ":" + action);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD);

  // Инициализируем все 32 пина (16 актуаторов × 2 пина)
  for (int i = 0; i < NUM_BLOCKS; i++) {
    // Актуатор 1
    pinMode(blocks[i].act1.pinUp, OUTPUT);
    pinMode(blocks[i].act1.pinDown, OUTPUT);
    digitalWrite(blocks[i].act1.pinUp, RELAY_OFF);
    digitalWrite(blocks[i].act1.pinDown, RELAY_OFF);

    // Актуатор 2
    pinMode(blocks[i].act2.pinUp, OUTPUT);
    pinMode(blocks[i].act2.pinDown, OUTPUT);
    digitalWrite(blocks[i].act2.pinUp, RELAY_OFF);
    digitalWrite(blocks[i].act2.pinDown, RELAY_OFF);
  }

  Serial.println("MEGA#1: Ready. Blocks 1-8 (16 actuators, 32 pins).");
}

void loop() {
  if (Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() > 0) processCommand(cmd);
  }
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() > 0) processCommand(cmd);
  }
  unsigned long now = millis();
  for (int i = 0; i < NUM_BLOCKS; i++) {
    // Проверяем timeout для актуатора 1
    if (blocks[i].act1.state != 0 && (now - blocks[i].act1.actionTime > ACTUATOR_TIMEOUT_MS)) {
      stopActuator(blocks[i].act1);
      Serial1.println("ERR:BLOCK:" + String(blocks[i].id) + ":ACT1:TIMEOUT");
    }
    // Проверяем timeout для актуатора 2
    if (blocks[i].act2.state != 0 && (now - blocks[i].act2.actionTime > ACTUATOR_TIMEOUT_MS)) {
      stopActuator(blocks[i].act2);
      Serial1.println("ERR:BLOCK:" + String(blocks[i].id) + ":ACT2:TIMEOUT");
    }
  }
}
