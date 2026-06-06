/**
 * Hardware IPC Service
 * Communicates with ESP32 via UDP through Electron main process IPC
 * Falls back to no-op when not running in Electron
 */

export interface TVStatus {
  connected: boolean;
  ip: string;
  hasKey: boolean;
}

interface ElectronAPI {
  isElectron: boolean;
  blockUp: (blockNumber: number) => Promise<boolean>;
  blockDown: (blockNumber: number) => Promise<boolean>;
  allStop: () => Promise<boolean>;
  allDown: () => Promise<boolean>;
  setLedMode: (mode: string) => Promise<boolean>;
  setLedColor: (hexColor: string) => Promise<boolean>;
  setLedBrightness: (brightness: number) => Promise<boolean>;
  getHardwareStatus: () => Promise<HardwareStatus>;
  setEspIP: (ip: string) => Promise<{ success: boolean; ip: string }>;
  pingHardware: () => Promise<{ sent: boolean; connected: boolean; lastPong: number }>;
  sendHardwareCommand: (cmd: string) => Promise<boolean>;
  getBlockMapping: () => Promise<Record<string, number>>;
  setBlockMapping: (mapping: Record<string, number>) => Promise<{ success: boolean }>;
  // TV
  tvConnect: () => Promise<boolean>;
  tvDisconnect: () => Promise<boolean>;
  tvPlayVideo: (videoPath: string) => Promise<boolean>;
  tvStopVideo: () => Promise<boolean>;
  tvGetStatus: () => Promise<TVStatus>;
  tvSetIP: (ip: string) => Promise<{ success: boolean; ip: string }>;
  onTvStatusChanged: (callback: (data: { connected: boolean; ip: string }) => void) => void;
}

export type BlockMapping = Record<string, number>; // galleryCardId → physical block number

export interface HardwareStatus {
  connected: boolean;
  ip: string;
  lastPong: number;
  currentBlock: number | null;
}

function getElectronAPI(): ElectronAPI | null {
  if (typeof window !== "undefined" && (window as any).electron?.isElectron) {
    return (window as any).electron as ElectronAPI;
  }
  return null;
}

class HardwareIPCService {
  private static instance: HardwareIPCService;

  private constructor() {}

  public static getInstance(): HardwareIPCService {
    if (!HardwareIPCService.instance) {
      HardwareIPCService.instance = new HardwareIPCService();
    }
    return HardwareIPCService.instance;
  }

  async blockUp(blockNumber: number): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.blockUp(blockNumber);
    } catch (e) {
      console.warn("[Hardware] blockUp failed:", e);
      return false;
    }
  }

  async blockDown(blockNumber: number): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.blockDown(blockNumber);
    } catch (e) {
      console.warn("[Hardware] blockDown failed:", e);
      return false;
    }
  }

  async allStop(): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.allStop();
    } catch (e) {
      console.warn("[Hardware] allStop failed:", e);
      return false;
    }
  }

  async allDown(): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.allDown();
    } catch (e) {
      console.warn("[Hardware] allDown failed:", e);
      return false;
    }
  }

  async setLedMode(mode: string): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.setLedMode(mode);
    } catch (e) {
      console.warn("[Hardware] setLedMode failed:", e);
      return false;
    }
  }

  async setLedColor(hexColor: string): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.setLedColor(hexColor);
    } catch (e) {
      console.warn("[Hardware] setLedColor failed:", e);
      return false;
    }
  }

  async setLedBrightness(brightness: number): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.setLedBrightness(brightness);
    } catch (e) {
      console.warn("[Hardware] setLedBrightness failed:", e);
      return false;
    }
  }

  async getStatus(): Promise<HardwareStatus> {
    const api = getElectronAPI();
    if (!api) return { connected: false, ip: "", lastPong: 0, currentBlock: null };
    try {
      return await api.getHardwareStatus();
    } catch (e) {
      console.warn("[Hardware] getStatus failed:", e);
      return { connected: false, ip: "", lastPong: 0, currentBlock: null };
    }
  }

  async setIP(ip: string): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      const result = await api.setEspIP(ip);
      return result.success;
    } catch (e) {
      console.warn("[Hardware] setIP failed:", e);
      return false;
    }
  }

  async ping(): Promise<{ sent: boolean; connected: boolean }> {
    const api = getElectronAPI();
    if (!api) return { sent: false, connected: false };
    try {
      const result = await api.pingHardware();
      return { sent: result.sent, connected: result.connected };
    } catch (e) {
      console.warn("[Hardware] ping failed:", e);
      return { sent: false, connected: false };
    }
  }

  async sendCommand(cmd: string): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.sendHardwareCommand(cmd);
    } catch (e) {
      console.warn("[Hardware] sendCommand failed:", e);
      return false;
    }
  }

  async getBlockMapping(): Promise<BlockMapping> {
    const api = getElectronAPI();
    if (!api) return {};
    try {
      return await api.getBlockMapping();
    } catch (e) {
      console.warn("[Hardware] getBlockMapping failed:", e);
      return {};
    }
  }

  async setBlockMapping(mapping: BlockMapping): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      const result = await api.setBlockMapping(mapping);
      return result.success;
    } catch (e) {
      console.warn("[Hardware] setBlockMapping failed:", e);
      return false;
    }
  }

  // ===================== TV Methods =====================

  async tvConnect(): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.tvConnect();
    } catch (e) {
      console.warn("[TV] connect failed:", e);
      return false;
    }
  }

  async tvDisconnect(): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.tvDisconnect();
    } catch (e) {
      console.warn("[TV] disconnect failed:", e);
      return false;
    }
  }

  async tvPlayVideo(videoPath: string): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.tvPlayVideo(videoPath);
    } catch (e) {
      console.warn("[TV] playVideo failed:", e);
      return false;
    }
  }

  async tvStopVideo(): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      return await api.tvStopVideo();
    } catch (e) {
      console.warn("[TV] stopVideo failed:", e);
      return false;
    }
  }

  async tvGetStatus(): Promise<TVStatus> {
    const api = getElectronAPI();
    if (!api) return { connected: false, ip: "", hasKey: false };
    try {
      return await api.tvGetStatus();
    } catch (e) {
      console.warn("[TV] getStatus failed:", e);
      return { connected: false, ip: "", hasKey: false };
    }
  }

  async tvSetIP(ip: string): Promise<boolean> {
    const api = getElectronAPI();
    if (!api) return false;
    try {
      const result = await api.tvSetIP(ip);
      return result.success;
    } catch (e) {
      console.warn("[TV] setIP failed:", e);
      return false;
    }
  }

  onTvStatusChanged(callback: (data: { connected: boolean; ip: string }) => void): void {
    const api = getElectronAPI();
    if (!api) return;
    try {
      api.onTvStatusChanged(callback);
    } catch (e) {
      console.warn("[TV] onStatusChanged failed:", e);
    }
  }
}

export default HardwareIPCService;
