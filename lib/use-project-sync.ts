/**
 * Project Sync Hook
 * Синхронизация проектов с актуаторами и LED
 */

import * as React from "react";
import { Project } from "@/lib/types";
import { ESP32Client, createLocalESP32Client } from "@/lib/esp32-client";
import { getBlockNumberForProject } from "@/lib/data/gallery-config";

// Генерация цветов для проектов (градиент по кругу)
const generateProjectColor = (index: number, total: number): { r: number; g: number; b: number } => {
  const hue = (index / total) * 360; // Распределение по цветовому кругу

  // HSL to RGB conversion
  const h = hue / 60;
  const c = 255;
  const x = c * (1 - Math.abs((h % 2) - 1));

  let r = 0, g = 0, b = 0;

  if (h >= 0 && h < 1) { r = c; g = x; b = 0; }
  else if (h >= 1 && h < 2) { r = x; g = c; b = 0; }
  else if (h >= 2 && h < 3) { r = 0; g = c; b = x; }
  else if (h >= 3 && h < 4) { r = 0; g = x; b = c; }
  else if (h >= 4 && h < 5) { r = x; g = 0; b = c; }
  else if (h >= 5 && h < 6) { r = c; g = 0; b = x; }

  return { r: Math.round(r), g: Math.round(g), b: Math.round(b) };
};

/**
 * Получить блоки для проекта по его ID
 * @param projectId ID проекта (например, "09-rams-garden-almaty")
 * @returns Массив номеров блоков
 */
const getBlocksForProject = (projectId: string): number[] => {
  const blockNum = getBlockNumberForProject(projectId);

  if (blockNum === undefined) {
    console.warn(`[getBlocksForProject] Project ${projectId} not found in gallery config`);
    return [];
  }

  return [blockNum];
};

export interface ProjectSyncOptions {
  projects?: Project[];        // Список всех проектов (для определения индекса)
  enableActuators?: boolean;  // Включить управление актуаторами
  enableLED?: boolean;         // Включить управление LED
  animationDuration?: number;  // Длительность подъема актуаторов (мс)
  fadeInDuration?: number;     // Длительность плавного включения LED (мс)
  autoConnect?: boolean;       // Автоматическое подключение к ESP32
}

const DEFAULT_OPTIONS: ProjectSyncOptions = {
  enableActuators: true,
  enableLED: true,
  animationDuration: 4000,  // 4 сек — физический подъём актуатора
  fadeInDuration: 4000,       // 4 секунды плавное свечение
  autoConnect: true,
};

export function useProjectSync(options: ProjectSyncOptions = {}) {
  const optsRef = React.useRef({ ...DEFAULT_OPTIONS, ...options });
  optsRef.current = { ...DEFAULT_OPTIONS, ...options };

  const [client] = React.useState(() => createLocalESP32Client());

  // State для UI рендеринга
  const [isConnected, setIsConnected] = React.useState(false);
  const [activeProject, setActiveProject] = React.useState<Project | null>(null);
  const [isAnimating, setIsAnimating] = React.useState(false);
  const [hwStatus, setHwStatus] = React.useState<'idle' | 'activating' | 'activated' | 'deactivating' | 'deactivated' | 'error'>('idle');

  // Refs для async-логики (избегаем stale closures)
  const isConnectedRef = React.useRef(false);
  const isAnimatingRef = React.useRef(false);
  const activeProjectRef = React.useRef<Project | null>(null);
  const ledCancelRef = React.useRef<{ cancelled: boolean }>({ cancelled: false });

  // Sync-сеттеры: обновляют и ref (синхронно), и state (для UI)
  const setIsConnectedSync = React.useCallback((value: boolean) => {
    isConnectedRef.current = value;
    setIsConnected(value);
  }, []);

  const setIsAnimatingSync = React.useCallback((value: boolean) => {
    isAnimatingRef.current = value;
    setIsAnimating(value);
  }, []);

  const setActiveProjectSync = React.useCallback((value: Project | null) => {
    activeProjectRef.current = value;
    setActiveProject(value);
  }, []);

  // Проверка подключения
  React.useEffect(() => {
    if (!optsRef.current.autoConnect) {
      console.log("[ProjectSync] Auto-connect disabled");
      return;
    }

    console.log("[ProjectSync] Starting auto-connect to ESP32...");

    const checkConnection = async () => {
      try {
        console.log("[ProjectSync] Pinging ESP32 at 192.168.4.1...");
        const connected = await client.ping();
        if (connected !== isConnectedRef.current) {
          console.log(`[ProjectSync] Connection status changed: ${connected ? '✅ ONLINE' : '❌ OFFLINE'}`);
        }
        setIsConnectedSync(connected);
      } catch (err) {
        console.error("[ProjectSync] Ping failed:", err);
        setIsConnectedSync(false);
      }
    };

    checkConnection();
    const interval = setInterval(checkConnection, 5000);
    return () => {
      console.log("[ProjectSync] Stopping auto-connect");
      clearInterval(interval);
    };
  }, [client, setIsConnectedSync]);

  /**
   * Плавное включение LED
   * 10 шагов с квадратичной кривой — LED заметно загорается с первого шага
   * Учитывает HTTP латенси в расчёте задержки
   */
  const fadeInLED = React.useCallback(async (
    r: number,
    g: number,
    b: number,
    duration: number,
    cancelToken: { cancelled: boolean }
  ) => {
    const steps = 20; // More steps = smoother fade
    const stepDelay = duration / steps;

    for (let i = 1; i <= steps; i++) {
      if (cancelToken.cancelled) {
        console.log("[ProjectSync] LED fade-in cancelled");
        return;
      }

      // Smooth cubic ease-out: быстрый старт, плавное замедление к концу
      const t = i / steps; // 0.05 → 1.0
      const brightness = Math.floor(t * t * t * 200); // cubic easing
      try {
        const start = Date.now();
        await client.setLEDBrightness(brightness);
        const elapsed = Date.now() - start;
        const remaining = Math.max(0, stepDelay - elapsed);
        if (remaining > 0) {
          await new Promise(resolve => setTimeout(resolve, remaining));
        }
      } catch (err) {
        console.error("[ProjectSync] Failed to fade in LED:", err);
        break;
      }
    }

    if (cancelToken.cancelled) return;

    console.log("[ProjectSync] ✅ LED fade-in completed. Setting final brightness to 200...");
    await client.setLEDBrightness(200);
    console.log("[ProjectSync] ✅ LED brightness locked at 200 until project exit");
  }, [client]);

  /**
   * Активировать проект (поднять актуаторы + включить LED)
   */
  const activateProject = React.useCallback(async (project: Project) => {
    const opts = optsRef.current;

    console.log(`[ProjectSync] ============ ACTIVATION START ============`);
    console.log(`[ProjectSync] Project: ${project.name} (ID: ${project.id})`);
    console.log(`[ProjectSync] ESP32 Connected: ${isConnectedRef.current}`);
    console.log(`[ProjectSync] Is Animating: ${isAnimatingRef.current}`);
    console.log(`[ProjectSync] Enable Actuators: ${opts.enableActuators}`);
    console.log(`[ProjectSync] Enable LED: ${opts.enableLED}`);

    if (!isConnectedRef.current) {
      console.warn("[ProjectSync] ❌ ESP32 not connected. Skipping activation.");
      return;
    }

    // Атомарный guard: ref блокирует двойной клик на уровне одного tick
    if (isAnimatingRef.current) {
      console.warn("[ProjectSync] ❌ Animation in progress. Skipping.");
      return;
    }
    isAnimatingRef.current = true;   // синхронно — блокирует следующий вызов мгновенно
    setIsAnimating(true);            // для UI
    setHwStatus('activating');

    setActiveProjectSync(project);

    // Отменяем предыдущий LED fade если был
    ledCancelRef.current.cancelled = true;
    const cancelToken = { cancelled: false };
    ledCancelRef.current = cancelToken;

    // Найти индекс проекта в списке (для генерации цвета)
    const projectIndex = opts.projects?.findIndex(p => p.id === project.id) ?? -1;
    console.log(`[ProjectSync] Project Index: ${projectIndex} (out of ${opts.projects?.length || 0} projects)`);

    // Получить блок для проекта по ID (из gallery-config.ts)
    const blocks = getBlocksForProject(project.id);
    const color = generateProjectColor(projectIndex >= 0 ? projectIndex : 0, opts.projects?.length || 28);

    if (blocks.length === 0) {
      console.warn(`[ProjectSync] ⚠️ Project ${project.id} has no associated blocks. Skipping actuator activation.`);
      setIsAnimatingSync(false);
      return;
    }

    console.log(`[ProjectSync] Blocks to activate: [${blocks.join(', ')}]`);
    console.log(`[ProjectSync] LED Color: RGB(${color.r}, ${color.g}, ${color.b})`);

    try {
      // Поднять блок — прямой HTTP к ESP32
      // duration=30000 чтобы STATE_UP держался 30 сек (LED зона горит белым)
      // Физически блок остановится по концевику за ~4 сек, но LED будет гореть
      if (opts.enableActuators && blocks.length > 0) {
        for (let i = 0; i < blocks.length; i++) {
          console.log(`[ProjectSync] 📈 Raising block ${blocks[i]} (${i + 1}/${blocks.length})...`);
          await client.blockUp(blocks[i], opts.animationDuration);
          await new Promise(resolve => setTimeout(resolve, 200));
        }
        console.log(`[ProjectSync] ✅ All blocks raised`);
      }

      console.log(`[ProjectSync] ✅ Activated project: ${project.name}`);
      console.log(`[ProjectSync] ============ ACTIVATION END ============`);
      setHwStatus('activated');
    } catch (err) {
      console.error(`[ProjectSync] ❌ Failed to activate project:`, err);
      setHwStatus('error');
    } finally {
      setIsAnimatingSync(false);
    }
  }, [client, fadeInLED, setIsAnimatingSync, setActiveProjectSync]);

  /**
   * Деактивировать проект (опустить актуаторы + выключить LED)
   */
  const deactivateProject = React.useCallback(async () => {
    if (!isConnectedRef.current || !activeProjectRef.current) return;

    const opts = optsRef.current;
    setHwStatus('deactivating');

    // Отменяем LED fade
    ledCancelRef.current.cancelled = true;

    const currentProject = activeProjectRef.current;
    const blocks = getBlocksForProject(currentProject.id);

    try {
      if (opts.enableActuators && blocks.length > 0) {
        console.log(`[ProjectSync] 📉 Lowering blocks...`);
        for (const blockNum of blocks) {
          await client.blockDown(blockNum, 5000);
          await new Promise(resolve => setTimeout(resolve, 200));
        }
        console.log(`[ProjectSync] ✅ Blocks lowered`);
      } else if (blocks.length > 0) {
        for (const blockNum of blocks) {
          await client.blockStop(blockNum);
        }
      }

      console.log(`[ProjectSync] ✅ Deactivated project: ${currentProject.name}`);
      setActiveProjectSync(null);
      setHwStatus('deactivated');
    } catch (err) {
      console.error(`[ProjectSync] ❌ Failed to deactivate project:`, err);
      setHwStatus('error');
    }
  }, [client, setActiveProjectSync]);

  /**
   * Экстренная остановка всех блоков
   */
  const emergencyStop = React.useCallback(async () => {
    if (!isConnectedRef.current) return;

    ledCancelRef.current.cancelled = true;

    try {
      await client.stopAll();
      await client.setLEDBrightness(0);
      setActiveProjectSync(null);
      setIsAnimatingSync(false);
      console.log("[ProjectSync] Emergency stop executed");
    } catch (err) {
      console.error("[ProjectSync] Emergency stop failed:", err);
    }
  }, [client, setActiveProjectSync, setIsAnimatingSync]);

  /**
   * Последовательно опустить все блоки (1-15)
   */
  const lowerAllBlocks = React.useCallback(async () => {
    if (!isConnectedRef.current) {
      console.warn("[ProjectSync] ❌ ESP32 not connected. Cannot lower blocks.");
      return;
    }

    if (isAnimatingRef.current) {
      console.warn("[ProjectSync] ❌ Animation in progress. Please wait.");
      return;
    }

    const opts = optsRef.current;

    setIsAnimatingSync(true);
    ledCancelRef.current.cancelled = true;
    console.log("[ProjectSync] ============ LOWER ALL START ============");

    try {
      if (opts.enableActuators) {
        console.log("[ProjectSync] 📉 Lowering all blocks sequentially with smooth LED fade (3s)...");
        for (let blockNum = 1; blockNum <= 15; blockNum++) {
          console.log(`[ProjectSync] 📉 Lowering block ${blockNum}/15...`);
          await client.blockDown(blockNum, 5000);
          await new Promise(resolve => setTimeout(resolve, 300));
        }
        console.log("[ProjectSync] ✅ All blocks lowered, LED faded out");
      }

      setActiveProjectSync(null);
      console.log("[ProjectSync] ============ LOWER ALL END ============");
    } catch (err) {
      console.error("[ProjectSync] ❌ Failed to lower all blocks:", err);
    } finally {
      setIsAnimatingSync(false);
    }
  }, [client, setIsAnimatingSync, setActiveProjectSync]);

  return {
    isConnected,
    activeProject,
    isAnimating,
    hwStatus,
    activateProject,
    deactivateProject,
    emergencyStop,
    lowerAllBlocks,
    client,
  };
}
