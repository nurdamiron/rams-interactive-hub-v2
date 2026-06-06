#!/bin/bash
# OTA Upload script for ESP32
# Usage: ./upload_ota.sh [IP_ADDRESS]

IP="${1:-192.168.110.65}"
PASSWORD="rams2024"
FIRMWARE=".pio/build/esp32_master/firmware.bin"

echo "===================="
echo "OTA Upload to ESP32"
echo "===================="
echo "Target: $IP"
echo "Firmware: $FIRMWARE"
echo ""

# Build first
echo "Building firmware..."
pio run || exit 1
echo ""

# Upload via OTA
echo "Uploading via OTA..."
python3 ~/.platformio/packages/framework-arduinoespressif32/tools/espota.py \
  --ip="$IP" \
  --port=3232 \
  --auth="$PASSWORD" \
  --file="$FIRMWARE" \
  --timeout=60 \
  --progress

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ OTA Upload successful!"
    echo "ESP32 is rebooting..."
else
    echo ""
    echo "✗ OTA Upload failed!"
    echo "Try using USB upload instead: pio run -t upload --upload-port /dev/cu.usbserial-0001"
    exit 1
fi
