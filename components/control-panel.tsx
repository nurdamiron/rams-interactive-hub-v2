/**
 * Control Panel Component
 * Центральная панель управления системой
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { cn } from "@/lib/utils";
import { ESP32Client } from "@/lib/esp32-client";

export interface ControlPanelProps {
  esp32Client?: ESP32Client;
  isConnected: boolean;
  isAnimating: boolean;
  onOpenActuatorControl: () => void;
  onOpenAdmin: () => void;
  onLowerAllBlocks: () => void;
  onEmergencyStop: () => void;
  onReconnect: () => void;
}

export const ControlPanel: React.FC<ControlPanelProps> = ({
  esp32Client,
  isConnected,
  isAnimating,
  onOpenActuatorControl,
  onOpenAdmin,
  onLowerAllBlocks,
  onEmergencyStop,
  onReconnect,
}) => {
  const [isExpanded, setIsExpanded] = React.useState(false);
  const [blockStats, setBlockStats] = React.useState({ active: 0, blocks: [] as number[] });

  // Обновление статистики блоков
  React.useEffect(() => {
    if (!esp32Client || !isConnected) return;

    const updateStats = async () => {
      try {
        const status = await esp32Client.getStatus();
        setBlockStats({ active: status.active, blocks: status.blocks });
      } catch (err) {
        console.error("[ControlPanel] Failed to get status:", err);
      }
    };

    updateStats();
    const interval = setInterval(updateStats, 3000); // Обновлять каждые 3 сек
    return () => clearInterval(interval);
  }, [esp32Client, isConnected]);

  return (
    <motion.div
      className="fixed bottom-8 left-8 z-50"
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.4 }}
    >
      {/* Кнопка раскрытия панели */}
      <motion.button
        onClick={() => setIsExpanded(!isExpanded)}
        className={cn(
          "flex items-center gap-3 px-6 py-4 rounded-2xl shadow-2xl backdrop-blur-md transition-all duration-300",
          "border border-white/10",
          isExpanded
            ? "bg-gradient-to-r from-cyan-600/90 to-blue-600/90 hover:from-cyan-500/90 hover:to-blue-500/90"
            : "bg-gradient-to-r from-cyan-600/80 to-blue-600/80 hover:from-cyan-500/80 hover:to-blue-500/80"
        )}
        whileHover={{ scale: 1.02 }}
        whileTap={{ scale: 0.98 }}
      >
        {/* Иконка */}
        <motion.svg
          className="w-6 h-6 text-white"
          fill="none"
          stroke="currentColor"
          viewBox="0 0 24 24"
          animate={{ rotate: isExpanded ? 180 : 0 }}
          transition={{ duration: 0.3 }}
        >
          <path
            strokeLinecap="round"
            strokeLinejoin="round"
            strokeWidth={2}
            d="M12 6V4m0 2a2 2 0 100 4m0-4a2 2 0 110 4m-6 8a2 2 0 100-4m0 4a2 2 0 110-4m0 4v2m0-6V4m6 6v10m6-2a2 2 0 100-4m0 4a2 2 0 110-4m0 4v2m0-6V4"
          />
        </motion.svg>

        {/* Текст */}
        <div className="flex flex-col items-start">
          <span className="text-white font-bold text-lg">Управление</span>
          {isConnected && (
            <span className="text-white/70 text-xs">
              {blockStats.active} блоков активно
            </span>
          )}
        </div>

        {/* Индикатор состояния */}
        <motion.div
          className={cn(
            "w-3 h-3 rounded-full ml-2",
            isConnected ? "bg-green-400" : "bg-red-400"
          )}
          animate={
            isConnected
              ? { scale: [1, 1.2, 1], opacity: [1, 0.7, 1] }
              : { opacity: [1, 0.5, 1] }
          }
          transition={{ duration: 2, repeat: Infinity }}
        />
      </motion.button>

      {/* Раскрытая панель управления */}
      <AnimatePresence>
        {isExpanded && (
          <motion.div
            className="mt-4 space-y-3"
            initial={{ opacity: 0, y: -10, scale: 0.95 }}
            animate={{ opacity: 1, y: 0, scale: 1 }}
            exit={{ opacity: 0, y: -10, scale: 0.95 }}
            transition={{ duration: 0.2 }}
          >
            {/* Статистика */}
            {isConnected && (
              <motion.div
                className="bg-black/40 backdrop-blur-md rounded-xl p-4 border border-white/10"
                initial={{ opacity: 0 }}
                animate={{ opacity: 1 }}
                transition={{ delay: 0.1 }}
              >
                <div className="text-white/60 text-xs mb-2">Статус системы</div>
                <div className="flex items-center justify-between">
                  <div>
                    <div className="text-white font-bold text-2xl">
                      {blockStats.active}/15
                    </div>
                    <div className="text-white/50 text-xs">активных блоков</div>
                  </div>
                  <div className="flex flex-wrap gap-1 max-w-[120px]">
                    {Array.from({ length: 15 }, (_, i) => i + 1).map((num) => (
                      <div
                        key={num}
                        className={cn(
                          "w-5 h-5 rounded text-xs flex items-center justify-center font-mono",
                          blockStats.blocks.includes(num)
                            ? "bg-green-500 text-white"
                            : "bg-gray-700/50 text-gray-500"
                        )}
                      >
                        {num}
                      </div>
                    ))}
                  </div>
                </div>
              </motion.div>
            )}

            {/* Кнопки управления */}
            <div className="space-y-2">
              {/* Emergency Stop */}
              {isConnected && (
                <ControlButton
                  icon={
                    <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path
                        strokeLinecap="round"
                        strokeLinejoin="round"
                        strokeWidth={2}
                        d="M21 12a9 9 0 11-18 0 9 9 0 0118 0z"
                      />
                      <path
                        strokeLinecap="round"
                        strokeLinejoin="round"
                        strokeWidth={2}
                        d="M9 10h6v4H9z"
                      />
                    </svg>
                  }
                  label="СТОП"
                  onClick={onEmergencyStop}
                  variant="danger"
                  disabled={isAnimating}
                />
              )}

              {/* Опустить все блоки */}
              {isConnected && (
                <ControlButton
                  icon={
                    <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path
                        strokeLinecap="round"
                        strokeLinejoin="round"
                        strokeWidth={2}
                        d="M19 14l-7 7m0 0l-7-7m7 7V3"
                      />
                    </svg>
                  }
                  label="Опустить все"
                  onClick={onLowerAllBlocks}
                  variant="warning"
                  disabled={isAnimating}
                />
              )}

              {/* Управление актуаторами */}
              {isConnected && (
                <ControlButton
                  icon={
                    <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path
                        strokeLinecap="round"
                        strokeLinejoin="round"
                        strokeWidth={2}
                        d="M12 6V4m0 2a2 2 0 100 4m0-4a2 2 0 110 4m-6 8a2 2 0 100-4m0 4a2 2 0 110-4m0 4v2m0-6V4m6 6v10m6-2a2 2 0 100-4m0 4a2 2 0 110-4m0 4v2m0-6V4"
                      />
                    </svg>
                  }
                  label="Панель управления"
                  onClick={onOpenActuatorControl}
                  variant="primary"
                  disabled={isAnimating}
                />
              )}

              {/* Переподключение */}
              {!isConnected && (
                <ControlButton
                  icon={
                    <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path
                        strokeLinecap="round"
                        strokeLinejoin="round"
                        strokeWidth={2}
                        d="M4 4v5h.582m15.356 2A8.001 8.001 0 004.582 9m0 0H9m11 11v-5h-.581m0 0a8.003 8.003 0 01-15.357-2m15.357 2H15"
                      />
                    </svg>
                  }
                  label="Переподключить"
                  onClick={onReconnect}
                  variant="secondary"
                />
              )}

              {/* Admin Panel */}
              <ControlButton
                icon={
                  <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path
                      strokeLinecap="round"
                      strokeLinejoin="round"
                      strokeWidth={2}
                      d="M10.325 4.317c.426-1.756 2.924-1.756 3.35 0a1.724 1.724 0 002.573 1.066c1.543-.94 3.31.826 2.37 2.37a1.724 1.724 0 001.065 2.572c1.756.426 1.756 2.924 0 3.35a1.724 1.724 0 00-1.066 2.573c.94 1.543-.826 3.31-2.37 2.37a1.724 1.724 0 00-2.572 1.065c-.426 1.756-2.924 1.756-3.35 0a1.724 1.724 0 00-2.573-1.066c-1.543.94-3.31-.826-2.37-2.37a1.724 1.724 0 00-1.065-2.572c-1.756-.426-1.756-2.924 0-3.35a1.724 1.724 0 001.066-2.573c-.94-1.543.826-3.31 2.37-2.37.996.608 2.296.07 2.572-1.065z"
                    />
                    <path
                      strokeLinecap="round"
                      strokeLinejoin="round"
                      strokeWidth={2}
                      d="M15 12a3 3 0 11-6 0 3 3 0 016 0z"
                    />
                  </svg>
                }
                label="Настройки"
                onClick={onOpenAdmin}
                variant="secondary"
              />
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </motion.div>
  );
};

// Компонент кнопки управления
interface ControlButtonProps {
  icon: React.ReactNode;
  label: string;
  onClick: () => void;
  variant?: "primary" | "secondary" | "danger" | "warning";
  disabled?: boolean;
}

const ControlButton: React.FC<ControlButtonProps> = ({
  icon,
  label,
  onClick,
  variant = "primary",
  disabled = false,
}) => {
  const variantStyles = {
    primary: "bg-cyan-600/90 hover:bg-cyan-500/90 border-cyan-500/30",
    secondary: "bg-gray-600/90 hover:bg-gray-500/90 border-gray-500/30",
    danger: "bg-red-600/90 hover:bg-red-500/90 border-red-500/30",
    warning: "bg-orange-600/90 hover:bg-orange-500/90 border-orange-500/30",
  };

  return (
    <motion.button
      onClick={onClick}
      disabled={disabled}
      className={cn(
        "w-full flex items-center gap-3 px-4 py-3 rounded-xl backdrop-blur-md shadow-lg transition-all duration-200",
        "border text-white font-medium",
        variantStyles[variant],
        disabled && "opacity-50 cursor-not-allowed"
      )}
      whileHover={!disabled ? { scale: 1.02, x: 4 } : {}}
      whileTap={!disabled ? { scale: 0.98 } : {}}
    >
      <div className="flex-shrink-0">{icon}</div>
      <span className="text-sm">{label}</span>
    </motion.button>
  );
};
