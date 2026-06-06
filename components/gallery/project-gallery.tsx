/**
 * Project Gallery Component
 * Main menu / Project showcase screen with 15 cards (some multi-logo)
 */

"use client";

import * as React from "react";
import { motion } from "framer-motion";
import { cn } from "@/lib/utils";
import { Project } from "@/lib/types";
import { ProjectCard } from "./project-card";
import { getMediaUrl } from "@/lib/media-utils";
import { LanguageSwitcher } from "@/components/ui/language-switcher";
import { useLanguage } from "@/lib/i18n";
import { GALLERY_CARDS, getProjectsForCard } from "@/lib/data/gallery-config";

import { Icon } from "@/components/ui/icon";
import { useTheme } from "@/lib/theme-context";
import { staggerContainer, iconSpin } from "@/lib/animations";
import { ESP32Client } from "@/lib/esp32-client";

export interface ProjectGalleryProps {
  projects: Project[];
  theme?: "light" | "dark";
  onProjectSelect?: (projectOrProjects: Project | Project[]) => void;
  onAboutCompany?: () => void;
  connectionStatus?: "online" | "offline" | "warning";
  esp32Client?: ESP32Client;
  onOpenControlPanel?: () => void;
}

export const ProjectGallery: React.FC<ProjectGalleryProps> = ({
  projects,
  theme = "dark",
  onProjectSelect,
  onAboutCompany,
  connectionStatus = "online",
  esp32Client,
  onOpenControlPanel,
}) => {
  const isDark = theme === "dark";
  const { toggleTheme } = useTheme();
    const { t } = useLanguage();
    const logoSrc = "/images/brand/rams-logo.png";

    return (
    <motion.div
      className={cn(
        "h-screen w-full relative overflow-hidden flex flex-col",
        isDark ? "bg-background-dark" : "bg-white"
      )}
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      exit={{ opacity: 0 }}
      transition={{ duration: 0.4 }}
    >
      {/* Background Pattern */}
      {!isDark && (
        <div className="absolute inset-0 bg-pattern opacity-10 pointer-events-none" />
      )}

      {/* Content Container */}
      <div className="relative z-10 flex flex-col h-full">
        {/* Header with RAMS Logo - Compact */}
        <motion.header
          className="py-4 px-6 relative z-50 shrink-0"
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.6, ease: "easeOut" }}
        >
          <div className="max-w-7xl mx-auto flex items-center justify-center relative">
            {/* RAMS Logo centered */}
            <motion.img
              src={logoSrc}
              alt="RAMS Global"
              className="h-12 w-auto"
              initial={{ opacity: 0, scale: 0.9 }}
              animate={{ opacity: 1, scale: 1 }}
              transition={{ duration: 0.6, delay: 0.1 }}
            />

            {/* Right side buttons - Icons only */}
            <div className="absolute right-0 top-1/2 -translate-y-1/2 flex items-center gap-2 z-50">
              {/* ESP32 Connection Status */}
              <motion.div
                initial={{ opacity: 0, scale: 0.8 }}
                animate={{ opacity: 1, scale: 1 }}
                transition={{ duration: 0.6, delay: 0.05 }}
                className={cn(
                  "flex items-center gap-2 px-3 py-2 rounded-full backdrop-blur-md shadow-lg",
                  connectionStatus === "online"
                    ? "bg-green-600/20 border border-green-500/30"
                    : "bg-red-600/20 border border-red-500/30"
                )}
                title={connectionStatus === "online" ? "ESP32 подключен" : "ESP32 отключен"}
              >
                <motion.div
                  className={cn(
                    "w-2.5 h-2.5 rounded-full",
                    connectionStatus === "online" ? "bg-green-500" : "bg-red-500"
                  )}
                  animate={
                    connectionStatus === "online"
                      ? { scale: [1, 1.2, 1], opacity: [1, 0.8, 1] }
                      : {}
                  }
                  transition={{ duration: 2, repeat: Infinity }}
                />
                <span className={cn(
                  "text-xs font-medium",
                  connectionStatus === "online" ? "text-green-300" : "text-red-300"
                )}>
                  {connectionStatus === "online" ? "ESP32" : "Offline"}
                </span>
              </motion.div>

              {/* Language Switcher */}
              <motion.div
                initial={{ opacity: 0, scale: 0.8 }}
                animate={{ opacity: 1, scale: 1 }}
                transition={{ duration: 0.6, delay: 0.1 }}
              >
                <LanguageSwitcher theme={theme} />
              </motion.div>

              {/* Control Panel Button */}
              {onOpenControlPanel && (
                <motion.button
                  initial={{ opacity: 0, scale: 0.8 }}
                  animate={{ opacity: 1, scale: 1 }}
                  transition={{ duration: 0.6, delay: 0.15 }}
                  onClick={onOpenControlPanel}
                  className="p-2.5 rounded-full backdrop-blur-md shadow-lg bg-cyan-600/30 border border-cyan-400/40 hover:bg-cyan-500/40 text-white transition-all duration-200"
                  title="Панель управления"
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                >
                  <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24" strokeWidth={2}>
                    <path strokeLinecap="round" strokeLinejoin="round" d="M10.325 4.317c.426-1.756 2.924-1.756 3.35 0a1.724 1.724 0 002.573 1.066c1.543-.94 3.31.826 2.37 2.37a1.724 1.724 0 001.065 2.572c1.756.426 1.756 2.924 0 3.35a1.724 1.724 0 00-1.066 2.573c.94 1.543-.826 3.31-2.37 2.37a1.724 1.724 0 00-2.572 1.065c-.426 1.756-2.924 1.756-3.35 0a1.724 1.724 0 00-2.573-1.066c-1.543.94-3.31-.826-2.37-2.37a1.724 1.724 0 00-1.065-2.572c-1.756-.426-1.756-2.924 0-3.35a1.724 1.724 0 001.066-2.573c-.94-1.543.826-3.31 2.37-2.37.996.608 2.296.07 2.572-1.065z" />
                    <path strokeLinecap="round" strokeLinejoin="round" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" />
                  </svg>
                </motion.button>
              )}
            </div>
          </div>
        </motion.header>

        {/* Projects Grid - 5x3 for 15 cards */}
        <main className="px-4 md:px-8 pb-4 flex-1">
          <motion.div
            className="max-w-[1800px] mx-auto h-full"
            variants={staggerContainer}
            initial="hidden"
            animate="visible"
          >
            <div className="grid grid-cols-5 grid-rows-3 gap-4 h-full">
              {GALLERY_CARDS.map((card, index) => {
                const cardProjects = getProjectsForCard(card);

                // Вычисляем номера блоков для проекта
                let blockNumber = "";
                if (index === 7) {
                  blockNumber = "№15"; // 8-й проект (индекс 7) = блок 15
                } else {
                  const outerBlock = index * 2 + 1;
                  const innerBlock = index * 2 + 2;
                  blockNumber = `№${outerBlock}-${innerBlock}`;
                }

                return (
                  <ProjectCard
                    key={card.id}
                    galleryCard={card}
                    projects={cardProjects as Project[]}
                    theme={theme}
                    onClick={onProjectSelect}
                    index={index}
                    blockNumber={blockNumber}
                  />
                );
              })}
            </div>
          </motion.div>
        </main>
      </div>


      {/* Decorative Elements */}
      <motion.div
        className={cn(
          "fixed top-0 left-0 w-1/3 h-1/3 blur-3xl opacity-10 pointer-events-none",
          isDark
            ? "bg-gradient-to-br from-primary to-transparent"
            : "bg-gradient-to-br from-primary-dark to-transparent"
        )}
        animate={{
          scale: [1, 1.2, 1],
          opacity: [0.1, 0.15, 0.1],
        }}
        transition={{ duration: 8, repeat: Infinity, ease: "easeInOut" }}
      />
      <motion.div
        className={cn(
          "fixed bottom-0 right-0 w-1/3 h-1/3 blur-3xl opacity-10 pointer-events-none",
          isDark
            ? "bg-gradient-to-tl from-primary to-transparent"
            : "bg-gradient-to-tl from-primary-dark to-transparent"
        )}
        animate={{
          scale: [1, 1.2, 1],
          opacity: [0.1, 0.15, 0.1],
        }}
        transition={{ duration: 8, repeat: Infinity, ease: "easeInOut", delay: 4 }}
      />
    </motion.div>
  );
};
