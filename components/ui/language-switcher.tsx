"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { useLanguage, Language, languageNames, languageFlags } from "@/lib/i18n";
import { cn } from "@/lib/utils";

interface LanguageSwitcherProps {
  theme?: "light" | "dark";
  className?: string;
}

export const LanguageSwitcher: React.FC<LanguageSwitcherProps> = ({
  theme = "dark",
  className,
}) => {
  const { language, setLanguage } = useLanguage();
  const [isOpen, setIsOpen] = React.useState(false);
  const containerRef = React.useRef<HTMLDivElement>(null);
  const isDark = theme === "dark";

  const languages: Language[] = ["ru", "kk", "tr"];

  // Close on click outside
  React.useEffect(() => {
    const handleClickOutside = (e: MouseEvent) => {
      if (containerRef.current && !containerRef.current.contains(e.target as Node)) {
        setIsOpen(false);
      }
    };

    if (isOpen) {
      document.addEventListener("mousedown", handleClickOutside);
    }
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, [isOpen]);

  return (
    <div ref={containerRef} className={cn("relative z-50", className)}>
      {/* Current Language Button - Icon only */}
      <motion.button
        onClick={() => setIsOpen(!isOpen)}
        className={cn(
          "w-11 h-11 rounded-full flex items-center justify-center transition-all shadow-lg",
          isDark
            ? "bg-white/10 backdrop-blur-md border border-white/20 hover:bg-white/20 text-white"
            : "bg-gray-900 border border-gray-800 hover:bg-gray-800 text-white"
        )}
        whileHover={{ scale: 1.1 }}
        whileTap={{ scale: 0.95 }}
      >
        <span className="text-xl">{languageFlags[language]}</span>
      </motion.button>

      {/* Dropdown */}
      <AnimatePresence>
        {isOpen && (
          <motion.div
            className={cn(
              "absolute top-full mt-2 right-0 overflow-hidden rounded-2xl shadow-2xl",
              isDark
                ? "bg-gray-900/95 backdrop-blur-xl border border-white/10"
                : "bg-white border border-gray-200"
            )}
            initial={{ opacity: 0, y: -10, scale: 0.95 }}
            animate={{ opacity: 1, y: 0, scale: 1 }}
            exit={{ opacity: 0, y: -10, scale: 0.95 }}
            transition={{ duration: 0.15 }}
          >
            {languages.map((lang) => (
              <motion.button
                key={lang}
                onClick={() => {
                  setLanguage(lang);
                  setIsOpen(false);
                }}
                className={cn(
                  "flex items-center gap-3 w-full px-5 py-3 text-left transition-colors",
                  language === lang
                    ? isDark
                      ? "bg-primary/20 text-primary"
                      : "bg-primary/10 text-primary-dark"
                    : isDark
                      ? "text-white hover:bg-white/10"
                      : "text-gray-900 hover:bg-gray-100"
                )}
                whileHover={{ x: 4 }}
              >
                <span className="text-xl">{languageFlags[lang]}</span>
                <span className="font-medium">{languageNames[lang]}</span>
                {language === lang && (
                  <svg className="w-4 h-4 ml-auto" fill="currentColor" viewBox="0 0 20 20">
                    <path fillRule="evenodd" d="M16.707 5.293a1 1 0 010 1.414l-8 8a1 1 0 01-1.414 0l-4-4a1 1 0 011.414-1.414L8 12.586l7.293-7.293a1 1 0 011.414 0z" clipRule="evenodd" />
                  </svg>
                )}
              </motion.button>
            ))}
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
};
