/**
 * ESP32 RAMS Controller Client
 * API для управления 15 актуаторными блоками + LED зонами
 *
 * @version 1.0
 * @date 2026-02-15
 */

export type BlockAction = "UP" | "DOWN" | "STOP";

export interface BlockStatus {
  id: number;
  isActive: boolean;
}

export interface ESP32Status {
  active: number;
  blocks: number[];
  mega1Alive?: boolean;
  mega2Alive?: boolean;
}

export interface LEDConfig {
  r: number;
  g: number;
  b: number;
  brightness: number;
  speed: number;
  effect: number; // 0-7: Static, Pulse, Rainbow, Chase, Sparkle, Wave, Fire, Meteor
}

export interface ESP32Config {
  host: string; // Обычно "192.168.4.1" для WiFi AP или IP адрес ESP32
  port?: number; // По умолчанию 80
  timeout?: number; // Таймаут запросов в мс
}

export class ESP32Client {
  private baseUrl: string;
  private timeout: number;
  private retryAttempts: number = 3;
  private retryDelay: number = 1000;

  constructor(config: ESP32Config) {
    const { host, port = 80, timeout = 5000 } = config;
    this.baseUrl = `http://${host}:${port}`;
    this.timeout = timeout;
  }

  /**
   * Проверить доступность ESP32
   */
  async ping(): Promise<boolean> {
    try {
      console.log(`[ESP32Client] 🔍 Ping ${this.baseUrl}/api/status`);
      const response = await this.fetchWithTimeout("/api/status", { method: "GET" });
      console.log(`[ESP32Client] Ping response: ${response.ok ? '✅ OK' : '❌ FAILED'} (status: ${response.status})`);
      return response.ok;
    } catch (err) {
      console.error(`[ESP32Client] Ping error:`, err);
      return false;
    }
  }

  /**
   * Получить статус всех блоков
   */
  async getStatus(): Promise<ESP32Status> {
    console.log(`[ESP32Client] 📊 GET ${this.baseUrl}/api/status`);
    const response = await this.fetchWithRetry("/api/status", { method: "GET" });
    if (!response.ok) {
      console.error(`[ESP32Client] ❌ Get status failed: ${response.status} ${response.statusText}`);
      throw new Error(`Failed to get status: ${response.statusText}`);
    }
    const data = await response.json();
    console.log(`[ESP32Client] ✅ Status: active=${data.active}, blocks=[${data.blocks}]`);
    return data;
  }

  /**
   * Управление блоком
   * @param blockNum Номер блока (1-15)
   * @param action Действие (UP/DOWN/STOP)
   * @param duration Длительность в мс (по умолчанию 10000)
   */
  async controlBlock(blockNum: number, action: BlockAction, duration: number = 10000): Promise<void> {
    if (blockNum < 1 || blockNum > 15) {
      console.error(`[ESP32Client] ❌ Invalid block number: ${blockNum}`);
      throw new Error(`Invalid block number: ${blockNum}. Must be 1-15`);
    }

    const url = `/api/block?num=${blockNum}&action=${action}&duration=${duration}`;
    console.log(`[ESP32Client] 🎯 POST ${this.baseUrl}${url}`);
    console.log(`[ESP32Client]    Block: ${blockNum}, Action: ${action}, Duration: ${duration}ms`);

    const startTime = Date.now();
    const response = await this.fetchWithRetry(url, { method: "POST" });
    const elapsed = Date.now() - startTime;

    if (!response.ok) {
      const error = await response.text();
      console.error(`[ESP32Client] ❌ Block ${blockNum} ${action} FAILED after ${elapsed}ms: ${error}`);
      throw new Error(`Failed to control block ${blockNum}: ${error}`);
    }

    const responseText = await response.text();
    console.log(`[ESP32Client] ✅ Block ${blockNum} ${action} SUCCESS after ${elapsed}ms`);
    console.log(`[ESP32Client]    Response: ${responseText}`);
  }

  /**
   * Остановить все блоки
   */
  async stopAll(): Promise<void> {
    const response = await this.fetchWithRetry("/api/stop", { method: "POST" });
    if (!response.ok) {
      throw new Error(`Failed to stop all blocks: ${response.statusText}`);
    }
  }

  /**
   * LED УПРАВЛЕНИЕ
   * ESP32 v3.2 API: /api/effect, /api/color, /api/bri, /api/spd
   *
   * Рабочие эффекты на ESP32 v3.2: 3=Wave, 4=Pulse, 5=Sparkle, 6=Fire
   * IDs 0-2 принимаются но визуально не меняются
   * UI отправляет firmware ID напрямую (без маппинга)
   */

  /**
   * Установить цвет LED
   */
  async setLEDColor(r: number, g: number, b: number): Promise<void> {
    const url = `/api/color?r=${r}&g=${g}&b=${b}`;
    console.log(`[ESP32Client] POST ${this.baseUrl}${url} (RGB: ${r},${g},${b})`);
    const response = await this.fetchWithRetry(url, { method: "POST" });
    if (!response.ok) {
      console.error(`[ESP32Client] ❌ LED color failed`);
      throw new Error(`Failed to set LED color: ${response.statusText}`);
    }
    console.log(`[ESP32Client] ✅ LED color set to RGB(${r}, ${g}, ${b})`);
  }

  /**
   * Установить яркость LED
   */
  async setLEDBrightness(brightness: number): Promise<void> {
    const url = `/api/bri?v=${brightness}`;
    console.log(`[ESP32Client] POST ${this.baseUrl}${url}`);
    const response = await this.fetchWithRetry(url, { method: "POST" });
    if (!response.ok) {
      console.error(`[ESP32Client] ❌ LED brightness failed`);
      throw new Error(`Failed to set LED brightness: ${response.statusText}`);
    }
    console.log(`[ESP32Client] ✅ LED brightness set to ${brightness}`);
  }

  /**
   * Установить скорость анимации
   */
  async setLEDSpeed(speed: number): Promise<void> {
    const url = `/api/spd?v=${speed}`;
    console.log(`[ESP32Client] POST ${this.baseUrl}${url}`);
    const response = await this.fetchWithRetry(url, { method: "POST" });
    if (!response.ok) {
      console.error(`[ESP32Client] ❌ LED speed failed`);
      throw new Error(`Failed to set LED speed: ${response.statusText}`);
    }
    console.log(`[ESP32Client] ✅ LED speed set to ${speed}`);
  }

  /**
   * Установить эффект LED
   * @param effectId 0-7: Static, Pulse, Rainbow, Chase, Sparkle, Wave, Fire, Meteor
   * @param speed Скорость эффекта (0-255, опционально)
   */
  async setLEDEffect(effectId: number, speed?: number): Promise<void> {
    // Send firmware ID directly (no mapping needed for v3.2)
    const speedParam = speed !== undefined ? `&speed=${speed}` : "";
    const url = `/api/effect?id=${effectId}${speedParam}`;
    console.log(`[ESP32Client] 🎨 POST ${this.baseUrl}${url} (FW:${effectId})`);

    const startTime = Date.now();
    const response = await this.fetchWithRetry(url, { method: "POST" });
    const elapsed = Date.now() - startTime;

    if (!response.ok) {
      console.error(`[ESP32Client] ❌ LED effect FAILED after ${elapsed}ms`);
      throw new Error(`Failed to set LED effect: ${response.statusText}`);
    }
    console.log(`[ESP32Client] ✅ LED effect set to FW:${effectId} after ${elapsed}ms`);
  }

  /**
   * Выключить LED (brightness=0)
   */
  async setLEDOff(): Promise<void> {
    await this.setLEDBrightness(0);
    console.log(`[ESP32Client] ✅ LED OFF`);
  }

  /**
   * Управление конкретным блоком с retry логикой
   */
  async blockUp(blockNum: number, duration?: number): Promise<void> {
    return this.controlBlock(blockNum, "UP", duration);
  }

  async blockDown(blockNum: number, duration?: number): Promise<void> {
    return this.controlBlock(blockNum, "DOWN", duration);
  }

  async blockStop(blockNum: number): Promise<void> {
    return this.controlBlock(blockNum, "STOP");
  }

  /**
   * Получить статус конкретного блока
   */
  async getBlockStatus(blockNum: number): Promise<boolean> {
    const status = await this.getStatus();
    return status.blocks.includes(blockNum);
  }

  /**
   * Fetch с таймаутом
   */
  private async fetchWithTimeout(url: string, options: RequestInit): Promise<Response> {
    const controller = new AbortController();
    const timeoutId = setTimeout(() => controller.abort(), this.timeout);

    try {
      // Use Next.js proxy in browser: /esp32-api/* → ESP32 /api/*
      const isBrowser = typeof window !== 'undefined';
      const finalUrl = isBrowser
        ? `${this.baseUrl}/esp32-api${url.replace('/api', '')}/`  // /api/status → /esp32-api/status/ (trailing slash for Next.js)
        : `${this.baseUrl}${url}`;  // Direct connection

      const response = await fetch(finalUrl, {
        ...options,
        signal: controller.signal,
      });
      return response;
    } finally {
      clearTimeout(timeoutId);
    }
  }

  /**
   * Fetch с повторными попытками
   */
  private async fetchWithRetry(url: string, options: RequestInit): Promise<Response> {
    let lastError: Error | null = null;

    for (let attempt = 0; attempt < this.retryAttempts; attempt++) {
      try {
        if (attempt > 0) {
          console.log(`[ESP32Client] 🔄 Retry attempt ${attempt + 1}/${this.retryAttempts} for ${url}`);
        }
        const response = await this.fetchWithTimeout(url, options);

        // 4xx — client error, не ретраим
        if (!response.ok && response.status < 500) {
          return response;
        }

        // 5xx — server error, ретраим
        if (!response.ok && response.status >= 500) {
          console.error(`[ESP32Client] ❌ Attempt ${attempt + 1} got HTTP ${response.status} for ${url}`);
          if (attempt === this.retryAttempts - 1) {
            return response; // последняя попытка — возвращаем как есть
          }
          const delay = this.retryDelay * (attempt + 1);
          console.log(`[ESP32Client] ⏳ Waiting ${delay}ms before retry...`);
          await this.sleep(delay);
          continue;
        }

        return response;
      } catch (error) {
        lastError = error instanceof Error ? error : new Error(String(error));

        // Abort — не ретраим (компонент размонтировался)
        if (lastError.name === 'AbortError' || lastError.message.includes('aborted')) {
          throw lastError;
        }

        console.error(`[ESP32Client] ❌ Attempt ${attempt + 1} failed:`, lastError.message);

        // Если это последняя попытка, выбрасываем ошибку
        if (attempt === this.retryAttempts - 1) {
          console.error(`[ESP32Client] ❌ All ${this.retryAttempts} attempts failed for ${url}`);
          break;
        }

        // Ждем перед следующей попыткой
        const delay = this.retryDelay * (attempt + 1);
        console.log(`[ESP32Client] ⏳ Waiting ${delay}ms before retry...`);
        await this.sleep(delay);
      }
    }

    throw lastError || new Error("Unknown error during fetch");
  }

  /**
   * Утилита для задержки
   */
  private sleep(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  /**
   * Обновить конфигурацию
   */
  updateConfig(config: Partial<ESP32Config>): void {
    if (config.host || config.port) {
      const host = config.host || this.baseUrl.split("://")[1].split(":")[0];
      const port = config.port || parseInt(this.baseUrl.split(":")[2] || "80");
      this.baseUrl = `http://${host}:${port}`;
    }
    if (config.timeout) {
      this.timeout = config.timeout;
    }
  }
}

/**
 * Сканировать подсеть и найти ESP32 по ответу /api/status
 * Работает из браузера (веб-режим без Electron)
 */
export async function discoverESP32(subnet: string = "192.168.110", port: number = 80): Promise<string | null> {
  console.log(`[ESP32Discovery] Scanning ${subnet}.1-254...`);

  // Scan in batches of 20 to avoid too many concurrent requests
  for (let batch = 0; batch < 255; batch += 20) {
    const promises: Promise<string | null>[] = [];

    for (let i = batch + 1; i <= Math.min(batch + 20, 254); i++) {
      const ip = `${subnet}.${i}`;
      promises.push(
        fetch(`http://${ip}:${port}/api/status`, { signal: AbortSignal.timeout(1500) })
          .then(async (res) => {
            if (!res.ok) return null;
            const data = await res.json();
            if (data.hasOwnProperty("active") && data.hasOwnProperty("blocks")) {
              console.log(`[ESP32Discovery] ✅ Found ESP32 at ${ip}`);
              return ip;
            }
            return null;
          })
          .catch(() => null)
      );
    }

    const results = await Promise.all(promises);
    const found = results.find((r) => r !== null);
    if (found) return found;
  }

  console.log("[ESP32Discovery] ESP32 not found");
  return null;
}

/**
 * Создать клиент для локальной WiFi AP ESP32
 * Использует переменные окружения из .env.local если доступны
 */
export function createLocalESP32Client(): ESP32Client {
  // Use Next.js proxy to avoid CORS issues in browser
  // Browser: /esp32-api/status → Next.js proxy → http://192.168.4.1/api/status
  const isBrowser = typeof window !== 'undefined';

  if (isBrowser) {
    console.log(`[ESP32Client] Creating browser client (using Next.js proxy at /esp32-api)`);
    // Use relative URL to leverage Next.js rewrite proxy
    return new ESP32Client({
      host: window.location.hostname, // localhost or server IP
      port: parseInt(window.location.port) || 3000,
      timeout: 5000,
    });
  }

  // Server-side or Electron: direct connection to ESP32
  const host = typeof process !== 'undefined' && process.env?.NEXT_PUBLIC_ESP32_HOST
    ? process.env.NEXT_PUBLIC_ESP32_HOST
    : "192.168.4.1";

  const port = typeof process !== 'undefined' && process.env?.NEXT_PUBLIC_ESP32_PORT
    ? parseInt(process.env.NEXT_PUBLIC_ESP32_PORT)
    : 80;

  console.log(`[ESP32Client] Creating server client for ${host}:${port}`);

  return new ESP32Client({
    host,
    port,
    timeout: 5000,
  });
}

/**
 * Создать клиент с автонахождением ESP32 в сети
 * Сначала пробует сохранённый/env IP, если не отвечает — сканирует подсеть
 */
export async function createAutoDiscoveryClient(): Promise<ESP32Client> {
  const envHost = typeof process !== 'undefined' && process.env?.NEXT_PUBLIC_ESP32_HOST
    ? process.env.NEXT_PUBLIC_ESP32_HOST
    : "192.168.4.1";

  const port = typeof process !== 'undefined' && process.env?.NEXT_PUBLIC_ESP32_PORT
    ? parseInt(process.env.NEXT_PUBLIC_ESP32_PORT)
    : 80;

  // Try saved IP first
  const client = new ESP32Client({ host: envHost, port, timeout: 3000 });
  const reachable = await client.ping();

  if (reachable) {
    console.log(`[ESP32AutoDiscovery] ESP32 reachable at ${envHost}`);
    return client;
  }

  // Auto-discover
  console.log(`[ESP32AutoDiscovery] ${envHost} unreachable, scanning network...`);
  const discovered = await discoverESP32(undefined, port);

  if (discovered) {
    return new ESP32Client({ host: discovered, port, timeout: 5000 });
  }

  // Fallback to env IP
  console.warn("[ESP32AutoDiscovery] Discovery failed, using fallback IP");
  return client;
}

/**
 * Создать клиент для ESP32 в локальной сети
 */
export function createNetworkESP32Client(ipAddress: string): ESP32Client {
  return new ESP32Client({
    host: ipAddress,
    port: 80,
    timeout: 5000,
  });
}

/**
 * Hook для использования в React компонентах
 */
export function useESP32Client(config?: ESP32Config) {
  const [client] = React.useState(() =>
    config ? new ESP32Client(config) : createLocalESP32Client()
  );
  return client;
}

// Re-export для удобства
import * as React from "react";
export { React };
