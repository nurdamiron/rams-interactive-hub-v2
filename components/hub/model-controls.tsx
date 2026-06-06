/**
 * Model Controls Component
 * Interactive controls for physical model elements
 */

"use client";

import * as React from "react";
import { motion } from "framer-motion";
import { cn } from "@/lib/utils";
import { Icon } from "@/components/ui/icon";
import { ModelElement, ModelControlState } from "@/lib/types";

export interface ModelControlsProps {
  controls: ModelControlState[];
  onToggle: (element: ModelElement) => void;
  theme?: "light" | "dark";
  layout?: "grid" | "list";
}

export const ModelControls: React.FC<ModelControlsProps> = ({
  controls,
  onToggle,
  theme = "dark",
  layout = "grid",
}) => {
  const isDark = theme === "dark";

  return (
    <div
      className={cn(
        "grid gap-4",
        layout === "grid" ? "grid-cols-2" : "grid-cols-1"
      )}
    >
      {controls.map((control) => (
        <motion.button
          key={control.element}
          onClick={() => onToggle(control.element)}
          className={cn(
            "relative p-6 rounded-2xl border-2 transition-all cursor-pointer",
            "flex flex-col items-center justify-center gap-4 h-32",
            control.isActive
              ? isDark
                ? "bg-primary/20 border-primary text-primary shadow-[0_0_20px_rgba(196,159,100,0.3)]"
                : "bg-primary-dark/20 border-primary-dark text-primary-dark shadow-[0_0_20px_rgba(196,159,100,0.3)]"
              : isDark
                ? "bg-white/5 border-white/5 text-text-muted-dark hover:border-white/20 hover:bg-white/10"
                : "bg-surface-light border-border-light text-text-muted hover:border-primary-dark/50"
          )}
          whileHover={{ scale: 1.05, y: -2 }}
          whileTap={{ scale: 0.95 }}
        >
          {/* Icon */}
          <div className={cn(
            "p-3 rounded-xl transition-colors",
            control.isActive
              ? "bg-primary text-black"
              : "bg-white/5 text-text-muted-dark"
          )}>
            <Icon
              name={control.icon || 'circle'}
              size="xl"
            />
          </div>

          {/* Label */}
          <span className="text-xs font-black tracking-widest uppercase text-center leading-tight">
            {control.element}
          </span>

          {/* Active Indicator */}
          {control.isActive && (
            <motion.div
              className={cn(
                "absolute top-1.5 right-1.5 w-2 h-2 rounded-full",
                isDark ? "bg-primary" : "bg-primary-dark"
              )}
              layoutId={`active-${control.element}`}
              initial={{ scale: 0 }}
              animate={{ scale: 1 }}
              exit={{ scale: 0 }}
            >
              <motion.span
                className={cn(
                  "absolute inset-0 rounded-full",
                  isDark ? "bg-primary" : "bg-primary-dark"
                )}
                animate={{ scale: [1, 1.5, 1], opacity: [1, 0, 1] }}
                transition={{ duration: 2, repeat: Infinity }}
              />
            </motion.div>
          )}
        </motion.button>
      ))}
    </div>
  );
};
