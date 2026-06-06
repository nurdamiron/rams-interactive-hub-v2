/**
 * Actuator Control Panel
 * Управление 15 актуаторными блоками через ESP32
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { ESP32Client, ESP32Status, createLocalESP32Client } from "@/lib/esp32-client";

interface ActuatorControlProps {
  onClose?: () => void;
  className?: string;
}

// All 8 effects available in ESP32 firmware (main.cpp:610-709)
const LED_EFFECTS = [
  { id: 0, name: "Статик", icon: "⬤" },
  { id: 1, name: "Пульс", icon: "◉" },
  { id: 2, name: "Радуга", icon: "🌈" },
  { id: 3, name: "Бег", icon: "➜" },
  { id: 4, name: "Искры", icon: "✨" },
  { id: 5, name: "Волна", icon: "〜" },
  { id: 6, name: "Огонь", icon: "🔥" },
  { id: 7, name: "Метеор", icon: "☄" },
];

export function ActuatorControl({ onClose, className = "" }: ActuatorControlProps) {
  const [client] = React.useState(() => createLocalESP32Client());
  const [status, setStatus] = React.useState<ESP32Status | null>(null);
  const [isConnected, setIsConnected] = React.useState(false);
  const [isLoading, setIsLoading] = React.useState(false);
  const [error, setError] = React.useState<string | null>(null);
  const [activeTab, setActiveTab] = React.useState<"actuators" | "led">("actuators");

  // LED Settings
  const [ledColor, setLedColor] = React.useState("#00BFFF");
  const [ledBrightness, setLedBrightness] = React.useState(200);
  const [ledSpeed, setLedSpeed] = React.useState(128);
  const [ledEffect, setLedEffect] = React.useState(5); // default to Wave (ID 5)
  const [autoCycle, setAutoCycle] = React.useState(false);
  const [cycleInterval, setCycleInterval] = React.useState(60);

  // Проверка подключения к ESP32
  const checkConnection = React.useCallback(async () => {
    try {
      const connected = await client.ping();
      setIsConnected(connected);
      if (connected) {
        const newStatus = await client.getStatus();
        setStatus(newStatus);
        setError(null);
      } else {
        setError("ESP32 недоступен. Проверьте WiFi подключение.");
      }
    } catch (err) {
      setIsConnected(false);
      setError(err instanceof Error ? err.message : "Ошибка подключения");
    }
  }, [client]);

  // Автоматическое обновление статуса каждые 2 секунды
  React.useEffect(() => {
    checkConnection();
    const interval = setInterval(checkConnection, 2000);
    return () => clearInterval(interval);
  }, [checkConnection]);

  // Управление блоком
  const handleControl = async (blockNum: number, action: "UP" | "DOWN" | "STOP") => {
    setIsLoading(true);
    setError(null);

    try {
      await client.controlBlock(blockNum, action, 10000);
      // Обновить статус сразу после команды
      await checkConnection();
    } catch (err) {
      setError(err instanceof Error ? err.message : "Ошибка управления");
    } finally {
      setIsLoading(false);
    }
  };

  // Остановить все блоки
  const handleStopAll = async () => {
    setIsLoading(true);
    setError(null);

    try {
      await client.stopAll();
      await checkConnection();
    } catch (err) {
      setError(err instanceof Error ? err.message : "Ошибка остановки");
    } finally {
      setIsLoading(false);
    }
  };

  // Проверка активности блока
  const isBlockActive = (blockNum: number): boolean => {
    return status?.blocks.includes(blockNum) ?? false;
  };

  // Определить тип блока (OUTER/INNER)
  const getBlockType = (blockNum: number): string => {
    if (blockNum === 15) return "FULL";
    return blockNum % 2 === 1 ? "OUTER" : "INNER";
  };

  // Use Electron IPC if available (avoids CORS), otherwise direct HTTP
  const electronApi = typeof window !== "undefined" ? (window as any).electron : null;

  // LED Control Handlers — via IPC to avoid CORS issues
  const handleColorChange = async (hex: string) => {
    setLedColor(hex);
    try {
      if (electronApi?.setLedColor) {
        await electronApi.setLedColor(hex);
      } else {
        const r = parseInt(hex.slice(1, 3), 16);
        const g = parseInt(hex.slice(3, 5), 16);
        const b = parseInt(hex.slice(5, 7), 16);
        await client.setLEDColor(r, g, b);
      }
    } catch (err) {
      console.error("Failed to set LED color:", err);
    }
  };

  const handleBrightnessChange = async (value: number) => {
    setLedBrightness(value);
    try {
      if (electronApi?.setLedBrightness) {
        await electronApi.setLedBrightness(value);
      } else {
        await client.setLEDBrightness(value);
      }
    } catch (err) {
      console.error("Failed to set LED brightness:", err);
    }
  };

  const handleSpeedChange = async (value: number) => {
    setLedSpeed(value);
    try {
      if (electronApi?.setLedSpeed) {
        await electronApi.setLedSpeed(value);
      } else {
        await client.setLEDSpeed(value);
      }
    } catch (err) {
      console.error("Failed to set LED speed:", err);
    }
  };

  const handleEffectChange = async (effectId: number) => {
    setLedEffect(effectId);
    if (autoCycle) setAutoCycle(false);

    try {
      if (electronApi?.setLedEffect) {
        await electronApi.setLedEffect(effectId);
      } else {
        await client.setLEDEffect(effectId);
      }
      console.log(`[LED] Effect changed to ID ${effectId}`);
    } catch (err) {
      console.error("Failed to set LED effect:", err);
    }
  };

  // Auto-cycle: switch between effects
  const autoCycleRef = React.useRef<ReturnType<typeof setInterval> | null>(null);
  const ledEffectRef = React.useRef(ledEffect);
  ledEffectRef.current = ledEffect;

  React.useEffect(() => {
    if (autoCycleRef.current) {
      clearInterval(autoCycleRef.current);
      autoCycleRef.current = null;
    }
    if (autoCycle) {
      const switchNext = async () => {
        const currentIdx = LED_EFFECTS.findIndex(e => e.id === ledEffectRef.current);
        const nextIdx = (currentIdx + 1) % LED_EFFECTS.length;
        const nextEffect = LED_EFFECTS[nextIdx];
        setLedEffect(nextEffect.id);
        try {
          if (electronApi?.setLedEffect) {
            await electronApi.setLedEffect(nextEffect.id);
          } else {
            await client.setLEDEffect(nextEffect.id);
          }
          console.log(`[AutoCycle] → ${nextEffect.name} (FW ID=${nextEffect.id})`);
        } catch (err) {
          console.error("Auto-cycle failed:", err);
        }
      };
      autoCycleRef.current = setInterval(switchNext, cycleInterval * 1000);
    }
    return () => {
      if (autoCycleRef.current) clearInterval(autoCycleRef.current);
    };
  }, [autoCycle, cycleInterval, client, electronApi]);

  return (
    <div className={`fixed inset-0 z-50 flex items-center justify-center bg-black/80 backdrop-blur-sm ${className}`}>
      <motion.div
        initial={{ opacity: 0, scale: 0.95 }}
        animate={{ opacity: 1, scale: 1 }}
        exit={{ opacity: 0, scale: 0.95 }}
        className="relative w-full max-w-6xl max-h-[90vh] overflow-hidden bg-gradient-to-br from-zinc-900 to-black border border-zinc-800 rounded-2xl shadow-2xl"
      >
        {/* Header */}
        <div className="sticky top-0 z-10 bg-zinc-900/95 backdrop-blur-md border-b border-zinc-800 p-6">
          <div className="flex items-center justify-between mb-4">
            <div>
              <h2 className="text-2xl font-bold text-white mb-1">
                RAMS Control Panel
              </h2>
              <p className="text-sm text-zinc-400">
                Актуаторы + LED Эффекты
              </p>
            </div>

            <div className="flex items-center gap-4">
              {/* Статус подключения */}
              <div className="flex items-center gap-2">
                <div className={`w-3 h-3 rounded-full ${isConnected ? "bg-green-500 animate-pulse" : "bg-red-500"}`} />
                <span className="text-sm text-zinc-400">
                  {isConnected ? "ESP32 Online" : "Offline"}
                </span>
              </div>

              {/* Stop All Button */}
              <button
                onClick={handleStopAll}
                disabled={isLoading || !isConnected}
                className="px-4 py-2 bg-red-600 hover:bg-red-700 disabled:bg-zinc-700 disabled:text-zinc-500 text-white rounded-lg font-medium transition-colors"
              >
                STOP ALL
              </button>

              {/* Close Button */}
              {onClose && (
                <button
                  onClick={onClose}
                  className="w-10 h-10 flex items-center justify-center rounded-lg bg-zinc-800 hover:bg-zinc-700 text-zinc-400 hover:text-white transition-colors"
                >
                  <svg className="w-5 h-5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M6 18L18 6M6 6l12 12" />
                  </svg>
                </button>
              )}
            </div>
          </div>

          {/* Error Message */}
          <AnimatePresence>
            {error && (
              <motion.div
                initial={{ opacity: 0, y: -10 }}
                animate={{ opacity: 1, y: 0 }}
                exit={{ opacity: 0, y: -10 }}
                className="mt-4 p-3 bg-red-500/10 border border-red-500/20 rounded-lg"
              >
                <p className="text-sm text-red-400">{error}</p>
              </motion.div>
            )}
          </AnimatePresence>

          {/* Tabs */}
          <div className="flex gap-2 mt-4">
            <button
              onClick={() => setActiveTab("actuators")}
              className={`px-6 py-2 rounded-lg font-medium transition-all ${
                activeTab === "actuators"
                  ? "bg-cyan-600 text-white shadow-lg shadow-cyan-500/20"
                  : "bg-zinc-800 text-zinc-400 hover:bg-zinc-700 hover:text-white"
              }`}
            >
              Актуаторы
            </button>
            <button
              onClick={() => setActiveTab("led")}
              className={`px-6 py-2 rounded-lg font-medium transition-all ${
                activeTab === "led"
                  ? "bg-cyan-600 text-white shadow-lg shadow-cyan-500/20"
                  : "bg-zinc-800 text-zinc-400 hover:bg-zinc-700 hover:text-white"
              }`}
            >
              LED Эффекты
            </button>
          </div>
        </div>

        {/* Content */}
        <div className="p-6 overflow-y-auto max-h-[calc(90vh-200px)]">
          {/* Actuators Tab */}
          {activeTab === "actuators" && (
            <div className="grid grid-cols-3 gap-4">
              {Array.from({ length: 15 }, (_, i) => i + 1).map((blockNum) => {
              const active = isBlockActive(blockNum);
              const blockType = getBlockType(blockNum);

              return (
                <motion.div
                  key={blockNum}
                  initial={{ opacity: 0, y: 20 }}
                  animate={{ opacity: 1, y: 0 }}
                  transition={{ delay: blockNum * 0.02 }}
                  className={`relative p-4 rounded-xl border-2 transition-all ${
                    active
                      ? "bg-green-500/10 border-green-500/50 shadow-lg shadow-green-500/20"
                      : "bg-zinc-800/50 border-zinc-700"
                  }`}
                >
                  {/* Block Header */}
                  <div className="flex items-center justify-between mb-3">
                    <div>
                      <h3 className="text-lg font-bold text-white">Блок {blockNum}</h3>
                      <p className="text-xs text-zinc-400">
                        {blockType} • Доля {Math.floor((blockNum - 1) / 2) + 1}
                      </p>
                    </div>
                    {active && (
                      <div className="w-3 h-3 rounded-full bg-green-500 animate-pulse" />
                    )}
                  </div>

                  {/* Control Buttons */}
                  <div className="flex gap-2">
                    <button
                      onClick={() => handleControl(blockNum, "UP")}
                      disabled={isLoading || !isConnected}
                      className="flex-1 py-2 px-3 bg-green-600 hover:bg-green-700 disabled:bg-zinc-700 disabled:text-zinc-500 text-white text-sm font-medium rounded-lg transition-colors"
                    >
                      UP
                    </button>
                    <button
                      onClick={() => handleControl(blockNum, "DOWN")}
                      disabled={isLoading || !isConnected}
                      className="flex-1 py-2 px-3 bg-orange-600 hover:bg-orange-700 disabled:bg-zinc-700 disabled:text-zinc-500 text-white text-sm font-medium rounded-lg transition-colors"
                    >
                      DOWN
                    </button>
                    <button
                      onClick={() => handleControl(blockNum, "STOP")}
                      disabled={isLoading || !isConnected}
                      className="flex-1 py-2 px-3 bg-red-600 hover:bg-red-700 disabled:bg-zinc-700 disabled:text-zinc-500 text-white text-sm font-medium rounded-lg transition-colors"
                    >
                      STOP
                    </button>
                  </div>
                </motion.div>
              );
            })}
            </div>
          )}

          {/* LED Tab */}
          {activeTab === "led" && (
            <div className="space-y-6">
              {/* Color Picker */}
              <div className="bg-zinc-800/50 rounded-xl p-6 border border-zinc-700">
                <h3 className="text-lg font-bold text-white mb-4">Цвет</h3>
                <div className="flex items-center gap-4">
                  <input
                    type="color"
                    value={ledColor}
                    onChange={(e) => handleColorChange(e.target.value)}
                    className="w-20 h-20 rounded-lg cursor-pointer border-2 border-zinc-600"
                  />
                  <div className="flex-1">
                    <div className="text-2xl font-mono text-white mb-1">{ledColor.toUpperCase()}</div>
                    <div className="text-sm text-zinc-400">
                      RGB: {parseInt(ledColor.slice(1, 3), 16)}, {parseInt(ledColor.slice(3, 5), 16)}, {parseInt(ledColor.slice(5, 7), 16)}
                    </div>
                  </div>
                </div>
              </div>

              {/* Brightness */}
              <div className="bg-zinc-800/50 rounded-xl p-6 border border-zinc-700">
                <div className="flex items-center justify-between mb-4">
                  <h3 className="text-lg font-bold text-white">Яркость</h3>
                  <span className="text-xl font-mono text-cyan-400">{ledBrightness}</span>
                </div>
                <input
                  type="range"
                  min="0"
                  max="255"
                  value={ledBrightness}
                  onChange={(e) => handleBrightnessChange(parseInt(e.target.value))}
                  className="w-full h-2 bg-zinc-700 rounded-lg appearance-none cursor-pointer accent-cyan-500"
                />
                <div className="flex justify-between text-xs text-zinc-500 mt-2">
                  <span>0</span>
                  <span>128</span>
                  <span>255</span>
                </div>
              </div>

              {/* Speed */}
              <div className="bg-zinc-800/50 rounded-xl p-6 border border-zinc-700">
                <div className="flex items-center justify-between mb-4">
                  <h3 className="text-lg font-bold text-white">Скорость анимации</h3>
                  <span className="text-xl font-mono text-cyan-400">{ledSpeed}</span>
                </div>
                <input
                  type="range"
                  min="0"
                  max="255"
                  value={ledSpeed}
                  onChange={(e) => handleSpeedChange(parseInt(e.target.value))}
                  className="w-full h-2 bg-zinc-700 rounded-lg appearance-none cursor-pointer accent-cyan-500"
                />
                <div className="flex justify-between text-xs text-zinc-500 mt-2">
                  <span>Медленно</span>
                  <span>Средне</span>
                  <span>Быстро</span>
                </div>
              </div>

              {/* Effects */}
              <div className="bg-zinc-800/50 rounded-xl p-6 border border-zinc-700">
                <div className="flex items-center justify-between mb-4">
                  <h3 className="text-lg font-bold text-white">Эффекты</h3>
                  <button
                    onClick={() => setAutoCycle(!autoCycle)}
                    className={`flex items-center gap-2 px-3 py-1.5 rounded-lg text-sm font-medium transition-all ${
                      autoCycle
                        ? "bg-cyan-600 text-white"
                        : "bg-zinc-700 text-zinc-400 hover:bg-zinc-600 hover:text-white"
                    }`}
                  >
                    <span style={autoCycle ? { display: "inline-block", animation: "spin 3s linear infinite" } : undefined}>&#x21bb;</span>
                    Авто {autoCycle ? "ON" : "OFF"}
                  </button>
                </div>

                {/* Auto-cycle interval */}
                {autoCycle && (
                  <div className="flex gap-2 mb-4">
                    {[30, 60, 120, 300].map((sec) => (
                      <button
                        key={sec}
                        onClick={() => setCycleInterval(sec)}
                        className={`flex-1 py-2 rounded-lg text-sm font-medium transition-all ${
                          cycleInterval === sec
                            ? "bg-cyan-600 text-white"
                            : "bg-zinc-700 text-zinc-400 hover:bg-zinc-600"
                        }`}
                      >
                        {sec < 60 ? `${sec}с` : `${sec / 60}м`}
                      </button>
                    ))}
                  </div>
                )}

                <div className="grid grid-cols-4 gap-3">
                  {LED_EFFECTS.map((effect) => (
                    <button
                      key={effect.id}
                      onClick={() => handleEffectChange(effect.id)}
                      className={`p-4 rounded-xl border-2 transition-all ${
                        ledEffect === effect.id
                          ? "bg-cyan-600 border-cyan-500 text-white shadow-lg shadow-cyan-500/20"
                          : "bg-zinc-800 border-zinc-700 text-zinc-400 hover:bg-zinc-700 hover:border-zinc-600 hover:text-white"
                      }`}
                    >
                      <div className="text-3xl mb-2">{effect.icon}</div>
                      <div className="text-sm font-medium">{effect.name}</div>
                    </button>
                  ))}
                </div>
              </div>

              {/* Preset Colors */}
              <div className="bg-zinc-800/50 rounded-xl p-6 border border-zinc-700">
                <h3 className="text-lg font-bold text-white mb-4">Быстрый выбор цвета</h3>
                <div className="flex gap-3">
                  {[
                    { name: "Красный", color: "#FF0000" },
                    { name: "Зеленый", color: "#00FF00" },
                    { name: "Синий", color: "#0000FF" },
                    { name: "Циан", color: "#00FFFF" },
                    { name: "Пурпурный", color: "#FF00FF" },
                    { name: "Желтый", color: "#FFFF00" },
                    { name: "Белый", color: "#FFFFFF" },
                    { name: "Оранжевый", color: "#FF6600" },
                  ].map((preset) => (
                    <button
                      key={preset.color}
                      onClick={() => handleColorChange(preset.color)}
                      className="flex-1 p-3 rounded-lg border-2 border-zinc-700 hover:border-zinc-600 transition-all"
                      style={{ backgroundColor: preset.color }}
                      title={preset.name}
                    >
                      <div className="h-8" />
                    </button>
                  ))}
                </div>
              </div>
            </div>
          )}
        </div>

        {/* Legend */}
        <div className="sticky bottom-0 bg-zinc-900/95 backdrop-blur-md border-t border-zinc-800 p-4">
          <div className="flex items-center justify-center gap-6 text-xs text-zinc-400">
            <div className="flex items-center gap-2">
              <div className="w-3 h-3 rounded bg-green-600" />
              <span>OUTER: внешняя часть + внешний круг</span>
            </div>
            <div className="flex items-center gap-2">
              <div className="w-3 h-3 rounded bg-blue-600" />
              <span>INNER: внутренняя часть + внутренний круг</span>
            </div>
            <div className="flex items-center gap-2">
              <div className="w-3 h-3 rounded bg-purple-600" />
              <span>FULL: весь луч (блок 15)</span>
            </div>
          </div>
        </div>
      </motion.div>
    </div>
  );
}
