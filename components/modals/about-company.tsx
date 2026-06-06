/**
 * About Company Modal Component
 * Информация о RAMS Global - Enhanced Design
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { cn } from "@/lib/utils";
import { Button } from "@/components/ui/button";
import { Icon } from "@/components/ui/icon";
import { useLanguage } from "@/lib/i18n";

export interface AboutCompanyProps {
  isOpen: boolean;
  onClose: () => void;
  theme?: "light" | "dark";
}

export const AboutCompany: React.FC<AboutCompanyProps> = ({
  isOpen,
  onClose,
  theme = "dark",
}) => {
  const isDark = theme === "dark";
  const { t } = useLanguage();

  const stats = [
    { label: t("foundedYear"), value: "1988", icon: "event" },
    { label: t("countriesPresence"), value: "6", icon: "public" },
    { label: t("cities"), value: "11", icon: "location_city" },
    { label: t("employees"), value: "25,000+", icon: "group" },
    { label: t("completedProjects"), value: "100+", icon: "apartment" },
  ];

  const values = [
    { title: t("trust"), icon: "handshake", description: t("trustDesc") },
    { title: t("conscience"), icon: "favorite", description: t("conscienceDesc") },
    { title: t("justice"), icon: "balance", description: t("justiceDesc") },
    { title: t("socialBenefit"), icon: "volunteer_activism", description: t("socialBenefitDesc") },
    { title: t("sustainability"), icon: "eco", description: t("sustainabilityDesc") },
  ];


  return (
    <AnimatePresence>
      {isOpen && (
        <>
          {/* Backdrop */}
          <motion.div
            className="fixed inset-0 z-40 bg-black/70 backdrop-blur-md"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            onClick={onClose}
          />

          {/* Modal */}
          <motion.div
            className={cn(
              "fixed inset-4 md:inset-8 z-50 mx-auto max-w-5xl rounded-3xl",
              "shadow-2xl overflow-hidden flex flex-col",
              isDark ? "bg-gray-900" : "bg-white"
            )}
            initial={{ opacity: 0, scale: 0.9, y: 50 }}
            animate={{ opacity: 1, scale: 1, y: 0 }}
            exit={{ opacity: 0, scale: 0.9, y: 50 }}
            transition={{ type: "spring", stiffness: 300, damping: 30 }}
          >
            {/* Header with gradient */}
            <div className="relative overflow-hidden">
              <div className="absolute inset-0 bg-gradient-to-r from-primary/20 via-primary/10 to-transparent" />
              <motion.div
                className={cn(
                  "relative flex items-center justify-between p-6 border-b",
                  isDark ? "border-gray-800" : "border-gray-200"
                )}
                initial={{ y: -20, opacity: 0 }}
                animate={{ y: 0, opacity: 1 }}
                transition={{ delay: 0.1 }}
              >
                <div className="flex items-center gap-4">
                  <motion.div
                    className="w-16 h-16 rounded-2xl bg-gradient-to-br from-primary to-yellow-600 flex items-center justify-center shadow-lg"
                    whileHover={{ scale: 1.05, rotate: 5 }}
                  >
                    <Icon name="business" size="xl" className="text-white" />
                  </motion.div>
                  <div>
                    <h2 className={cn(
                      "text-2xl font-bold tracking-wider uppercase",
                      isDark ? "text-white" : "text-gray-900"
                    )}>
                      RAMS GLOBAL
                    </h2>
                    <p className={cn(
                      "text-sm",
                      isDark ? "text-gray-400" : "text-gray-500"
                    )}>
                      {t("companySlogan")}
                    </p>
                  </div>
                </div>
                <motion.button
                  onClick={onClose}
                  className={cn(
                    "w-12 h-12 rounded-full flex items-center justify-center transition-all",
                    isDark
                      ? "hover:bg-gray-800 text-white"
                      : "hover:bg-gray-100 text-gray-900"
                  )}
                  whileHover={{ scale: 1.05 }}
                  whileTap={{ scale: 0.95 }}
                >
                  <Icon name="close" size="lg" />
                </motion.button>
              </motion.div>
            </div>

            {/* Scrollable Content */}
            <div className="flex-1 overflow-y-auto p-8 space-y-10">
              {/* Mission & Vision */}
              <motion.section
                initial={{ y: 30, opacity: 0 }}
                animate={{ y: 0, opacity: 1 }}
                transition={{ delay: 0.2 }}
              >
                <h3 className={cn(
                  "text-xl font-bold tracking-wide mb-4 flex items-center gap-3",
                  isDark ? "text-white" : "text-gray-900"
                )}>
                  <div className="w-10 h-10 rounded-xl bg-primary flex items-center justify-center">
                    <Icon name="lightbulb" className="text-white" />
                  </div>
                  {t("missionAndVision")}
                </h3>
                <p className={cn(
                  "text-base leading-relaxed pl-13",
                  isDark ? "text-gray-300" : "text-gray-600"
                )}>
                  {t("missionText")}
                </p>
              </motion.section>

              {/* History */}
              <motion.section
                initial={{ y: 30, opacity: 0 }}
                animate={{ y: 0, opacity: 1 }}
                transition={{ delay: 0.3 }}
              >
                <h3 className={cn(
                  "text-xl font-bold tracking-wide mb-4 flex items-center gap-3",
                  isDark ? "text-white" : "text-gray-900"
                )}>
                  <div className="w-10 h-10 rounded-xl bg-primary flex items-center justify-center">
                    <Icon name="history" className="text-white" />
                  </div>
                  {t("history")}
                </h3>
                <p className={cn(
                  "text-base leading-relaxed pl-13",
                  isDark ? "text-gray-300" : "text-gray-600"
                )}>
                  {t("historyText")}
                </p>
              </motion.section>

              {/* Stats Grid */}
              <motion.section
                initial={{ y: 30, opacity: 0 }}
                animate={{ y: 0, opacity: 1 }}
                transition={{ delay: 0.4 }}
              >
                <h3 className={cn(
                  "text-xl font-bold tracking-wide mb-6 flex items-center gap-3",
                  isDark ? "text-white" : "text-gray-900"
                )}>
                  <div className="w-10 h-10 rounded-xl bg-primary flex items-center justify-center">
                    <Icon name="analytics" className="text-white" />
                  </div>
                  {t("keyMetrics")}
                </h3>
                <div className="grid grid-cols-2 md:grid-cols-5 gap-4">
                  {stats.map((stat, index) => (
                    <motion.div
                      key={stat.label}
                      className={cn(
                        "relative p-5 rounded-2xl text-center overflow-hidden group cursor-pointer border",
                        isDark ? "bg-gray-800/50 border-gray-700 hover:border-primary/50" : "bg-gray-50 border-gray-200 hover:border-primary"
                      )}
                      initial={{ opacity: 0, y: 20, scale: 0.9 }}
                      animate={{ opacity: 1, y: 0, scale: 1 }}
                      transition={{ delay: 0.5 + index * 0.1 }}
                      whileHover={{ scale: 1.05, y: -5 }}
                    >
                      <div className="absolute inset-0 bg-primary opacity-0 group-hover:opacity-5 transition-opacity" />
                      <div className="w-12 h-12 mx-auto mb-3 rounded-xl bg-primary/10 flex items-center justify-center">
                        <Icon name={stat.icon} className="text-primary" />
                      </div>
                      <motion.div
                        className={cn(
                          "text-3xl font-bold mb-1",
                          isDark ? "text-white" : "text-gray-900"
                        )}
                        initial={{ scale: 0 }}
                        animate={{ scale: 1 }}
                        transition={{ delay: 0.7 + index * 0.1, type: "spring" }}
                      >
                        {stat.value}
                      </motion.div>
                      <div className={cn(
                        "text-xs font-medium",
                        isDark ? "text-gray-400" : "text-gray-500"
                      )}>
                        {stat.label}
                      </div>
                    </motion.div>
                  ))}
                </div>
              </motion.section>

              {/* Core Values */}
              <motion.section
                initial={{ y: 30, opacity: 0 }}
                animate={{ y: 0, opacity: 1 }}
                transition={{ delay: 0.6 }}
              >
                <h3 className={cn(
                  "text-xl font-bold tracking-wide mb-6 flex items-center gap-3",
                  isDark ? "text-white" : "text-gray-900"
                )}>
                  <div className="w-10 h-10 rounded-xl bg-primary flex items-center justify-center">
                    <Icon name="stars" className="text-white" />
                  </div>
                  {t("ourValues")}
                </h3>
                <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
                  {values.map((value, index) => (
                    <motion.div
                      key={value.title}
                      className={cn(
                        "p-5 rounded-2xl border group cursor-pointer",
                        isDark
                          ? "bg-gray-800/30 border-gray-700 hover:border-primary/50 hover:bg-gray-800/50"
                          : "bg-gray-50 border-gray-200 hover:border-primary hover:bg-gray-100"
                      )}
                      initial={{ opacity: 0, scale: 0.9 }}
                      animate={{ opacity: 1, scale: 1 }}
                      transition={{ delay: 0.7 + index * 0.1 }}
                      whileHover={{ scale: 1.02, y: -3 }}
                    >
                      <Icon
                        name={value.icon}
                        size="lg"
                        className={cn(
                          "mb-3 transition-colors",
                          isDark
                            ? "text-gray-500 group-hover:text-primary"
                            : "text-gray-400 group-hover:text-primary"
                        )}
                      />
                      <h4 className={cn(
                        "font-bold text-lg mb-2",
                        isDark ? "text-white" : "text-gray-900"
                      )}>
                        {value.title}
                      </h4>
                      <p className={cn(
                        "text-sm",
                        isDark ? "text-gray-400" : "text-gray-500"
                      )}>
                        {value.description}
                      </p>
                    </motion.div>
                  ))}
                </div>
              </motion.section>

            </div>

            {/* Footer */}
            <motion.div
              className={cn(
                "p-6 border-t flex items-center justify-between",
                isDark ? "border-gray-800 bg-gray-900/50" : "border-gray-200 bg-gray-50"
              )}
              initial={{ y: 20, opacity: 0 }}
              animate={{ y: 0, opacity: 1 }}
              transition={{ delay: 0.5 }}
            >
              <div className={cn(
                "text-sm",
                isDark ? "text-gray-500" : "text-gray-400"
              )}>
                © 1988-2026 RAMS Global. {t("allRightsReserved")}
              </div>
              <motion.div whileHover={{ scale: 1.05 }} whileTap={{ scale: 0.95 }}>
                <Button variant="primary" size="md" onClick={onClose}>
                  {t("close")}
                </Button>
              </motion.div>
            </motion.div>
          </motion.div>
        </>
      )}
    </AnimatePresence>
  );
};
