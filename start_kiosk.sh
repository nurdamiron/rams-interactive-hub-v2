#!/bin/bash
# RAMS Interactive Hub - Kiosk Startup Script (Mac/Linux)

# Get directory of this script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

echo "🚀 Starting RAMS Hub..."

# 1. Start Next.js Server (if not using built Electron app)
# Using pm2 if installed, otherwise npm start
if command -v pm2 &> /dev/null; then
    pm2 start npm --name "rams-hub" -- start
else
    # Fallback: start in background
    nohup npm start > custom_server.log 2>&1 &
fi

# Wait for server
echo "Waiting for server to be ready..."
until $(curl --output /dev/null --silent --head --fail http://localhost:3000); do
    printf '.'
    sleep 1
done

# 2. Launch Chrome in Kiosk Mode (Hardware Graphics Acceleration Disabled for stability optionally)
echo "Launching Kiosk..."

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    open -a "Google Chrome" --args --kiosk --incognito --disable-pinch --overscroll-history-navigation=0 "http://localhost:3000"
else
    # Linux (Raspberry Pi / Ubuntu)
    chromium-browser --kiosk --incognito --disable-pinch --overscroll-history-navigation=0 http://localhost:3000
fi
