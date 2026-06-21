import os
import glob
import serial
import time
import sys

# ANSI Escape Sequences for terminal coloring
RED = "\033[1;31m"
GREEN = "\033[1;32m"
CYAN = "\033[1;36m"
YELLOW = "\033[1;33m"
RESET = "\033[0m"

print(f"{YELLOW}RAMS Actuator Identity Scanner...{RESET}")

# 1. Scan for active serial ports
ports = glob.glob("/dev/cu.usbmodem*") + glob.glob("/dev/cu.usbserial*")

if not ports:
    print(f"\n{RED}❌ NO ARDUINO MEGA DETECTED!{RESET}")
    print("Please make sure the board is connected to the laptop via a USB data cable.")
    sys.exit(1)

port = ports[0]
print(f"Found USB serial device: {CYAN}{port}{RESET}")
print("Opening connection and triggering reset (please wait)...")

try:
    ser = serial.Serial(port, 115200, timeout=3.5)
    time.sleep(2.5)  # Wait for Mega to auto-reset on connection
except Exception as e:
    print(f"{RED}❌ Error opening port: {e}{RESET}")
    sys.exit(1)

# Read startup messages
boot_output = []
while ser.in_waiting:
    try:
        line = ser.readline().decode('utf-8', errors='replace').strip()
        if line:
            boot_output.append(line)
    except Exception:
        pass

ser.close()

# 2. Identify the board from the boot logs
is_mega1 = False
is_mega2 = False

for line in boot_output:
    if "MEGA #1" in line:
        is_mega1 = True
    elif "MEGA #2" in line:
        is_mega2 = True

print("\n----------------------------------------")
if is_mega1:
    print(f"{RED}🔴 CONNECTED: ARDUINO MEGA #1 (Blocks 1-7){RESET}")
    print(f"Functions: Handles Nomad, Grande Vie, Keruen, Garden, Resort, Halic 2, Maslak.")
elif is_mega2:
    print(f"{CYAN}🔵 CONNECTED: ARDUINO MEGA #2 (Blocks 8-13){RESET}")
    print(f"Functions: Handles Sakura, Halic 1, Gaziantep, School, Hyatt, Almaty.")
else:
    print(f"{YELLOW}⚠️ CONNECTED: UNKNOWN DEVICE OR OLD FIRMWARE{RESET}")
    print("Startup logs received:")
    for line in boot_output:
        print(f"  > {line}")
print("----------------------------------------")
