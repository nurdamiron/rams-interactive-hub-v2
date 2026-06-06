/**
 * RAMS Kinetic Table — Shared Protocol Definitions
 * Used by ESP32 and both Arduino Mega controllers
 */

#ifndef RAMS_PROTOCOL_H
#define RAMS_PROTOCOL_H

// --- Block definitions ---
#define TOTAL_BLOCKS      15
#define OUTER_RING_START  1
#define OUTER_RING_END    7
#define INNER_RING_START  8
#define INNER_RING_END    14
#define CENTER_BLOCK      15

// --- Mega routing ---
#define MEGA1_BLOCK_START 1
#define MEGA1_BLOCK_END   8
#define MEGA2_BLOCK_START 9
#define MEGA2_BLOCK_END   15

// --- Actions ---
#define ACTION_UP    "UP"
#define ACTION_DOWN  "DOWN"
#define ACTION_STOP  "STOP"

// --- Serial baud ---
#define SERIAL_BAUD  115200

// --- Safety ---
#define DEADTIME_MS         50
#define ACTUATOR_TIMEOUT_MS 15000
#define HEARTBEAT_INTERVAL  2000
#define WIFI_TIMEOUT_MS     5000
#define STAGGER_DELAY_MS    300

// --- UDP ---
#define UDP_PORT 4210

// --- Relay logic ---
// true  = Active LOW (blue modules with optocoupler: LOW = ON)
// false = Active HIGH (simple relays: HIGH = ON)
#define RELAY_ACTIVE_LOW true

#if RELAY_ACTIVE_LOW
  #define RELAY_ON  LOW
  #define RELAY_OFF HIGH
#else
  #define RELAY_ON  HIGH
  #define RELAY_OFF LOW
#endif

#endif
