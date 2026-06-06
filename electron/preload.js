const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electron', {
  isElectron: true,
  getMediaRoot: () => ipcRenderer.invoke('get-media-root'),
  getProjectsData: () => ipcRenderer.invoke('get-projects-data'),
  getMediaStats: () => ipcRenderer.invoke('get-media-stats'),
  getDiagnostics: () => ipcRenderer.invoke('get-diagnostics'),
  onOpenAdmin: (callback) => ipcRenderer.on('open-admin', callback),

  // Hardware UDP control via IPC
  blockUp: (blockNumber) => ipcRenderer.invoke('hardware-block-up', blockNumber),
  blockDown: (blockNumber) => ipcRenderer.invoke('hardware-block-down', blockNumber),
  allStop: () => ipcRenderer.invoke('hardware-all-stop'),
  allDown: () => ipcRenderer.invoke('hardware-all-down'),
  setLedMode: (mode) => ipcRenderer.invoke('hardware-led-mode', mode),
  setLedEffect: (effectId, speed) => ipcRenderer.invoke('hardware-led-effect', effectId, speed),
  setLedColor: (hexColor) => ipcRenderer.invoke('hardware-led-color', hexColor),
  setLedBrightness: (brightness) => ipcRenderer.invoke('hardware-led-brightness', brightness),
  setLedSpeed: (speed) => ipcRenderer.invoke('hardware-led-speed', speed),
  getHardwareStatus: () => ipcRenderer.invoke('hardware-get-status'),
  setEspIP: (ip) => ipcRenderer.invoke('hardware-set-ip', ip),
  pingHardware: () => ipcRenderer.invoke('hardware-ping'),
  sendHardwareCommand: (cmd) => ipcRenderer.invoke('hardware-send-command', cmd),
  getBlockMapping: () => ipcRenderer.invoke('hardware-get-block-mapping'),
  setBlockMapping: (mapping) => ipcRenderer.invoke('hardware-set-block-mapping', mapping),

  // LG TV control via SSAP
  tvConnect: () => ipcRenderer.invoke('tv-connect'),
  tvDisconnect: () => ipcRenderer.invoke('tv-disconnect'),
  tvPlayVideo: (videoPath) => ipcRenderer.invoke('tv-play-video', videoPath),
  tvStopVideo: () => ipcRenderer.invoke('tv-stop-video'),
  tvGetStatus: () => ipcRenderer.invoke('tv-get-status'),
  tvSetIP: (ip) => ipcRenderer.invoke('tv-set-ip', ip),
  onTvStatusChanged: (callback) => ipcRenderer.on('tv-status-changed', (_event, data) => callback(data)),
});
