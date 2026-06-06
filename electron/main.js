const { app, BrowserWindow, globalShortcut, protocol, ipcMain, dialog, net } = require('electron');
const { pathToFileURL } = require('url');
const path = require('path');
const fs = require('fs');
const dgram = require('dgram');
const http = require('http');
const os = require('os');
const WebSocket = require('ws');

// Register custom schemes
protocol.registerSchemesAsPrivileged([
  { scheme: 'media', privileges: { secure: true, standard: true, supportFetchAPI: true, corsEnabled: true, stream: true } },
  { scheme: 'app', privileges: { secure: true, standard: true, supportFetchAPI: true, corsEnabled: true } }
]);

let mainWindow;

// ===================== AUTO-LAUNCH ON WINDOWS STARTUP =====================
function setupAutoLaunch() {
  if (process.platform !== 'win32' || !app.isPackaged) return;

  const exePath = app.getPath('exe');
  const appName = 'RAMSInteractiveHub';

  try {
    // Use Windows registry to add to startup via reg command
    const { execSync } = require('child_process');
    const regKey = 'HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run';

    // Check if already registered
    try {
      const result = execSync(`reg query "${regKey}" /v "${appName}"`, { encoding: 'utf8' });
      if (result.includes(exePath)) {
        console.log('[AutoLaunch] Already registered in startup');
        return;
      }
    } catch (e) {
      // Key doesn't exist yet, proceed to create
    }

    execSync(`reg add "${regKey}" /v "${appName}" /t REG_SZ /d "${exePath}" /f`, { encoding: 'utf8' });
    console.log(`[AutoLaunch] Registered in Windows startup: ${exePath}`);
  } catch (e) {
    console.error(`[AutoLaunch] Failed to register: ${e.message}`);
  }
}

// ===================== END AUTO-LAUNCH =====================

// Setup file logging
const logFile = path.join(app.getPath('userData'), 'rams-app.log');
const logStream = fs.createWriteStream(logFile, { flags: 'a' });

function log(message) {
  const timestamp = new Date().toISOString();
  const logMessage = `[${timestamp}] ${message}\n`;
  console.log(message);
  try { logStream.write(logMessage); } catch (e) {}
}

log('=== RAMS Interactive Hub Starting ===');
log(`Platform: ${process.platform} | Arch: ${process.arch} | Electron: ${process.versions.electron} | Node: ${process.versions.node}`);
log(`Packaged: ${app.isPackaged} | AppPath: ${app.getAppPath()}`);
log(`UserData: ${app.getPath('userData')}`);
log(`LogFile: ${logFile}`);

// Catch errors
process.on('uncaughtException', (error) => {
  log(`UNCAUGHT EXCEPTION: ${error.message}\n${error.stack}`);
  dialog.showErrorBox('Error', `${error.message}\n\nLog: ${logFile}`);
});

process.on('unhandledRejection', (reason) => {
  log(`UNHANDLED REJECTION: ${reason}`);
});

// ===================== HTTP HARDWARE CLIENT =====================
const ESP32_PORT = 80;
const COMMAND_DEBOUNCE_MS = 150;
const HEALTH_CHECK_INTERVAL = 10000;
const BLOCK_DURATION_MS = 4000;  // 4 секунды для UP (синхронно с React)

const hardwareConfigPath = path.join(app.getPath('userData'), 'hardware-config.json');

let espIP = '192.168.4.1';  // IP адрес ESP32 Access Point
let blockMapping = {}; // { galleryCardId: physicalBlockNumber } — custom overrides

// ===================== LG TV SSAP CLIENT =====================
const MEDIA_SERVER_PORT = 8888;
const SSAP_PORT = 3001;        // webOS 6+ uses wss:// on port 3001 (not ws:// on 3000)

let tvIP = '';
let tvClientKey = '';
let tvWs = null;
let tvConnected = false;
let mediaServer = null;
let ssapRequestId = 1;
let currentBlock = null;
let activeBlocks = new Set(); // Track active blocks (max 2)
let lastCommandTime = 0;
let debounceTimer = null;
let healthCheckTimer = null;
let espConnected = false;
let lastPong = 0;

// Load saved hardware config
function loadHardwareConfig() {
  try {
    if (fs.existsSync(hardwareConfigPath)) {
      const config = JSON.parse(fs.readFileSync(hardwareConfigPath, 'utf8'));
      if (config.espIP) espIP = config.espIP;
      if (config.blockMapping) blockMapping = config.blockMapping;
      if (config.tvIP) tvIP = config.tvIP;
      if (config.tvClientKey) tvClientKey = config.tvClientKey;
      log(`[UDP] Loaded config: ESP=${espIP}, TV=${tvIP || 'not set'}, mapping keys=${Object.keys(blockMapping).length}`);
    }
  } catch (e) {
    log(`[UDP] Failed to load config: ${e.message}`);
  }
}

function saveHardwareConfig() {
  try {
    fs.writeFileSync(hardwareConfigPath, JSON.stringify({ espIP, blockMapping, tvIP, tvClientKey }, null, 2));
    log(`[Config] Saved: ESP=${espIP}, TV=${tvIP || 'not set'}`);
  } catch (e) {
    log(`[Config] Failed to save: ${e.message}`);
  }
}

// ===================== ESP32 AUTO-DISCOVERY =====================

// Scan local subnet for ESP32 by hitting /api/status on each IP
function discoverESP32() {
  return new Promise((resolve) => {
    const interfaces = os.networkInterfaces();
    const localIPs = [];

    // Find all local IPv4 addresses to determine subnets
    for (const name of Object.keys(interfaces)) {
      for (const iface of interfaces[name]) {
        if (iface.family === 'IPv4' && !iface.internal && iface.netmask === '255.255.255.0') {
          localIPs.push(iface.address);
        }
      }
    }

    if (localIPs.length === 0) {
      log('[Discovery] No suitable network interfaces found');
      resolve(null);
      return;
    }

    log(`[Discovery] Scanning subnets for ESP32... Local IPs: ${localIPs.join(', ')}`);

    let found = false;
    let pending = 0;

    for (const localIP of localIPs) {
      const subnet = localIP.split('.').slice(0, 3).join('.');
      log(`[Discovery] Scanning ${subnet}.1-254...`);

      for (let i = 1; i <= 254; i++) {
        const ip = `${subnet}.${i}`;
        if (ip === localIP) continue; // Skip self

        pending++;
        const url = `http://${ip}:${ESP32_PORT}/api/status`;

        const req = http.get(url, { timeout: 1500 }, (res) => {
          if (found) { req.destroy(); pending--; return; }

          let data = '';
          res.on('data', (chunk) => { data += chunk; });
          res.on('end', () => {
            pending--;
            if (found) return;
            if (res.statusCode === 200) {
              try {
                const json = JSON.parse(data);
                // Verify it's an ESP32 response (has "active" and "blocks" fields)
                if (json.hasOwnProperty('active') && json.hasOwnProperty('blocks')) {
                  found = true;
                  log(`[Discovery] ✅ Found ESP32 at ${ip} — status: ${data}`);
                  resolve(ip);
                }
              } catch (e) { /* not JSON, skip */ }
            }
            if (!found && pending === 0) resolve(null);
          });
        });

        req.on('error', () => { pending--; if (!found && pending === 0) resolve(null); });
        req.on('timeout', () => { req.destroy(); });
      }
    }

    // Safety timeout — stop after 10 seconds
    setTimeout(() => {
      if (!found) {
        log('[Discovery] ⏱ Scan timeout (10s)');
        resolve(null);
      }
    }, 10000);
  });
}

// Auto-discover and update ESP32 IP
async function autoDiscoverESP32() {
  log('[Discovery] Starting auto-discovery...');
  const discoveredIP = await discoverESP32();

  if (discoveredIP) {
    if (discoveredIP !== espIP) {
      log(`[Discovery] ESP32 IP changed: ${espIP} → ${discoveredIP}`);
      espIP = discoveredIP;
      saveHardwareConfig();
    }
    espConnected = true;
    lastPong = Date.now();
    return true;
  }

  log('[Discovery] ESP32 not found on network');
  return false;
}

// ===================== END AUTO-DISCOVERY =====================

// Initialize HTTP client
function initHttp() {
  loadHardwareConfig();
  log(`[HTTP] Initialized - ESP32 at http://${espIP}:${ESP32_PORT}`);

  // Try current saved IP first, if fails — auto-discover
  const url = `http://${espIP}:${ESP32_PORT}/api/status`;
  const req = http.get(url, { timeout: 3000 }, (res) => {
    if (res.statusCode === 200) {
      espConnected = true;
      lastPong = Date.now();
      log(`[HTTP] ESP32 reachable at saved IP ${espIP}`);
      startHealthCheck();
    }
  });
  req.on('error', () => {
    log(`[HTTP] Saved IP ${espIP} unreachable — starting auto-discovery...`);
    autoDiscoverESP32().then(() => startHealthCheck());
  });
  req.on('timeout', () => {
    req.destroy();
    log(`[HTTP] Saved IP ${espIP} timeout — starting auto-discovery...`);
    autoDiscoverESP32().then(() => startHealthCheck());
  });
}

// Send HTTP request to ESP32
function sendHttpRequest(endpoint, params = {}) {
  return new Promise((resolve) => {
    const queryString = Object.keys(params).map(key => `${key}=${encodeURIComponent(params[key])}`).join('&');
    const url = `http://${espIP}:${ESP32_PORT}${endpoint}${queryString ? '?' + queryString : ''}`;

    const options = {
      method: 'POST',
      timeout: 5000,
    };

    log(`[HTTP] Request: ${url}`);

    const req = http.request(url, options, (res) => {
      let data = '';
      res.on('data', (chunk) => { data += chunk; });
      res.on('end', () => {
        if (res.statusCode === 200) {
          log(`[HTTP] Success: ${data}`);
          espConnected = true;
          lastPong = Date.now();
          lastCommandTime = Date.now();
          resolve(true);
        } else if (res.statusCode === 429) {
          log(`[HTTP] Error 429: Maximum 2 blocks active`);
          resolve(false);
        } else {
          log(`[HTTP] Error ${res.statusCode}: ${data}`);
          resolve(false);
        }
      });
    });

    req.on('error', (err) => {
      log(`[HTTP] Request error: ${err.message}`);
      espConnected = false;
      resolve(false);
    });

    req.on('timeout', () => {
      log(`[HTTP] Request timeout`);
      req.destroy();
      espConnected = false;
      resolve(false);
    });

    req.end();
  });
}

// Debounced block command - with HTTP API
let pendingResolve = null;

function sendDebouncedBlockCommand(block, action) {
  return new Promise((resolve) => {
    if (debounceTimer) {
      clearTimeout(debounceTimer);
      if (pendingResolve) {
        pendingResolve(false);
        pendingResolve = null;
      }
    }

    pendingResolve = resolve;

    const timeSinceLast = Date.now() - lastCommandTime;
    const delay = Math.max(0, COMMAND_DEBOUNCE_MS - timeSinceLast);

    debounceTimer = setTimeout(async () => {
      debounceTimer = null;
      pendingResolve = null;

      // Отправляем HTTP запрос с длительностью 10 секунд
      const success = await sendHttpRequest('/api/block', {
        num: block,
        action: action.toLowerCase(),
        duration: BLOCK_DURATION_MS
      });

      if (success) {
        if (action === 'UP') currentBlock = block;
        else if (action === 'DOWN' && currentBlock === block) currentBlock = null;
      }

      resolve(success);
    }, delay);
  });
}

// Health check — ping ESP32 периодически через HTTP
function startHealthCheck() {
  if (healthCheckTimer) clearInterval(healthCheckTimer);
  healthCheckTimer = setInterval(async () => {
    // Проверяем статус ESP32
    try {
      const url = `http://${espIP}:${ESP32_PORT}/api/status`;
      const req = http.get(url, (res) => {
        if (res.statusCode === 200) {
          espConnected = true;
          lastPong = Date.now();
        }
      });
      req.on('error', () => { espConnected = false; });
      req.on('timeout', () => { req.destroy(); espConnected = false; });
      req.setTimeout(3000);
    } catch (e) {
      espConnected = false;
    }

    // Mark as disconnected if no response for 30s — trigger re-discovery
    if (lastPong > 0 && Date.now() - lastPong > 30000) {
      if (espConnected) {
        espConnected = false;
        log('[HealthCheck] ESP32 lost — triggering auto-discovery...');
        autoDiscoverESP32();
      }
    }
  }, HEALTH_CHECK_INTERVAL);
}

// Cleanup HTTP resources
function cleanupHttp() {
  if (debounceTimer) {
    clearTimeout(debounceTimer);
    debounceTimer = null;
  }
  if (healthCheckTimer) {
    clearInterval(healthCheckTimer);
    healthCheckTimer = null;
  }
  log('[HTTP] Cleanup complete');
}

// ===================== END UDP =====================

// ===================== LG TV SSAP FUNCTIONS =====================

// Get local IPv4 address (for media server URL)
function getLocalIP() {
  const interfaces = os.networkInterfaces();
  for (const name of Object.keys(interfaces)) {
    for (const iface of interfaces[name]) {
      if (iface.family === 'IPv4' && !iface.internal) {
        return iface.address;
      }
    }
  }
  return '127.0.0.1';
}

// Start HTTP media server to serve video files to TV
function startMediaServer() {
  if (mediaServer) return;

  const videoMime = {
    '.mp4': 'video/mp4',
    '.webm': 'video/webm',
    '.mov': 'video/quicktime',
    '.mkv': 'video/x-matroska',
  };

  mediaServer = http.createServer((req, res) => {
    const mediaRoot = getMediaRoot();
    const rawUrl = req.url || '/';

    // Special route: /player?video=... — HTML5 video player page for LG TV browser
    if (rawUrl.startsWith('/player')) {
      const queryIndex = rawUrl.indexOf('?');
      const query = queryIndex !== -1 ? rawUrl.slice(queryIndex + 1) : '';
      const params = new URLSearchParams(query);
      const videoRelPath = params.get('video') || '';
      const localIP = getLocalIP();
      const videoSrc = videoRelPath
        ? `http://${localIP}:${MEDIA_SERVER_PORT}/${videoRelPath.replace(/^\/+/, '')}`
        : '';

      const html = `<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>RAMS Video</title>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; }
  html, body { width: 100%; height: 100%; background: #000; overflow: hidden; }
  video { position: fixed; top: 0; left: 0; width: 100%; height: 100%; object-fit: contain; }
</style>
</head>
<body>
  <video id="v" autoplay playsinline>
    <source src="${videoSrc}" type="video/mp4">
  </video>
  <script>
    var v = document.getElementById('v');
    v.play().catch(function() {
      document.body.addEventListener('click', function() { v.play(); }, { once: true });
    });
  </script>
</body>
</html>`;

      res.writeHead(200, {
        'Content-Type': 'text/html; charset=utf-8',
        'Access-Control-Allow-Origin': '*',
      });
      res.end(html);
      log(`[MediaServer] Served player page for: ${videoRelPath}`);
      return;
    }

    const urlPath = decodeURIComponent(rawUrl.split('?')[0]);
    const filePath = path.join(mediaRoot, urlPath);

    // Security: prevent directory traversal
    if (!path.normalize(filePath).startsWith(mediaRoot)) {
      log(`[MediaServer] Blocked traversal: ${filePath}`);
      res.writeHead(403);
      res.end();
      return;
    }

    if (!fs.existsSync(filePath)) {
      log(`[MediaServer] Not found: ${filePath}`);
      res.writeHead(404);
      res.end();
      return;
    }

    const stat = fs.statSync(filePath);
    const ext = path.extname(filePath).toLowerCase();
    const contentType = videoMime[ext] || 'application/octet-stream';

    // Range request support (required for video seeking on TV)
    const range = req.headers.range;
    if (range) {
      const parts = range.replace(/bytes=/, '').split('-');
      const start = parseInt(parts[0], 10);
      const end = parts[1] ? parseInt(parts[1], 10) : stat.size - 1;
      const chunkSize = end - start + 1;
      const stream = fs.createReadStream(filePath, { start, end });
      res.writeHead(206, {
        'Content-Range': `bytes ${start}-${end}/${stat.size}`,
        'Accept-Ranges': 'bytes',
        'Content-Length': chunkSize,
        'Content-Type': contentType,
        'Access-Control-Allow-Origin': '*',
      });
      stream.pipe(res);
    } else {
      res.writeHead(200, {
        'Content-Length': stat.size,
        'Content-Type': contentType,
        'Accept-Ranges': 'bytes',
        'Access-Control-Allow-Origin': '*',
      });
      fs.createReadStream(filePath).pipe(res);
    }

    log(`[MediaServer] ${req.method} ${urlPath} → ${res.statusCode}`);
  });

  mediaServer.listen(MEDIA_SERVER_PORT, '0.0.0.0', () => {
    log(`[MediaServer] Started on http://0.0.0.0:${MEDIA_SERVER_PORT}`);
  });

  mediaServer.on('error', (err) => {
    log(`[MediaServer] Error: ${err.message}`);
  });
}

// Connect to LG TV via SSAP WebSocket
function connectToTV() {
  if (tvWs) {
    try { tvWs.close(); } catch (e) {}
    tvWs = null;
    tvConnected = false;
  }

  if (!tvIP) {
    log('[TV] No TV IP configured');
    return false;
  }

  try {
    log(`[TV] Connecting to wss://${tvIP}:${SSAP_PORT}...`);
    // LG webOS 6+ uses wss:// with self-signed cert — must disable cert check
    tvWs = new WebSocket(`wss://${tvIP}:${SSAP_PORT}`, {
      rejectUnauthorized: false,
    });

    tvWs.on('open', () => {
      log(`[TV] WebSocket connected to ${tvIP}:${SSAP_PORT}`);
      // Send SSAP registration
      const registerMsg = {
        type: 'register',
        id: `reg_${ssapRequestId++}`,
        payload: {
          pairingType: 'PROMPT',
          'manifest': {
            permissions: [
              'LAUNCH', 'LAUNCH_WEBAPP', 'CONTROL_AUDIO',
              'CONTROL_INPUT_MEDIA_PLAYBACK', 'READ_INSTALLED_APPS',
              'CONTROL_POWER', 'READ_TV_CURRENT_TIME',
              'CONTROL_DISPLAY', 'CONTROL_INPUT_TV',
            ],
          },
          ...(tvClientKey ? { 'client-key': tvClientKey } : {}),
        },
      };
      tvWs.send(JSON.stringify(registerMsg));
      log('[TV] Registration request sent' + (tvClientKey ? ' (with saved key)' : ' (first time — check TV for prompt)'));
    });

    tvWs.on('message', (data) => {
      try {
        const msg = JSON.parse(data.toString());
        log(`[TV] SSAP response: type=${msg.type}, id=${msg.id}`);

        if (msg.type === 'registered') {
          tvConnected = true;
          if (msg.payload && msg.payload['client-key']) {
            tvClientKey = msg.payload['client-key'];
            saveHardwareConfig();
            log(`[TV] Paired! Client key saved.`);
          }
          // Notify renderer
          if (mainWindow) {
            mainWindow.webContents.send('tv-status-changed', { connected: true, ip: tvIP });
          }
        }

        if (msg.type === 'error') {
          log(`[TV] SSAP Error: ${msg.error}`);
        }
      } catch (e) {
        log(`[TV] Parse error: ${e.message}`);
      }
    });

    tvWs.on('close', () => {
      log('[TV] WebSocket disconnected');
      tvConnected = false;
      tvWs = null;
      if (mainWindow) {
        mainWindow.webContents.send('tv-status-changed', { connected: false, ip: tvIP });
      }
    });

    tvWs.on('error', (err) => {
      log(`[TV] WebSocket error: ${err.message}`);
      tvConnected = false;
    });

    return true;
  } catch (e) {
    log(`[TV] Connection failed: ${e.message}`);
    return false;
  }
}

// Send SSAP request to TV
function sendSSAP(uri, payload = {}) {
  return new Promise((resolve) => {
    if (!tvWs || tvWs.readyState !== WebSocket.OPEN) {
      log(`[TV] Cannot send — not connected`);
      resolve(false);
      return;
    }

    const msg = {
      type: 'request',
      id: `req_${ssapRequestId++}`,
      uri,
      payload,
    };

    try {
      tvWs.send(JSON.stringify(msg));
      log(`[TV] SSAP → ${uri}`);
      resolve(true);
    } catch (e) {
      log(`[TV] Send error: ${e.message}`);
      resolve(false);
    }
  });
}

// Play video on LG TV
// Strategy:
//   1. Try ssap://media.viewer/open (works on webOS 4-5)
//   2. Fallback: open LG TV browser with our HTML5 player page (works on all webOS versions)
async function playVideoOnTV(videoRelPath) {
  // Ensure media server is running
  startMediaServer();

  const localIP = getLocalIP();

  // Strip leading slashes to avoid double-slash in URL
  const cleanPath = videoRelPath.replace(/^\/+/, '');
  const videoUrl = `http://${localIP}:${MEDIA_SERVER_PORT}/${cleanPath}`;
  // HTML5 player page URL (served by our media server at /player?video=...)
  const playerUrl = `http://${localIP}:${MEDIA_SERVER_PORT}/player?video=${encodeURIComponent(cleanPath)}`;

  log(`[TV] Playing on TV:`);
  log(`[TV]   videoUrl   = ${videoUrl}`);
  log(`[TV]   playerUrl  = ${playerUrl}`);

  if (!tvWs || tvWs.readyState !== WebSocket.OPEN) {
    log('[TV] Not connected — cannot play video');
    return false;
  }

  // Method 1: ssap://media.viewer/open (webOS 4-5)
  // Try it first, then also open browser as fallback (webOS 6+)
  const mediaViewerSent = await sendSSAP('ssap://media.viewer/open', {
    url: videoUrl,
    title: 'RAMS Global',
    description: '',
    mimeType: 'video/mp4',
    loop: false,
  });

  log(`[TV] media.viewer/open sent: ${mediaViewerSent}`);

  // Method 2 (always): Open browser with our HTML5 player page
  // This works on ALL webOS versions regardless of media.viewer support
  const browserSent = await sendSSAP('ssap://system.launcher/open', {
    id: 'com.webos.app.browser',
    params: { target: playerUrl },
  });

  log(`[TV] browser launcher sent: ${browserSent}`);

  return mediaViewerSent || browserSent;
}

// Stop video on TV
async function stopVideoOnTV() {
  // Close media viewer (webOS 4-5)
  await sendSSAP('ssap://media.viewer/close');
  // Close browser app (webOS 6+)
  await sendSSAP('ssap://system.launcher/close', { id: 'com.webos.app.browser' });
  log('[TV] Stop video sent');
  return true;
}

// Cleanup TV resources
function cleanupTV() {
  if (tvWs) {
    try { tvWs.close(); } catch (e) {}
    tvWs = null;
  }
  if (mediaServer) {
    try { mediaServer.close(); } catch (e) {}
    mediaServer = null;
  }
  tvConnected = false;
  log('[TV] Cleanup complete');
}

// ===================== END LG TV =====================

// Get media root path - searches multiple locations
function getMediaRoot() {
  const isDev = !app.isPackaged;
  if (isDev) {
    // Dev: check media/ folder first (project-level), then fallback to public/
    const devMedia = path.join(__dirname, '..', 'media');
    if (fs.existsSync(path.join(devMedia, 'projects'))) {
      return devMedia;
    }
    return path.join(__dirname, '..', 'public');
  }

  // Production: search for media/projects folder in multiple locations
  const exeDir = path.dirname(app.getPath('exe'));

  // On macOS the exe is inside .app bundle, so go up
  const appDir = process.platform === 'darwin'
    ? path.join(exeDir, '..', '..')  // RAMS.app/Contents/MacOS -> RAMS.app/../
    : exeDir;

  // Prioritize resources/media (extraResources from build), then external locations
  const candidates = [
    { label: 'resources/media', path: path.join(process.resourcesPath, 'media') },
    { label: '[exe]/media', path: path.join(exeDir, 'media') },
    { label: '[app]/media', path: path.join(appDir, 'media') },
    { label: '[exe]/../media', path: path.join(exeDir, '..', 'media') },
    { label: '[exe]/../../media', path: path.join(exeDir, '..', '..', 'media') },
  ];

  log(`[MediaSearch] ExeDir: ${exeDir}`);
  log(`[MediaSearch] AppDir: ${appDir}`);
  log(`[MediaSearch] ResourcesPath: ${process.resourcesPath}`);

  for (const candidate of candidates) {
    const projectsDir = path.join(candidate.path, 'projects');
    const exists = fs.existsSync(projectsDir);
    log(`[MediaSearch] ${candidate.label}: ${candidate.path} → projects/ ${exists ? 'FOUND' : 'not found'}`);
    if (exists) {
      try {
        const contents = fs.readdirSync(projectsDir);
        log(`[MediaSearch] Projects found: ${contents.join(', ')}`);
        // Verify folder actually has content (check first project has files)
        if (contents.length > 0) {
          const firstProject = path.join(projectsDir, contents[0]);
          const hasFiles = fs.statSync(firstProject).isDirectory() && fs.readdirSync(firstProject).length > 0;
          if (!hasFiles) {
            log(`[MediaSearch] ✗ Skipping (empty project dirs): ${candidate.path}`);
            continue;
          }
        }
      } catch (e) {
        log(`[MediaSearch] ✗ Error reading: ${e.message}`);
        continue;
      }
      log(`[MediaSearch] ✓ Using: ${candidate.path}`);
      return candidate.path;
    }
  }

  // Fallback to extraResources location
  const fallback = path.join(process.resourcesPath, 'media');
  log(`[MediaSearch] ✗ No media found! Fallback: ${fallback}`);
  return fallback;
}

function createWindow() {
  log('Creating main window...');

  mainWindow = new BrowserWindow({
    width: 1920,
    height: 1080,
    // KIOSK MODE
    kiosk: true,
    frame: false,
    fullscreen: true,
    autoHideMenuBar: true,
    alwaysOnTop: false,
    backgroundColor: '#000000',
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      webSecurity: true,
      preload: path.join(__dirname, 'preload.js'),
    },
  });

  // Load app
  const isDev = !app.isPackaged;
  if (isDev) {
    log('DEV: Loading http://localhost:3000');
    mainWindow.loadURL('http://localhost:3000');
  } else {
    log('PROD: Loading app://./index.html');
    mainWindow.loadURL('app://./index.html');
  }

  mainWindow.webContents.on('did-finish-load', () => {
    log('[Window] Page loaded successfully');
    log(`[Window] URL: ${mainWindow.webContents.getURL()}`);
  });

  mainWindow.webContents.on('console-message', (event, level, message, line, sourceId) => {
    const levels = ['LOG', 'WARN', 'ERROR'];
    log(`[Renderer ${levels[level] || level}] ${message}`);
  });

  mainWindow.webContents.on('did-fail-load', (event, errorCode, errorDescription, validatedURL) => {
    log(`[Window] Page load FAILED: ${errorCode} - ${errorDescription} (URL: ${validatedURL})`);
    // Retry after 2 seconds in case dev server isn't ready
    if (isDev) {
      setTimeout(() => mainWindow.loadURL('http://localhost:3000'), 2000);
    }
  });

  // Keyboard shortcuts
  // Ctrl/Cmd+Q - quit app
  globalShortcut.register('CommandOrControl+Q', () => app.quit());
  // Ctrl/Cmd+Shift+A - open admin panel
  globalShortcut.register('CommandOrControl+Shift+A', () => {
    mainWindow.webContents.send('open-admin');
  });
  // F5 - reload
  globalShortcut.register('F5', () => mainWindow.reload());

  // Disable right-click in kiosk mode
  mainWindow.setMenuBarVisibility(false);
  mainWindow.webContents.on('context-menu', (e) => e.preventDefault());

  // Prevent new windows (external links etc.)
  mainWindow.webContents.setWindowOpenHandler(() => ({ action: 'deny' }));

  mainWindow.on('closed', () => { mainWindow = null; });
}

// App ready
app.whenReady().then(() => {
  log('App ready');

  // Register auto-launch on Windows startup
  setupAutoLaunch();

  // Initialize HTTP client for hardware communication
  initHttp();

  // Start media server and auto-connect to TV if configured
  startMediaServer();
  if (tvIP) {
    setTimeout(() => connectToTV(), 2000);
  }

  // Register app:// protocol to serve static files (solves absolute path issue)
  const outDir = path.join(__dirname, '..', 'out');
  const mime = {
    '.html': 'text/html',
    '.js': 'application/javascript',
    '.css': 'text/css',
    '.json': 'application/json',
    '.png': 'image/png',
    '.jpg': 'image/jpeg',
    '.jpeg': 'image/jpeg',
    '.svg': 'image/svg+xml',
    '.ico': 'image/x-icon',
    '.woff': 'font/woff',
    '.woff2': 'font/woff2',
    '.ttf': 'font/ttf',
    '.webp': 'image/webp',
    '.gif': 'image/gif',
    '.map': 'application/json',
    '.txt': 'text/plain',
  };

  protocol.handle('app', (request) => {
    let url = request.url.replace('app://.', '');
    // Remove query string
    url = url.split('?')[0];
    // Decode
    url = decodeURIComponent(url);
    // Default to index.html
    if (url === '/' || url === '') url = '/index.html';
    // Remove leading slash
    if (url.startsWith('/')) url = url.slice(1);

    const filePath = path.join(outDir, url);

    // Security: prevent directory traversal
    if (!path.normalize(filePath).startsWith(outDir)) {
      log(`Security: blocked traversal: ${filePath}`);
      return new Response('Forbidden', { status: 403 });
    }

    // Try exact file, then with .html, then index.html in dir
    let resolvedPath = filePath;
    if (!fs.existsSync(resolvedPath)) {
      if (fs.existsSync(resolvedPath + '.html')) {
        resolvedPath = resolvedPath + '.html';
      } else if (fs.existsSync(path.join(resolvedPath, 'index.html'))) {
        resolvedPath = path.join(resolvedPath, 'index.html');
      } else {
        log(`App file not found: ${resolvedPath}`);
        return new Response('Not Found', { status: 404 });
      }
    }

    const ext = path.extname(resolvedPath).toLowerCase();
    const mimeType = mime[ext] || 'application/octet-stream';

    try {
      const data = fs.readFileSync(resolvedPath);
      return new Response(data, {
        headers: { 'Content-Type': mimeType }
      });
    } catch (e) {
      log(`Error reading file: ${resolvedPath} - ${e.message}`);
      return new Response('Error', { status: 500 });
    }
  });

  const outExists = fs.existsSync(outDir);
  const outIndexExists = fs.existsSync(path.join(outDir, 'index.html'));
  log(`[AppProtocol] Serving from: ${outDir}`);
  log(`[AppProtocol] out/ exists: ${outExists} | index.html exists: ${outIndexExists}`);
  if (outExists) {
    try {
      const outContents = fs.readdirSync(outDir).slice(0, 20);
      log(`[AppProtocol] out/ contents: ${outContents.join(', ')}`);
    } catch (e) {}
  }

  // Register custom protocol for media files with Range request support
  const mediaMime = {
    '.mp4': 'video/mp4', '.webm': 'video/webm', '.mov': 'video/quicktime',
    '.jpg': 'image/jpeg', '.jpeg': 'image/jpeg', '.png': 'image/png',
    '.svg': 'image/svg+xml', '.webp': 'image/webp', '.gif': 'image/gif',
    '.json': 'application/json',
  };

  protocol.handle('media', (request) => {
    // Strip protocol (media:// or media:///)
    const url = decodeURIComponent(request.url.replace(/^media:\/\/\/?/, ''));
    const mediaRoot = getMediaRoot();
    const resourcePath = path.normalize(path.join(mediaRoot, url));

    // Security: prevent directory traversal
    if (!resourcePath.startsWith(mediaRoot)) {
      log(`Security: blocked path traversal attempt: ${resourcePath}`);
      return new Response('Forbidden', { status: 403 });
    }

    if (!fs.existsSync(resourcePath)) {
      log(`Media not found: ${resourcePath}`);
      return new Response('Not Found', { status: 404 });
    }

    const stat = fs.statSync(resourcePath);
    const fileSize = stat.size;
    const ext = path.extname(resourcePath).toLowerCase();
    const contentType = mediaMime[ext] || 'application/octet-stream';
    const rangeHeader = request.headers.get('range');

    // Range request (video seeking)
    if (rangeHeader) {
      const parts = rangeHeader.replace(/bytes=/, '').split('-');
      const start = parseInt(parts[0], 10);
      // If the browser specifies an end, use it; otherwise serve the rest of the file.
      // Chromium knows how much it needs — artificial 5MB caps cause decode errors
      // because keyframes may land beyond the chunk boundary.
      const end = parts[1] ? parseInt(parts[1], 10) : fileSize - 1;
      const chunkSize = end - start + 1;

      const stream = fs.createReadStream(resourcePath, { start, end });
      const readable = new ReadableStream({
        start(controller) {
          stream.on('data', (chunk) => controller.enqueue(chunk));
          stream.on('end', () => controller.close());
          stream.on('error', (err) => controller.error(err));
        },
        cancel() { stream.destroy(); }
      });

      return new Response(readable, {
        status: 206,
        headers: {
          'Content-Type': contentType,
          'Content-Range': `bytes ${start}-${end}/${fileSize}`,
          'Content-Length': String(chunkSize),
          'Accept-Ranges': 'bytes',
        }
      });
    }

    // Full file request (images, small files)
    // For video files, return headers that signal range support
    if (ext === '.mp4' || ext === '.webm' || ext === '.mov') {
      // Return just headers first — browser will then make range requests
      const stream = fs.createReadStream(resourcePath);
      const readable = new ReadableStream({
        start(controller) {
          stream.on('data', (chunk) => controller.enqueue(chunk));
          stream.on('end', () => controller.close());
          stream.on('error', (err) => controller.error(err));
        },
        cancel() { stream.destroy(); }
      });

      return new Response(readable, {
        status: 200,
        headers: {
          'Content-Type': contentType,
          'Content-Length': String(fileSize),
          'Accept-Ranges': 'bytes',
        }
      });
    }

    // Non-video files: read entirely
    const data = fs.readFileSync(resourcePath);
    return new Response(data, {
      headers: { 'Content-Type': contentType }
    });
  });

  // IPC handlers — existing
  ipcMain.handle('get-media-root', () => getMediaRoot());

  ipcMain.handle('get-projects-data', () => {
    const mediaRoot = getMediaRoot();
    const jsonPath = path.join(mediaRoot, 'projects.json');
    if (fs.existsSync(jsonPath)) {
      try {
        return JSON.parse(fs.readFileSync(jsonPath, 'utf8'));
      } catch (e) {
        log(`Error reading projects.json: ${e.message}`);
        return null;
      }
    }
    return null;
  });

  // Media stats IPC (replaces API route)
  ipcMain.handle('get-media-stats', () => {
    const mediaRoot = getMediaRoot();
    const projectsPath = path.join(mediaRoot, 'projects');
    if (!fs.existsSync(projectsPath)) return [];

    try {
      const folders = fs.readdirSync(projectsPath).filter(f =>
        fs.statSync(path.join(projectsPath, f)).isDirectory()
      );

      return folders.map(projectId => {
        const projectPath = path.join(projectsPath, projectId);
        const stats = { projectId, videos: 0, photos: 0, hasLogo: false, hasMainImage: false };

        const imagesPath = path.join(projectPath, 'images');
        if (fs.existsSync(imagesPath)) {
          stats.hasMainImage = fs.existsSync(path.join(imagesPath, 'main.jpg'));
          const logoPath = path.join(imagesPath, 'logo');
          if (fs.existsSync(logoPath)) {
            stats.hasLogo = fs.readdirSync(logoPath).some(f => f.startsWith('logo.'));
          }
          const scenesPath = path.join(imagesPath, 'scenes');
          if (fs.existsSync(scenesPath)) {
            stats.photos = fs.readdirSync(scenesPath).filter(f => /\.(jpg|jpeg|png|webp)$/i.test(f)).length;
          }
        }

        const videosPath = path.join(projectPath, 'videos');
        if (fs.existsSync(videosPath)) {
          stats.videos = fs.readdirSync(videosPath).filter(f => /\.(mp4|webm|mov)$/i.test(f)).length;
        }

        return stats;
      });
    } catch (e) {
      log(`Error scanning media: ${e.message}`);
      return [];
    }
  });

  // Diagnostic IPC - shows where media is being searched
  ipcMain.handle('get-diagnostics', () => {
    const exeDir = path.dirname(app.getPath('exe'));
    const mediaRoot = getMediaRoot();

    const candidates = [
      { label: 'resources/media', path: path.join(process.resourcesPath, 'media') },
      { label: '[exe]/media', path: path.join(exeDir, 'media') },
      { label: '[exe]/../media', path: path.join(exeDir, '..', 'media') },
      { label: 'resources (direct)', path: process.resourcesPath },
      { label: '[exe] (direct)', path: exeDir },
    ];

    const results = candidates.map(c => {
      const projectsDir = path.join(c.path, 'projects');
      const exists = fs.existsSync(projectsDir);
      let sampleFiles = [];
      if (exists) {
        try {
          const dirs = fs.readdirSync(projectsDir).slice(0, 5);
          sampleFiles = dirs;
        } catch (e) {}
      }
      return { ...c, projectsExists: exists, sampleFiles };
    });

    return {
      platform: process.platform,
      isPackaged: app.isPackaged,
      appPath: app.getAppPath(),
      exePath: app.getPath('exe'),
      exeDir,
      resourcesPath: process.resourcesPath,
      mediaRoot,
      mediaRootExists: fs.existsSync(mediaRoot),
      candidates: results,
    };
  });

  // ===================== Hardware IPC Handlers =====================

  ipcMain.handle('hardware-block-up', async (_event, blockNumber) => {
    return sendDebouncedBlockCommand(blockNumber, 'UP');
  });

  ipcMain.handle('hardware-block-down', async (_event, blockNumber) => {
    return sendDebouncedBlockCommand(blockNumber, 'DOWN');
  });

  ipcMain.handle('hardware-all-stop', async () => {
    currentBlock = null;
    activeBlocks.clear();
    return sendHttpRequest('/api/stop');
  });

  ipcMain.handle('hardware-all-down', async () => {
    currentBlock = null;
    activeBlocks.clear();
    return sendHttpRequest('/api/all', { action: 'down' });
  });

  // ---- LED Auto-Cycle + Rainbow state ----
  let ledAutoCycleTimer = null;
  let ledRainbowTimer = null;
  let ledRainbowHue = 0;
  let ledAutoCycleIndex = 0;
  const LED_MODE_ORDER = ['WAVE', 'PULSE', 'SPARKLE', 'FIRE'];
  const modeToFwId = {
    'WAVE': 3, 'PULSE': 4, 'SPARKLE': 5, 'FIRE': 6,
    'CHASE': 3, 'STATIC': 3, 'RAINBOW': 3, 'METEOR': 3,
  };

  // HSL to RGB for rainbow
  function hslToRgb(h, s, l) {
    s /= 100; l /= 100;
    const k = n => (n + h / 30) % 12;
    const a = s * Math.min(l, 1 - l);
    const f = n => l - a * Math.max(-1, Math.min(k(n) - 3, Math.min(9 - k(n), 1)));
    return [Math.round(f(0) * 255), Math.round(f(8) * 255), Math.round(f(4) * 255)];
  }

  function startRainbowCycle() {
    if (ledRainbowTimer) return;
    log('[LED] Rainbow color cycling started');
    ledRainbowTimer = setInterval(() => {
      ledRainbowHue = (ledRainbowHue + 3) % 360;
      const [r, g, b] = hslToRgb(ledRainbowHue, 100, 50);
      sendHttpRequest('/api/color', { r, g, b }).catch(() => {});
    }, 150);
  }

  function stopRainbowCycle() {
    if (ledRainbowTimer) {
      clearInterval(ledRainbowTimer);
      ledRainbowTimer = null;
      log('[LED] Rainbow color cycling stopped');
    }
  }

  function stopLedAutoCycle() {
    if (ledAutoCycleTimer) {
      clearInterval(ledAutoCycleTimer);
      ledAutoCycleTimer = null;
      log('[LED] Auto-cycle stopped');
    }
    stopRainbowCycle();
  }

  ipcMain.handle('hardware-led-mode', async (_event, mode) => {
    const upperMode = mode.toUpperCase();

    // Handle AUTO mode — effects + rainbow color cycling
    if (upperMode === 'AUTO') {
      if (ledAutoCycleTimer) {
        stopLedAutoCycle();
        return { autoCycle: false };
      }
      log('[LED] Auto-cycle started (60s interval) + rainbow colors');
      ledAutoCycleIndex = 0;

      // Start rainbow color cycling (smooth hue rotation)
      await sendHttpRequest('/api/bri', { v: 200 });
      await sendHttpRequest('/api/effect', { id: 3 }); // Wave as base
      startRainbowCycle();

      // Switch effects every 60 seconds
      const cycleFn = async () => {
        const modeName = LED_MODE_ORDER[ledAutoCycleIndex % LED_MODE_ORDER.length];
        const fwId = modeToFwId[modeName];
        log(`[LED AutoCycle] → ${modeName} (FW id=${fwId}) + rainbow`);
        await sendHttpRequest('/api/effect', { id: fwId });
        ledAutoCycleIndex++;
      };
      await cycleFn();
      ledAutoCycleTimer = setInterval(cycleFn, 60 * 1000);
      return { autoCycle: true };
    }

    // Handle RAINBOW mode (manual)
    if (upperMode === 'RAINBOW') {
      stopLedAutoCycle();
      log('[LED] Rainbow mode — wave + color cycling');
      await sendHttpRequest('/api/bri', { v: 200 });
      await sendHttpRequest('/api/effect', { id: 3 });
      startRainbowCycle();
      return true;
    }

    // Any other mode stops auto-cycle and rainbow
    stopLedAutoCycle();

    // Handle OFF — just set brightness to 0
    if (upperMode === 'OFF') {
      log(`[LED] Mode OFF — brightness 0`);
      return await sendHttpRequest('/api/bri', { v: 0 });
    }

    const fwId = modeToFwId[upperMode];
    if (typeof fwId === 'number') {
      log(`[LED] Mode "${mode}" → FW effect ID ${fwId}`);
      await sendHttpRequest('/api/bri', { v: 200 });
      return await sendHttpRequest('/api/effect', { id: fwId });
    }
    return false;
  });

  // Direct effect ID (used by actuator-control panel) — sends firmware ID as-is
  ipcMain.handle('hardware-led-effect', async (_event, effectId, speed) => {
    const params = { id: effectId };
    if (speed !== undefined && speed !== null) params.speed = speed;
    log(`[LED] Effect FW:${effectId}${speed !== undefined ? ` speed=${speed}` : ''}`);
    stopLedAutoCycle(); // manual effect change stops auto-cycle
    await sendHttpRequest('/api/bri', { v: 200 }); // ensure brightness is on
    return sendHttpRequest('/api/effect', params);
  });

  ipcMain.handle('hardware-led-speed', async (_event, speed) => {
    log(`[LED] Speed → ${speed}`);
    return sendHttpRequest('/api/spd', { v: speed });
  });

  ipcMain.handle('hardware-led-color', async (_event, hexColor) => {
    const hex = hexColor.replace('#', '');
    const r = parseInt(hex.substring(0, 2), 16) || 0;
    const g = parseInt(hex.substring(2, 4), 16) || 0;
    const b = parseInt(hex.substring(4, 6), 16) || 0;
    log(`[LED] Color #${hex} → RGB(${r}, ${g}, ${b})`);
    return sendHttpRequest('/api/color', { r, g, b });
  });

  ipcMain.handle('hardware-led-brightness', async (_event, brightness) => {
    log(`[LED] Brightness → ${brightness}`);
    return sendHttpRequest('/api/bri', { v: brightness });
  });

  ipcMain.handle('hardware-get-status', () => {
    return {
      connected: espConnected,
      ip: espIP,
      lastPong,
      currentBlock,
    };
  });

  ipcMain.handle('hardware-set-ip', (_event, newIP) => {
    espIP = newIP;
    saveHardwareConfig();
    // Reset connection state and check immediately
    espConnected = false;
    lastPong = 0;
    sendHttpRequest('/api/status').catch(() => {});
    return { success: true, ip: espIP };
  });

  ipcMain.handle('hardware-ping', async () => {
    const sent = await sendHttpRequest('/api/status');
    return { sent, connected: espConnected, lastPong };
  });

  ipcMain.handle('hardware-send-command', async (_event, cmd) => {
    // Translate legacy UDP command strings to HTTP for backward compat
    if (cmd === 'PING') return sendHttpRequest('/api/status');
    if (cmd === 'ALL:STOP') return sendHttpRequest('/api/stop');
    if (cmd === 'ALL:DOWN') return sendHttpRequest('/api/all', { action: 'down' });
    if (cmd.startsWith('LED:MODE:')) {
      const modeToFwId = { 'STATIC': 2, 'PULSE': 1, 'RAINBOW': 0, 'CHASE': 3, 'SPARKLE': 5, 'WAVE': 3, 'FIRE': 6, 'METEOR': 7, 'OFF': 4 };
      const fwId = modeToFwId[cmd.split(':')[2].toUpperCase()];
      if (fwId !== undefined) return sendHttpRequest('/api/effect', { id: fwId });
      return false;
    }
    if (cmd.startsWith('LED:COLOR:')) {
      const hex = cmd.split(':')[2].replace('#', '');
      const r = parseInt(hex.substring(0, 2), 16) || 0;
      const g = parseInt(hex.substring(2, 4), 16) || 0;
      const b = parseInt(hex.substring(4, 6), 16) || 0;
      return sendHttpRequest('/api/color', { r, g, b });
    }
    if (cmd.startsWith('LED:BRIGHTNESS:')) {
      return sendHttpRequest('/api/bri', { v: cmd.split(':')[2] });
    }
    if (cmd.startsWith('BLOCK:')) {
      const parts = cmd.split(':');
      return sendHttpRequest('/api/block', { num: parts[1], action: parts[2].toLowerCase(), duration: BLOCK_DURATION_MS });
    }
    log(`[HTTP] Unknown command: ${cmd}`);
    return false;
  });

  ipcMain.handle('hardware-get-block-mapping', () => {
    return blockMapping;
  });

  ipcMain.handle('hardware-set-block-mapping', (_event, newMapping) => {
    blockMapping = newMapping;
    saveHardwareConfig();
    log(`[UDP] Block mapping updated: ${JSON.stringify(blockMapping)}`);
    return { success: true };
  });

  // ===================== TV IPC Handlers =====================

  ipcMain.handle('tv-connect', async () => {
    return connectToTV();
  });

  ipcMain.handle('tv-disconnect', async () => {
    if (tvWs) {
      try { tvWs.close(); } catch (e) {}
      tvWs = null;
      tvConnected = false;
    }
    return true;
  });

  ipcMain.handle('tv-play-video', async (_event, videoRelPath) => {
    return playVideoOnTV(videoRelPath);
  });

  ipcMain.handle('tv-stop-video', async () => {
    return stopVideoOnTV();
  });

  ipcMain.handle('tv-get-status', () => {
    return { connected: tvConnected, ip: tvIP, hasKey: !!tvClientKey };
  });

  ipcMain.handle('tv-set-ip', (_event, ip) => {
    tvIP = ip;
    saveHardwareConfig();
    // Reset connection
    tvConnected = false;
    tvClientKey = '';
    if (tvWs) {
      try { tvWs.close(); } catch (e) {}
      tvWs = null;
    }
    return { success: true, ip: tvIP };
  });

  // ===================== End TV IPC =====================

  // Log all registered protocols
  log('[Protocols] app:// and media:// registered');

  createWindow();
  log('[Startup] Application started successfully!');
  log(`[Startup] Window size: ${mainWindow.getBounds().width}x${mainWindow.getBounds().height}`);
  log(`[Startup] Fullscreen: ${mainWindow.isFullScreen()} | Kiosk: ${mainWindow.isKiosk()}`);
  log(`[Startup] Memory: ${JSON.stringify(process.memoryUsage())}`);
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});

app.on('activate', () => {
  if (mainWindow === null) createWindow();
});

app.on('will-quit', () => {
  globalShortcut.unregisterAll();
});

app.on('before-quit', async () => {
  log('[HTTP] Sending STOP ALL before quit...');
  // Send STOP ALL via HTTP before quitting
  try {
    await sendHttpRequest('/api/stop');
  } catch (e) {
    log(`[HTTP] Failed to send STOP on quit: ${e.message}`);
  }
  cleanupHttp();
  cleanupTV();
  log('=== RAMS Interactive Hub Stopped ===');
  try { logStream.end(); } catch (e) {}
});
