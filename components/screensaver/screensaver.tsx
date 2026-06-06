/**
 * Screensaver Component
 * Shows after idle timeout (default 5 minutes)
 * Displays animated RAMS logo
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { easings } from "@/lib/animations";
import { useLanguage, Language } from "@/lib/i18n";
import { hardwareService } from "@/lib/hardware-service";

// Locale mapping for date/time formatting
const localeMap: Record<Language, string> = {
  ru: "ru-RU",
  kk: "kk-KZ",
  tr: "tr-TR",
  en: "en-US",
};

export interface ScreensaverProps {
  /** Idle timeout in milliseconds (default: 5 minutes) */
  idleTimeout?: number;
  /** Callback when screensaver is dismissed */
  onWake?: () => void;
  /** Manual control - force show/hide */
  forceShow?: boolean;
}

export const Screensaver: React.FC<ScreensaverProps> = ({
  idleTimeout = 5 * 60 * 1000, // 5 minutes
  onWake,
  forceShow,
}) => {
  const [isActive, setIsActive] = React.useState(false);
  const [currentTime, setCurrentTime] = React.useState(new Date());
  const timeoutRef = React.useRef<NodeJS.Timeout | null>(null);
  const { language, t } = useLanguage();

  // Use official RAMS Global logo from web
  const logoSrc = "https://rams-global.com/favicon.svg";
  const locale = localeMap[language];

  // Update time every minute
  React.useEffect(() => {
    const interval = setInterval(() => {
      setCurrentTime(new Date());
    }, 1000);
    return () => clearInterval(interval);
  }, []);

  // Reset idle timer
  const resetTimer = React.useCallback(() => {
    if (timeoutRef.current) {
      clearTimeout(timeoutRef.current);
    }

    // If screensaver is active, wake up
    if (isActive) {
      setIsActive(false);
      onWake?.();
      return;
    }

    // Set new timeout
    timeoutRef.current = setTimeout(() => {
      setIsActive(true);
    }, idleTimeout);
  }, [idleTimeout, isActive, onWake]);

  // Set up event listeners for user activity
  React.useEffect(() => {
    const events = [
      "mousedown",
      "mousemove",
      "keydown",
      "touchstart",
      "wheel",
      "scroll",
    ];

    // Start the initial timer
    resetTimer();

    // Add listeners
    events.forEach((event) => {
      document.addEventListener(event, resetTimer, { passive: true });
    });

    return () => {
      // Cleanup
      if (timeoutRef.current) {
        clearTimeout(timeoutRef.current);
      }
      events.forEach((event) => {
        document.removeEventListener(event, resetTimer);
      });
    };
  }, [resetTimer]);

  // Hardware control when screensaver activates/deactivates
  React.useEffect(() => {
    if (isActive) {
      hardwareService.resetAll();
      hardwareService.setLedMode("RAINBOW"); // Rainbow = Wave + smooth color cycling
    } else {
      hardwareService.setLedMode("WAVE");
    }
  }, [isActive]);

  // Handle forceShow prop
  React.useEffect(() => {
    if (forceShow !== undefined) {
      setIsActive(forceShow);
    }
  }, [forceShow]);

  // Handle click/touch to dismiss
  const handleDismiss = () => {
    setIsActive(false);
    onWake?.();
    // Restart timer
    if (timeoutRef.current) {
      clearTimeout(timeoutRef.current);
    }
    timeoutRef.current = setTimeout(() => {
      setIsActive(true);
    }, idleTimeout);
  };

  // Format time with dynamic locale
  const formattedTime = currentTime.toLocaleTimeString(locale, {
    hour: "2-digit",
    minute: "2-digit",
  });

  const formattedDate = currentTime.toLocaleDateString(locale, {
    weekday: "long",
    day: "numeric",
    month: "long",
  });

  return (
    <AnimatePresence>
      {isActive && (
        <motion.div
          className="fixed inset-0 z-[9999] flex flex-col items-center justify-center bg-black cursor-pointer overflow-hidden"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          exit={{ opacity: 0 }}
          transition={{ duration: 0.5 }}
          onClick={handleDismiss}
          onTouchStart={handleDismiss}
        >
          {/* Animated background gradient */}
          <motion.div
            className="absolute inset-0 pointer-events-none"
            animate={{
              background: [
                "radial-gradient(ellipse 80% 50% at 30% 40%, rgba(212, 175, 55, 0.08) 0%, transparent 60%)",
                "radial-gradient(ellipse 80% 50% at 70% 60%, rgba(212, 175, 55, 0.08) 0%, transparent 60%)",
                "radial-gradient(ellipse 80% 50% at 50% 30%, rgba(212, 175, 55, 0.08) 0%, transparent 60%)",
                "radial-gradient(ellipse 80% 50% at 30% 40%, rgba(212, 175, 55, 0.08) 0%, transparent 60%)",
              ],
            }}
            transition={{
              duration: 15,
              repeat: Infinity,
              ease: "linear",
            }}
          />

          {/* Floating particles */}
          <div className="absolute inset-0 pointer-events-none overflow-hidden">
            {[...Array(15)].map((_, i) => (
              <motion.div
                key={i}
                className="absolute w-1 h-1 bg-primary/20 rounded-full"
                style={{
                  left: `${10 + (i * 6)}%`,
                }}
                initial={{
                  y: "100vh",
                  opacity: 0,
                }}
                animate={{
                  y: "-10vh",
                  opacity: [0, 0.5, 0],
                }}
                transition={{
                  duration: 8 + Math.random() * 4,
                  repeat: Infinity,
                  delay: i * 0.5,
                  ease: "linear",
                }}
              />
            ))}
          </div>

          {/* RAMS Logo - Centered with glow */}
          <motion.div
            className="relative z-10"
            initial={{ scale: 0.8, opacity: 0 }}
            animate={{ scale: 1, opacity: 1 }}
            transition={{
              duration: 0.8,
              ease: easings.smooth,
            }}
          >
            {/* Glow effect */}
            <motion.div
              className="absolute inset-0 blur-3xl pointer-events-none"
              animate={{
                opacity: [0.2, 0.4, 0.2],
              }}
              transition={{
                duration: 4,
                repeat: Infinity,
                ease: "easeInOut",
              }}
            >
              <div className="w-full h-full bg-primary/30 rounded-full scale-150" />
            </motion.div>

            {/* Logo */}
            <motion.img
              src={logoSrc}
              alt="RAMS Global"
              className="relative w-[400px] h-auto"
              animate={{
                scale: [1, 1.03, 1],
              }}
              transition={{
                duration: 4,
                repeat: Infinity,
                ease: "easeInOut",
              }}
            />
          </motion.div>

          {/* Time and Date */}
          <motion.div
            className="absolute bottom-20 text-center"
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.3 }}
          >
            <div className="text-white/80 text-6xl font-light tracking-wider mb-2">
              {formattedTime}
            </div>
            <div className="text-white/40 text-xl capitalize">
              {formattedDate}
            </div>
          </motion.div>

          {/* Touch to wake hint */}
          <motion.div
            className="absolute bottom-8 text-center"
            animate={{
              opacity: [0.2, 0.5, 0.2],
            }}
            transition={{
              duration: 3,
              repeat: Infinity,
              ease: "easeInOut",
            }}
          >
            <span className="text-white/30 text-sm tracking-widest uppercase">
              {t("tapToContinue")}
            </span>
          </motion.div>
        </motion.div>
      )}
    </AnimatePresence>
  );
};
