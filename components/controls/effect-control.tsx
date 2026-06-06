"use client"

import { useState, useEffect, useRef } from "react"
import { motion, AnimatePresence } from "framer-motion"
import { Sparkles, Waves, Flame, Zap, ChevronDown, ChevronUp, RefreshCw } from "lucide-react"
import { ESP32Client } from "@/lib/esp32-client"
import { cn } from "@/lib/utils"

// All 8 effects available in ESP32 firmware (main.cpp:610-709)
const EFFECTS = [
  { id: 0, name: "Static", icon: Zap, description: "Статичный цвет" },
  { id: 1, name: "Pulse", icon: Zap, description: "Пульсация" },
  { id: 2, name: "Rainbow", icon: Sparkles, description: "Радуга" },
  { id: 3, name: "Chase", icon: Waves, description: "Бегущий огонь" },
  { id: 4, name: "Sparkle", icon: Sparkles, description: "Искры" },
  { id: 5, name: "Wave", icon: Waves, description: "Волна" },
  { id: 6, name: "Fire", icon: Flame, description: "Огонь" },
  { id: 7, name: "Meteor", icon: Zap, description: "Метеор" },
]

interface EffectControlProps {
  esp32Client: ESP32Client
  className?: string
}

export function EffectControl({ esp32Client, className }: EffectControlProps) {
  const [selectedEffect, setSelectedEffect] = useState(0)
  const [speed, setSpeed] = useState(128)
  const [isExpanded, setIsExpanded] = useState(false)
  const [isLoading, setIsLoading] = useState(false)
  const [autoCycle, setAutoCycle] = useState(false)
  const [cycleInterval, setCycleInterval] = useState(60)
  const autoCycleTimer = useRef<ReturnType<typeof setInterval> | null>(null)

  // Use Electron IPC if available (avoids CORS)
  const electronApi = typeof window !== "undefined" ? (window as any).electron : null

  const sendEffect = async (effectId: number, spd?: number) => {
    if (electronApi?.setLedEffect) {
      await electronApi.setLedEffect(effectId, spd)
    } else {
      await esp32Client.setLEDEffect(effectId, spd)
    }
  }

  // Auto-cycle: switch effect every N seconds (stable ref to avoid interval recreation)
  const selectedEffectRef = useRef(selectedEffect)
  selectedEffectRef.current = selectedEffect

  useEffect(() => {
    if (autoCycleTimer.current) {
      clearInterval(autoCycleTimer.current)
      autoCycleTimer.current = null
    }
    if (autoCycle) {
      const switchNext = async () => {
        const nextId = (selectedEffectRef.current + 1) % EFFECTS.length
        try {
          await sendEffect(nextId)
          setSelectedEffect(nextId)
          console.log(`[AutoCycle] → ${EFFECTS[nextId].name} (id=${nextId})`)
        } catch (error) {
          console.error(`[AutoCycle] Failed:`, error)
        }
      }
      autoCycleTimer.current = setInterval(switchNext, cycleInterval * 1000)
    }
    return () => {
      if (autoCycleTimer.current) clearInterval(autoCycleTimer.current)
    }
  }, [autoCycle, cycleInterval, esp32Client, electronApi])

  const handleEffectChange = async (effectId: number) => {
    try {
      setIsLoading(true)
      if (autoCycle) setAutoCycle(false)
      await sendEffect(effectId)
      setSelectedEffect(effectId)
    } catch (error) {
      console.error(`Failed to set effect:`, error)
    } finally {
      setIsLoading(false)
    }
  }

  const handleSpeedChange = async (newSpeed: number) => {
    setSpeed(newSpeed)
    try {
      await sendEffect(selectedEffect, newSpeed)
    } catch (error) {
      console.error(`Failed to set speed:`, error)
    }
  }

  return (
    <motion.div
      initial={{ opacity: 0, y: -10 }}
      animate={{ opacity: 1, y: 0 }}
      className={cn(
        "backdrop-blur-md bg-black/20 border border-white/10 rounded-xl overflow-hidden",
        className
      )}
    >
      {/* Header */}
      <button
        onClick={() => setIsExpanded(!isExpanded)}
        className="w-full px-4 py-3 flex items-center justify-between hover:bg-white/5 transition-colors"
      >
        <div className="flex items-center gap-2">
          <Sparkles className="w-4 h-4 text-cyan-400" />
          <span className="text-sm font-medium text-white">LED Effects</span>
          <span className="text-xs text-gray-400">
            {EFFECTS[selectedEffect]?.name}
          </span>
          {autoCycle && (
            <RefreshCw className="w-3 h-3 text-cyan-400 animate-spin" style={{ animationDuration: "3s" }} />
          )}
        </div>
        {isExpanded ? (
          <ChevronUp className="w-4 h-4 text-gray-400" />
        ) : (
          <ChevronDown className="w-4 h-4 text-gray-400" />
        )}
      </button>

      {/* Expanded Content */}
      <AnimatePresence>
        {isExpanded && (
          <motion.div
            initial={{ height: 0, opacity: 0 }}
            animate={{ height: "auto", opacity: 1 }}
            exit={{ height: 0, opacity: 0 }}
            transition={{ duration: 0.2 }}
            className="border-t border-white/10"
          >
            <div className="p-4 space-y-4">
              {/* Effect Buttons */}
              <div className="grid grid-cols-4 sm:grid-cols-8 gap-2">
                {EFFECTS.map((effect) => {
                  const Icon = effect.icon
                  const isActive = selectedEffect === effect.id

                  return (
                    <motion.button
                      key={effect.id}
                      whileHover={{ scale: 1.05 }}
                      whileTap={{ scale: 0.95 }}
                      onClick={() => handleEffectChange(effect.id)}
                      disabled={isLoading}
                      className={cn(
                        "relative flex flex-col items-center gap-1.5 p-3 rounded-lg border transition-all",
                        isActive
                          ? "bg-cyan-500/20 border-cyan-400/50 text-cyan-300"
                          : "bg-white/5 border-white/10 text-gray-400 hover:bg-white/10 hover:border-white/20",
                        isLoading && "opacity-50 cursor-not-allowed"
                      )}
                      title={effect.description}
                    >
                      <Icon className={cn("w-5 h-5", isActive && "text-cyan-400")} />
                      <span className="text-xs font-medium">{effect.name}</span>

                      {/* Active Indicator */}
                      {isActive && (
                        <motion.div
                          layoutId="activeEffect"
                          className="absolute inset-0 border-2 border-cyan-400 rounded-lg"
                          transition={{ type: "spring", bounce: 0.2, duration: 0.6 }}
                        />
                      )}
                    </motion.button>
                  )
                })}
              </div>

              {/* Speed Control */}
              {selectedEffect !== 0 && (
                <motion.div
                  initial={{ opacity: 0, y: -10 }}
                  animate={{ opacity: 1, y: 0 }}
                  className="space-y-2"
                >
                  <div className="flex items-center justify-between text-xs">
                    <span className="text-gray-400">Speed</span>
                    <span className="text-cyan-400 font-mono">{speed}</span>
                  </div>
                  <input
                    type="range"
                    min="0"
                    max="255"
                    value={speed}
                    onChange={(e) => handleSpeedChange(parseInt(e.target.value))}
                    className="w-full h-2 bg-white/10 rounded-lg appearance-none cursor-pointer accent-cyan-500"
                  />
                  <div className="flex justify-between text-xs text-gray-500">
                    <span>Slow</span>
                    <span>Fast</span>
                  </div>
                </motion.div>
              )}

              {/* Auto-Cycle Control */}
              <div className="space-y-2 pt-2 border-t border-white/10">
                <button
                  onClick={() => setAutoCycle(!autoCycle)}
                  className={cn(
                    "w-full flex items-center justify-between px-3 py-2 rounded-lg border transition-all",
                    autoCycle
                      ? "bg-cyan-500/20 border-cyan-400/50 text-cyan-300"
                      : "bg-white/5 border-white/10 text-gray-400 hover:bg-white/10"
                  )}
                >
                  <div className="flex items-center gap-2">
                    <RefreshCw className={cn("w-4 h-4", autoCycle && "animate-spin")} style={autoCycle ? { animationDuration: "3s" } : undefined} />
                    <span className="text-sm font-medium">Auto-Cycle</span>
                  </div>
                  <span className="text-xs">{autoCycle ? "ON" : "OFF"}</span>
                </button>
                {autoCycle && (
                  <motion.div
                    initial={{ opacity: 0, height: 0 }}
                    animate={{ opacity: 1, height: "auto" }}
                    className="space-y-1"
                  >
                    <div className="flex items-center justify-between text-xs">
                      <span className="text-gray-400">Interval</span>
                      <span className="text-cyan-400 font-mono">{cycleInterval}s</span>
                    </div>
                    <div className="flex gap-1">
                      {[30, 60, 120, 300].map((sec) => (
                        <button
                          key={sec}
                          onClick={() => setCycleInterval(sec)}
                          className={cn(
                            "flex-1 py-1.5 rounded text-xs font-medium transition-colors",
                            cycleInterval === sec
                              ? "bg-cyan-500/30 text-cyan-300"
                              : "bg-white/5 text-gray-400 hover:bg-white/10"
                          )}
                        >
                          {sec < 60 ? `${sec}s` : `${sec / 60}m`}
                        </button>
                      ))}
                    </div>
                  </motion.div>
                )}
              </div>
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </motion.div>
  )
}
