/**
 * Theme Toggle Component
 * Animated switch for dark/light mode
 */

"use client";

import * as React from "react";
import { motion } from "framer-motion";
import { cn } from "@/lib/utils";
import { useTheme } from "@/lib/theme-context";
import { Icon } from "./icon";

export interface ThemeToggleProps {
  className?: string;
  variant?: "default" | "compact";
}

export const ThemeToggle: React.FC<ThemeToggleProps> = ({
  className,
  variant = "default",
}) => {
  const { theme, toggleTheme } = useTheme();
  const isDark = theme === "dark";

  if (variant === "compact") {
    return (
      <motion.button
        onClick={toggleTheme}
        className={cn(
          "relative w-12 h-12 rounded-full flex items-center justify-center",
          "border transition-colors",
          isDark
            ? "bg-surface-dark border-border-dark hover:border-primary"
            : "bg-surface-light border-border-light hover:border-primary-dark",
          className
        )}
        whileHover={{ scale: 1.05 }}
        whileTap={{ scale: 0.95 }}
      >
        <motion.div
          initial={false}
          animate={{
            rotate: isDark ? 0 : 180,
            scale: isDark ? 1 : 0.8,
          }}
          transition={{ duration: 0.3, ease: "easeInOut" }}
        >
          <Icon
            name={isDark ? "dark_mode" : "light_mode"}
            className={isDark ? "text-primary" : "text-primary-dark"}
          />
        </motion.div>
      </motion.button>
    );
  }

  return (
    <motion.button
      onClick={toggleTheme}
      className={cn(
        "relative flex items-center gap-3 px-4 py-2 rounded-full",
        "border transition-colors",
        isDark
          ? "bg-surface-dark border-border-dark hover:border-primary"
          : "bg-surface-light border-border-light hover:border-primary-dark",
        className
      )}
      whileHover={{ scale: 1.02 }}
      whileTap={{ scale: 0.98 }}
    >
      {/* Icon Container */}
      <div className="relative w-6 h-6">
        {/* Sun Icon */}
        <motion.div
          className="absolute inset-0 flex items-center justify-center"
          initial={false}
          animate={{
            scale: isDark ? 0 : 1,
            rotate: isDark ? 90 : 0,
            opacity: isDark ? 0 : 1,
          }}
          transition={{ duration: 0.3, ease: "easeInOut" }}
        >
          <Icon name="light_mode" className="text-primary-dark" />
        </motion.div>

        {/* Moon Icon */}
        <motion.div
          className="absolute inset-0 flex items-center justify-center"
          initial={false}
          animate={{
            scale: isDark ? 1 : 0,
            rotate: isDark ? 0 : -90,
            opacity: isDark ? 1 : 0,
          }}
          transition={{ duration: 0.3, ease: "easeInOut" }}
        >
          <Icon name="dark_mode" className="text-primary" />
        </motion.div>
      </div>

      {/* Label */}
      <span
        className={cn(
          "text-sm font-medium tracking-wide",
          isDark ? "text-text-light" : "text-text-dark"
        )}
      >
        {isDark ? "Dark" : "Light"}
      </span>

      {/* Toggle Track */}
      <div
        className={cn(
          "relative w-12 h-6 rounded-full transition-colors",
          isDark ? "bg-border-dark" : "bg-border-light"
        )}
      >
        <motion.div
          className={cn(
            "absolute top-1 w-4 h-4 rounded-full",
            isDark ? "bg-primary" : "bg-primary-dark"
          )}
          initial={false}
          animate={{
            left: isDark ? "calc(100% - 1.25rem)" : "0.25rem",
          }}
          transition={{
            type: "spring",
            stiffness: 500,
            damping: 30,
          }}
        />
      </div>
    </motion.button>
  );
};
