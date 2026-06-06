/**
 * Project Hub Component - Split Scroll Layout
 * Left: Scrollable projects info | Right: Fixed gallery
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { cn } from "@/lib/utils";
import { Project, Scene } from "@/lib/types";
import { Button } from "@/components/ui/button";
import { Icon } from "@/components/ui/icon";
import { getMediaUrl } from "@/lib/media-utils";
import { useLanguage, getLocalizedProject, getLocalizedStatus, getLocalizedClass, type Language, type TranslationKey } from "@/lib/i18n";
import { easings } from "@/lib/animations";

export interface ProjectHubProps {
  project?: Project;
  projects?: Project[];
  theme?: "light" | "dark";
  onBack?: () => void;
  onVideoPlay?: (scene: Scene) => void;
  onInfoOpen?: () => void;
}

// Single Project Info Block (for left side scroll)
const ProjectInfoBlock: React.FC<{
  project: Project;
  language: Language;
  t: (key: TranslationKey) => string;
  index: number;
}> = ({ project, language, t, index }) => {
  const localizedProject = getLocalizedProject(project.id, language);
  const localizedStatus = getLocalizedStatus(project.status, language);
  const localizedClass = getLocalizedClass(project.info.class, language);
  const logoSource = getMediaUrl(project.logo || project.image);

  return (
    <motion.div
      className="mb-8 pb-8 border-b border-gray-200 last:border-b-0"
      initial={{ opacity: 0, x: -20 }}
      animate={{ opacity: 1, x: 0 }}
      transition={{ delay: index * 0.1, duration: 0.5 }}
    >
      {/* Logo */}
      <div className="flex items-center justify-center mb-6">
        <div className="w-32 h-32 rounded-2xl bg-white border border-gray-100 flex items-center justify-center p-4 shadow-lg">
          <img
            src={logoSource}
            alt={project.name}
            className="w-full h-full object-contain"
          />
        </div>
      </div>

      {/* Project Name & Title */}
      <div className="space-y-2 mb-4">
        <h2 className="text-2xl font-black text-gray-900 tracking-tight leading-tight text-center">
          {project.name}
        </h2>
        {(localizedProject?.title || project.title) && (
          <p className="text-base text-primary font-bold tracking-tight text-center">
            {localizedProject?.title || project.title}
          </p>
        )}
      </div>

      {/* Status Badge */}
      <div className="flex justify-center mb-4">
        <span className={cn(
          "px-3 py-1 rounded-full text-xs font-black uppercase tracking-widest inline-block shadow-sm",
          project.status === "Сдан" || project.status.includes("Завершен")
            ? "bg-emerald-50 text-emerald-600 border border-emerald-100"
            : "bg-amber-50 text-amber-600 border border-amber-100"
        )}>
          {localizedStatus}
        </span>
      </div>

      {/* Quick Stats Grid */}
      <div className="grid grid-cols-2 gap-3 mb-4">
        {[
          { label: t("class"), value: localizedClass, icon: "stars" },
          { label: t("floors"), value: project.info.floors, icon: "layers" },
          ...(project.info.units > 0 ? [{ label: t("apartments"), value: project.info.units, icon: "apartment" }] : []),
          { label: t("ceilingHeight"), value: project.info.ceilingHeight, icon: "straighten" },
        ].map((stat) => (
          <div
            key={stat.label}
            className="bg-gray-50/80 rounded-xl p-3 border border-gray-100"
          >
            <div className="flex items-center gap-2 mb-1">
              <Icon name={stat.icon} size="sm" className="text-primary/60" />
              <p className="text-[9px] text-gray-400 font-black uppercase tracking-widest">{stat.label}</p>
            </div>
            <p className="text-sm font-bold text-gray-900">{stat.value}</p>
          </div>
        ))}
      </div>

      {/* Description */}
      <p className="text-sm text-gray-500 leading-relaxed font-medium">
        {localizedProject?.description || project.description}
      </p>
    </motion.div>
  );
};

export const ProjectHub: React.FC<ProjectHubProps> = ({
  project,
  projects,
  theme = "dark",
  onBack,
  onVideoPlay,
  onInfoOpen,
}) => {
  const { t, language } = useLanguage();

  // Support both single project and multiple projects
  const allProjects = projects || (project ? [project] : []);
  const isMultiProject = allProjects.length > 1;

  // Collect all scenes from all projects for the gallery
  const allScenes = React.useMemo(() => {
    const scenes: Scene[] = [];
    allProjects.forEach(proj => {
      const photoScenes = proj.scenes.filter(s => !s.video);
      const validScenes = photoScenes.length > 0 ? photoScenes : proj.scenes;
      scenes.push(...validScenes);
    });
    return scenes;
  }, [allProjects]);

  const [activeSceneIndex, setActiveSceneIndex] = React.useState(0);
  const globalLogoSrc = "https://rams-global.com/assets/images/logo.webp";

  const activeScene = allScenes[activeSceneIndex];

  // Check if any project has video
  const videoScene = allProjects.find(p => p.scenes.find(s => s.video))?.scenes.find(s => s.video);
  const hasVideo = !!videoScene;

  // Auto slideshow (5 seconds interval)
  React.useEffect(() => {
    if (allScenes.length <= 1) return;
    const timer = setInterval(() => {
      setActiveSceneIndex((prev) => (prev < allScenes.length - 1 ? prev + 1 : 0));
    }, 5000);
    return () => clearInterval(timer);
  }, [allScenes.length]);

  // Hardware управляется через ProjectSync в page.tsx — здесь не дублируем

  const imageSource = activeScene ? getMediaUrl(activeScene.image || allProjects[0].heroImage || allProjects[0].image) : "";

  const goToPrevScene = () => {
    setActiveSceneIndex((prev) => (prev > 0 ? prev - 1 : allScenes.length - 1));
  };

  const goToNextScene = () => {
    setActiveSceneIndex((prev) => (prev < allScenes.length - 1 ? prev + 1 : 0));
  };

  return (
    <motion.div
      className="h-screen w-full flex flex-col bg-white overflow-hidden"
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      exit={{ opacity: 0 }}
      transition={{ duration: 0.6, ease: easings.smooth }}
    >
      {/* Header with Back Button - Fixed */}
      <motion.div
        className="h-16 flex items-center px-6 border-b border-gray-100 shrink-0 bg-white z-50"
        initial={{ y: -20, opacity: 0 }}
        animate={{ y: 0, opacity: 1 }}
        transition={{ duration: 0.4, delay: 0.2 }}
      >
        <Button
          variant="ghost"
          size="icon"
          onClick={onBack}
          className="w-10 h-10 rounded-full hover:bg-gray-100 transition-all hover:scale-110 active:scale-90"
        >
          <Icon name="arrow_back" className="text-gray-600" />
        </Button>
        <span className="ml-3 text-gray-400 text-xs font-semibold uppercase tracking-widest">{t("backToProjects")}</span>

        <div className="flex justify-end items-center gap-4 flex-1">
          <img src={globalLogoSrc} alt="RAMS" className="h-8 w-auto grayscale opacity-70 hover:opacity-100 transition-opacity" />
        </div>
      </motion.div>

      {/* Main Content - Split */}
      <div className="flex-1 flex overflow-hidden">
        {/* LEFT SIDE - Scrollable Projects Info */}
        <div className="w-1/2 border-r border-gray-200 bg-white overflow-y-auto p-8">
          {allProjects.map((proj, index) => (
            <ProjectInfoBlock
              key={proj.id}
              project={proj}
              language={language}
              t={t}
              index={index}
            />
          ))}
        </div>

        {/* RIGHT SIDE - Fixed Gallery with swipe support */}
        <div className="w-1/2 flex flex-col relative bg-gray-100">
          <div
            className="flex-1 relative overflow-hidden"
            onPointerDown={(e) => {
              if (allScenes.length <= 1) return;
              const startX = e.clientX;
              const onPointerUp = (upEvent: PointerEvent) => {
                const diff = upEvent.clientX - startX;
                if (Math.abs(diff) > 50) {
                  if (diff < 0) goToNextScene();
                  else goToPrevScene();
                }
                window.removeEventListener('pointerup', onPointerUp);
              };
              window.addEventListener('pointerup', onPointerUp);
            }}
            style={{ touchAction: 'pan-y', cursor: allScenes.length > 1 ? 'grab' : 'default' }}
          >
            <AnimatePresence mode="wait">
              <motion.div
                key={activeScene?.id || 'default'}
                className="absolute inset-0"
                initial={{ opacity: 0, scale: 1.1, filter: "blur(20px)" }}
                animate={{ opacity: 1, scale: 1, filter: "blur(0px)" }}
                exit={{ opacity: 0, scale: 0.9, filter: "blur(20px)" }}
                transition={{ duration: 0.8, ease: easings.smooth }}
              >
                <div
                  className="w-full h-full bg-cover bg-center bg-no-repeat transition-transform duration-[10000ms] ease-linear transform hover:scale-110"
                  style={{ backgroundImage: `url('${imageSource}')` }}
                />
                <div className="absolute inset-0 bg-gradient-to-b from-black/20 via-transparent to-black/40 pointer-events-none" />
              </motion.div>
            </AnimatePresence>

            {/* Video Button */}
            {hasVideo && (
              <div className="absolute bottom-10 left-10 flex flex-col items-start gap-3">
                {/* Play Video Fullscreen Button */}
                <button
                  onClick={() => {
                    if (onVideoPlay && videoScene) {
                      onVideoPlay(videoScene);
                    }
                  }}
                  className="px-10 py-5 rounded-full font-black text-xl flex items-center gap-4 group overflow-hidden transition-all bg-primary text-white shadow-[0_20px_50px_rgba(200,161,97,0.4)] hover:scale-105 active:scale-95"
                >
                  <span className="relative flex items-center gap-4">
                    <Icon name="play_circle" className="!text-3xl" />
                    <span className="uppercase tracking-widest">
                      {t("video")}
                    </span>
                  </span>
                </button>

              </div>
            )}

            {/* Navigation Arrows */}
            {allScenes.length > 1 && (
              <>
                <button
                  onClick={goToPrevScene}
                  className="absolute left-6 top-1/2 -translate-y-1/2 w-16 h-16 rounded-full bg-white/10 backdrop-blur-md border border-white/20 flex items-center justify-center text-white hover:scale-110 hover:bg-white/20 transition-all"
                >
                  <Icon name="chevron_left" className="!text-4xl" />
                </button>
                <button
                  onClick={goToNextScene}
                  className="absolute right-6 top-1/2 -translate-y-1/2 w-16 h-16 rounded-full bg-white/10 backdrop-blur-md border border-white/20 flex items-center justify-center text-white hover:scale-110 hover:bg-white/20 transition-all"
                >
                  <Icon name="chevron_right" className="!text-4xl" />
                </button>
              </>
            )}

            {/* Navigation Dots */}
            <div className="absolute bottom-12 left-1/2 -translate-x-1/2 flex items-center gap-4 z-30">
              {allScenes.map((_, idx) => (
                <button
                  key={idx}
                  onClick={() => setActiveSceneIndex(idx)}
                  className={cn(
                    "w-3 h-3 rounded-full transition-all duration-500 border border-white/20",
                    idx === activeSceneIndex
                      ? "bg-primary w-12 shadow-[0_0_15px_rgba(200,161,97,0.8)]"
                      : "bg-white/40 hover:bg-white/60"
                  )}
                />
              ))}
            </div>
          </div>
        </div>
      </div>
    </motion.div>
  );
};
