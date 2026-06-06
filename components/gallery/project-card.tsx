/**
 * Project Card Component
 * Logo-based grid card with hover info effect
 * Supports single or multiple logos
 */

"use client";

import * as React from "react";
import { motion, useMotionValue, useSpring } from "framer-motion";
import { cn } from "@/lib/utils";
import { Project } from "@/lib/types";
import { easings } from "@/lib/animations";
import { getMediaUrl } from "@/lib/media-utils";
import { useLanguage, getLocalizedProject, getLocalizedStatus, getLocalizedClass, isProjectUnderConstruction } from "@/lib/i18n";
import { GalleryCard } from "@/lib/data/gallery-config";

export interface ProjectCardProps {
  project?: Project;
  galleryCard?: GalleryCard;
  projects?: Project[];
  theme?: "light" | "dark";
  onClick?: (projectOrProjects: Project | Project[]) => void;
  index?: number;
  blockNumber?: string; // Номер блока для отображения
}

export const ProjectCard: React.FC<ProjectCardProps> = ({
  project,
  galleryCard,
  projects = [],
  theme = "dark",
  onClick,
  index = 0,
  blockNumber,
}) => {
  const [isHovered, setIsHovered] = React.useState(false);
  const isDark = theme === "dark";
  const { language, t } = useLanguage();

  // Magnetic effect values
  const x = useMotionValue(0);
  const y = useMotionValue(0);
  const springX = useSpring(x, { stiffness: 150, damping: 15 });
  const springY = useSpring(y, { stiffness: 150, damping: 15 });

  const handleMouseMove = (e: React.MouseEvent<HTMLDivElement>) => {
    const rect = e.currentTarget.getBoundingClientRect();
    const centerX = rect.left + rect.width / 2;
    const centerY = rect.top + rect.height / 2;
    const distanceX = e.clientX - centerX;
    const distanceY = e.clientY - centerY;
    
    // Magnetic pull strength
    x.set(distanceX * 0.1);
    y.set(distanceY * 0.1);
  };

  const handleMouseLeave = () => {
    setIsHovered(false);
    x.set(0);
    y.set(0);
  };

  // Support both single project and multi-project cards
  const allProjects = projects.length > 0 ? projects : (project ? [project] : []);
  const mainProject = allProjects[0];
  const isMultiLogo = allProjects.length > 1;

  if (!mainProject) return null;

  const localizedProject = getLocalizedProject(mainProject.id, language);
  const localizedStatus = getLocalizedStatus(mainProject.status, language);
  const localizedClass = getLocalizedClass(mainProject.info.class, language);
  const isBuilding = isProjectUnderConstruction(mainProject.status);
  const logos = allProjects.map(p => getMediaUrl(p.logo || p.image));

  const handleClick = () => {
    if (!onClick) return;

    // Если несколько проектов - передаем массив всех проектов
    if (isMultiLogo) {
      onClick(allProjects);
    } else {
      // Один проект - передаем один проект
      onClick(mainProject);
    }
  };

  return (
    <motion.div
      className="group relative cursor-pointer h-full perspective-1000"
      initial={{ opacity: 0, y: 40, scale: 0.9, filter: "blur(10px)" }}
      animate={{ opacity: 1, y: 0, scale: 1, filter: "blur(0px)" }}
      transition={{
        delay: index * 0.04 + 0.2,
        duration: 0.7,
        ease: easings.smooth,
      }}
      style={{ x: springX, y: springY }}
      whileTap={{ scale: 0.96 }}
      onMouseMove={handleMouseMove}
      onMouseEnter={() => {
        setIsHovered(true);
      }}
      onMouseLeave={handleMouseLeave}
      onClick={handleClick}
    >
      {/* Card Container */}
      <div className={cn(
        "relative w-full h-full overflow-hidden rounded-2xl transition-all duration-500 flex items-center justify-center",
        isDark
          ? "bg-gray-900/40 backdrop-blur-sm border border-white/5 hover:border-primary/40 hover:shadow-[0_0_30px_rgba(200,161,97,0.15)]"
          : "bg-white border border-gray-100 hover:border-primary/50 hover:shadow-[0_20px_40px_rgba(0,0,0,0.08)]"
      )}>
        
        {/* Animated Background Gradient on Hover */}
        <motion.div 
          className={cn(
            "absolute inset-0 opacity-0 group-hover:opacity-100 transition-opacity duration-700 pointer-events-none",
            isDark 
              ? "bg-[radial-gradient(circle_at_var(--mouse-x)_var(--mouse-y),rgba(200,161,97,0.1),transparent_70%)]"
              : "bg-[radial-gradient(circle_at_var(--mouse-x)_var(--mouse-y),rgba(200,161,97,0.05),transparent_70%)]"
          )}
        />

        {/* Logo Container */}
        <div className={cn(
          "flex items-center justify-center w-full h-full z-10",
          isMultiLogo ? "gap-4 p-4" : "p-6"
        )}>
            {isMultiLogo ? (
              <div className={cn(
                "flex items-center justify-center gap-3 w-full h-full",
                allProjects.length === 2 ? "flex-row" : "flex-wrap"
              )}>
                {logos.map((logo, i) => (
                  <motion.img
                    key={i}
                    layoutId={`project-logo-${allProjects[i]?.id}`}
                    src={logo}
                    alt={allProjects[i]?.name || ""}
                    className={cn(
                      "object-contain filter transition-all duration-500",
                      isDark ? "brightness-110 contrast-110" : "",
                      allProjects.length === 2 ? "w-[38%] h-[38%]" : "w-[30%] h-[30%]"
                    )}
                    animate={{
                      scale: isHovered ? 1.1 : 1,
                      y: isHovered ? -4 : 0,
                    }}
                    transition={{ duration: 0.5, ease: easings.smooth, delay: i * 0.05 }}
                  />
                ))}
              </div>
            ) : (
              <motion.img
                layoutId={`project-logo-${mainProject.id}`}
                src={logos[0]}
                alt={mainProject.name}
                className={cn(
                  "w-[60%] h-[45%] object-contain filter transition-all duration-500",
                  isDark ? "brightness-110 contrast-110" : ""
                )}
                animate={{
                  scale: isHovered ? 1.12 : 1,
                  y: isHovered ? -8 : 0,
                }}
                transition={{ duration: 0.6, ease: easings.smooth }}
              />
            )}
        </div>


        {/* Info Overlay */}
        <motion.div
          className={cn(
            "absolute inset-0 flex flex-col justify-end p-5 z-20",
            isDark
              ? "bg-gradient-to-t from-black/95 via-black/40 to-transparent"
              : "bg-gradient-to-t from-gray-900/90 via-gray-900/30 to-transparent"
          )}
          initial={{ opacity: 0, y: 20 }}
          animate={{
            opacity: isHovered ? 1 : 0,
            y: isHovered ? 0 : 20
          }}
          transition={{ duration: 0.4, ease: easings.smooth }}
        >
          {/* Status Badge */}
          <div className="absolute top-4 left-4">
            <motion.span
              className={cn(
                "px-2.5 py-1 rounded-full text-[10px] font-bold uppercase tracking-wider shadow-xl",
                isBuilding
                  ? "bg-amber-500/90 text-white backdrop-blur-md"
                  : "bg-emerald-500/90 text-white backdrop-blur-md"
              )}
              initial={{ x: -10, opacity: 0 }}
              animate={{ x: isHovered ? 0 : -10, opacity: isHovered ? 1 : 0 }}
              transition={{ delay: 0.1 }}
            >
              {isMultiLogo ? (allProjects.length + " " + t("projectsShort")) : localizedStatus}
            </motion.span>
          </div>

          {/* Project Details */}
          <div className="space-y-1">
            <motion.h3 
              className="text-white text-base font-bold tracking-tight leading-tight"
              initial={{ y: 10, opacity: 0 }}
              animate={{ y: isHovered ? 0 : 10, opacity: isHovered ? 1 : 0 }}
              transition={{ delay: 0.15 }}
            >
              {galleryCard?.name || mainProject.name}
            </motion.h3>

            {(galleryCard?.title || localizedProject?.title || mainProject.title || mainProject.subtitle) && (
              <motion.p 
                className="text-white/80 text-xs font-medium leading-tight line-clamp-2"
                initial={{ y: 10, opacity: 0 }}
                animate={{ y: isHovered ? 0 : 10, opacity: isHovered ? 1 : 0 }}
                transition={{ delay: 0.2 }}
              >
                {galleryCard?.title || localizedProject?.title || mainProject.title || mainProject.subtitle}
              </motion.p>
            )}

            {!isMultiLogo && (
              <motion.div 
                className="flex items-center gap-3 pt-2 text-white/70 text-[10px] font-semibold tracking-wide uppercase"
                initial={{ y: 10, opacity: 0 }}
                animate={{ y: isHovered ? 0 : 10, opacity: isHovered ? 1 : 0 }}
                transition={{ delay: 0.25 }}
              >
                <span className="flex items-center gap-1.5">
                  <span className="w-1.5 h-1.5 rounded-full bg-primary shadow-[0_0_8px_rgba(200,161,97,0.8)]" />
                  {localizedClass}
                </span>
                <span className="flex items-center gap-1.5">
                  <span className="w-1.5 h-1.5 rounded-full bg-primary shadow-[0_0_8px_rgba(200,161,97,0.8)]" />
                  {mainProject.info.floors} {t("floorsShort")}
                </span>
              </motion.div>
            )}
          </div>
        </motion.div>

        {/* Interactive Border Highlight */}
        <motion.div
          className="absolute inset-0 rounded-2xl pointer-events-none border-2 border-primary/0"
          animate={{
            borderColor: isHovered ? "rgba(200, 161, 97, 0.4)" : "rgba(200, 161, 97, 0)",
            boxShadow: isHovered ? "inset 0 0 20px rgba(200, 161, 97, 0.1)" : "inset 0 0 0px rgba(200, 161, 97, 0)"
          }}
          transition={{ duration: 0.4 }}
        />
      </div>
    </motion.div>
  );
};
