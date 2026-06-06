/**
 * RAMS Interactive Hub - Main Application
 * Premium real estate presentation system
 */

"use client";

import * as React from "react";
import { AnimatePresence, motion } from "framer-motion";
import { Screensaver } from "@/components/screensaver";
import { ProjectGallery } from "@/components/gallery";
import { ProjectHub } from "@/components/hub/project-hub";
import { FullscreenVideo } from "@/components/video-player/fullscreen-video";
import { InfoModal } from "@/components/modals/info-modal";
import { AboutCompany } from "@/components/modals/about-company";
import { AdminModal } from "@/components/modals/admin-modal";
import { ActuatorControl } from "@/components/actuator-control";
import { RAMS_PROJECTS } from "@/lib/data/projects";
import { Project, Scene } from "@/lib/types";
import { useTheme } from "@/lib/theme-context";
import { useProjectSync } from "@/lib/use-project-sync";

type AppScreen = "gallery" | "hub";

export default function Home() {
  const { theme } = useTheme();

  // App State - start directly on gallery
  const [currentScreen, setCurrentScreen] = React.useState<AppScreen>("gallery");
  const [selectedProject, setSelectedProject] = React.useState<Project | null>(null);
  const [selectedProjects, setSelectedProjects] = React.useState<Project[]>([]);
  const [isVideoOpen, setIsVideoOpen] = React.useState(false);
  const [activeVideoScene, setActiveVideoScene] = React.useState<Scene | null>(null);
  const [isInfoModalOpen, setIsInfoModalOpen] = React.useState(false);
  const [isAboutCompanyOpen, setIsAboutCompanyOpen] = React.useState(false);
  const [isAdminOpen, setIsAdminOpen] = React.useState(false);
  const [isActuatorControlOpen, setIsActuatorControlOpen] = React.useState(false);
  const [projects, setProjects] = React.useState<Project[]>(RAMS_PROJECTS);
  const [visibleProjects, setVisibleProjects] = React.useState<Project[]>(RAMS_PROJECTS);

  // Project Sync Hook (актуаторы + LED) - ПОСЛЕ инициализации visibleProjects
  const projectSync = useProjectSync({
    projects: visibleProjects,  // Передаем список проектов для определения индекса
    enableActuators: true,
    enableLED: true,
    animationDuration: 4000,  // 4 сек подъём
    fadeInDuration: 4000,     // 4 сек плавное свечение
  });

  // Load external projects if available
  React.useEffect(() => {
    const loadProjects = async () => {
      if (typeof window !== 'undefined' && (window as any).electron) {
        try {
          const externalProjects = await (window as any).electron.getProjectsData();
          if (externalProjects && externalProjects.length > 0) {
            console.log('[App] Loaded external projects:', externalProjects.length);
            setProjects(externalProjects);
          }
        } catch (error) {
          console.error('[App] Failed to load external projects:', error);
        }
      }
    };

    loadProjects();
  }, []);

  // Filter projects based on visibility settings from localStorage
  const loadVisibilitySettings = React.useCallback(() => {
    const savedVisibility = localStorage.getItem("rams-project-visibility");
    if (savedVisibility) {
      const visibility = JSON.parse(savedVisibility) as Record<string, boolean>;
      const filtered = projects.filter((p) => visibility[p.id] !== false);
      setVisibleProjects(filtered);
    } else {
      setVisibleProjects(projects);
    }
  }, [projects]);

  React.useEffect(() => {
    loadVisibilitySettings();

    // Reload settings when returning from admin page
    const handleFocus = () => loadVisibilitySettings();
    window.addEventListener("focus", handleFocus);
    return () => window.removeEventListener("focus", handleFocus);
  }, [loadVisibilitySettings]);

  // Keyboard shortcuts
  React.useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      // Ctrl+Shift+A - Admin modal
      if (e.ctrlKey && e.shiftKey && e.key.toLowerCase() === "a") {
        e.preventDefault();
        setIsAdminOpen(true);
      }
      // Ctrl+Shift+C - Actuator Control
      if (e.ctrlKey && e.shiftKey && e.key.toLowerCase() === "c") {
        e.preventDefault();
        setIsActuatorControlOpen(true);
      }
    };

    document.addEventListener("keydown", handleKeyDown);

    // Listen for Electron IPC event (in kiosk mode)
    if (typeof window !== 'undefined' && (window as any).electron?.onOpenAdmin) {
      (window as any).electron.onOpenAdmin(() => {
        console.log('[App] Received open-admin from Electron');
        setIsAdminOpen(true);
      });
    }

    return () => document.removeEventListener("keydown", handleKeyDown);
  }, []);

  // Idle screensaver configuration (5 minutes = 300000ms)
  const IDLE_SCREENSAVER_TIMEOUT = 5 * 60 * 1000; // 5 minutes

  // Project selection from gallery - supports both single project and multiple projects
  const handleProjectSelect = (projectOrProjects: Project | Project[]) => {
    const projects = Array.isArray(projectOrProjects) ? projectOrProjects : [projectOrProjects];
    const mainProject = projects[0];

    console.log(`[App] ========== PROJECT SELECT ==========`);
    console.log(`[App] ${projects.length} project(s) selected`);
    projects.forEach(p => console.log(`  - ${p.name} (ID: ${p.id})`));

    // UI навигация — мгновенно, не ждём ESP32
    setSelectedProject(mainProject);
    setSelectedProjects(projects);
    setCurrentScreen("hub");

    // Актуаторы + LED — в фоне, не блокируем UI
    projectSync.activateProject(mainProject);
  };

  // Navigate back to gallery
  const handleBackToGallery = () => {
    // UI навигация — мгновенно
    setCurrentScreen("gallery");
    setSelectedProject(null);
    setSelectedProjects([]);

    // Деактивация — в фоне, не блокируем UI
    projectSync.deactivateProject();
  };

  // Video playback
  const handleVideoPlay = (scene: Scene) => {
    setActiveVideoScene(scene);
    setIsVideoOpen(true);
  };

  const handleVideoClose = () => {
    setIsVideoOpen(false);
    setActiveVideoScene(null);
  };

  // Info modal
  const handleInfoOpen = () => {
    setIsInfoModalOpen(true);
  };

  const handleInfoClose = () => {
    setIsInfoModalOpen(false);
  };

  return (
    <div className="relative w-full min-h-screen overflow-hidden">
      <AnimatePresence mode="wait">
        {/* Gallery / Main Menu */}
        {currentScreen === "gallery" && (
          <motion.div
            key="gallery"
            initial={{ opacity: 0, scale: 1.1, filter: "blur(10px)" }}
            animate={{ opacity: 1, scale: 1, filter: "blur(0px)" }}
            exit={{ opacity: 0, scale: 0.95, filter: "blur(10px)" }}
            transition={{ duration: 0.6, ease: [0.22, 1, 0.36, 1] }}
          >
            <ProjectGallery
              projects={visibleProjects}
              theme={theme}
              onProjectSelect={handleProjectSelect}
              onAboutCompany={() => setIsAboutCompanyOpen(true)}
              connectionStatus={projectSync.isConnected ? "online" : "offline"}
              esp32Client={projectSync.client}
              onOpenControlPanel={() => setIsActuatorControlOpen(true)}
            />

            {/* About Company Modal */}
            <AboutCompany
              isOpen={isAboutCompanyOpen}
              onClose={() => setIsAboutCompanyOpen(false)}
              theme={theme}
            />
          </motion.div>
        )}

        {/* Project Hub */}
        {currentScreen === "hub" && selectedProjects.length > 0 && (
          <motion.div
            key="hub"
            initial={{ opacity: 0, x: 100, scale: 1.05 }}
            animate={{ opacity: 1, x: 0, scale: 1 }}
            exit={{ opacity: 0, x: -100, scale: 0.95 }}
            transition={{ duration: 0.7, ease: [0.22, 1, 0.36, 1] }}
          >
            <ProjectHub
              projects={selectedProjects}
              theme={theme}
              onBack={handleBackToGallery}
              onVideoPlay={handleVideoPlay}
              onInfoOpen={handleInfoOpen}
            />

            {/* Info Modal */}
            <InfoModal
              isOpen={isInfoModalOpen}
              project={selectedProjects[0] || null}
              onClose={handleInfoClose}
              theme={theme}
            />
          </motion.div>
        )}
      </AnimatePresence>

      {/* Video Player Modal - outside AnimatePresence to avoid transform issues */}
      <FullscreenVideo
        isOpen={isVideoOpen}
        videoSrc={activeVideoScene?.video}
        posterSrc={activeVideoScene?.image}
        onClose={handleVideoClose}
      />

      {/* Admin Modal */}
      <AdminModal
        isOpen={isAdminOpen}
        onClose={() => setIsAdminOpen(false)}
        onSave={loadVisibilitySettings}
      />

      {/* Actuator Control Panel */}
      {isActuatorControlOpen && (
        <ActuatorControl onClose={() => setIsActuatorControlOpen(false)} />
      )}

      {/* Idle Screensaver - shows after 5 minutes of inactivity */}
      <Screensaver idleTimeout={IDLE_SCREENSAVER_TIMEOUT} />
    </div>
  );
}
