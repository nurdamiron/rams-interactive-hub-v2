/**
 * RAMS Kinetic Table — Protocol Definitions
 * Shared between ESP32 and Arduino Mega controllers
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

// Serial communication
#define SERIAL_BAUD 115200

// Relay logic (active HIGH - INVERTED)
#define RELAY_ON HIGH
#define RELAY_OFF LOW

// Protocol commands
#define ACTION_UP "UP"
#define ACTION_DOWN "DOWN"
#define ACTION_STOP "STOP"

// Timing constants
#define DEADTIME_MS 50          // Dead time between relay switches
#define ACTUATOR_TIMEOUT_MS 15000  // Max actuator run time (15 seconds)
#define STAGGER_DELAY_MS 100    // Delay between blocks in ALL: commands

#endif
