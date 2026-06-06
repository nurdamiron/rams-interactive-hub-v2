/**
 * Admin Page - Project Visibility Settings
 * Access via Ctrl+Shift+A from main app
 */

"use client";

import * as React from "react";
import { motion } from "framer-motion";
import { RAMS_PROJECTS } from "@/lib/data/projects";
import { useRouter } from "next/navigation";
import { useLanguage, getLocalizedStatus, isProjectUnderConstruction } from "@/lib/i18n";

interface MediaStats {
  projectId: string;
  videos: number;
  photos: number;
  hasLogo: boolean;
  hasMainImage: boolean;
}

export default function AdminPage() {
  const router = useRouter();
  const { language, t } = useLanguage();
  const [visibility, setVisibility] = React.useState<Record<string, boolean>>({});
  const [saved, setSaved] = React.useState(false);
  const [mediaStats, setMediaStats] = React.useState<Record<string, MediaStats>>({});
  const [loadingStats, setLoadingStats] = React.useState(true);

  // Load real media stats from API
  React.useEffect(() => {
    fetch("/api/media-stats")
      .then((res) => res.json())
      .then((data) => {
        if (data.success) {
          const statsMap: Record<string, MediaStats> = {};
          data.stats.forEach((s: MediaStats) => {
            statsMap[s.projectId] = s;
          });
          setMediaStats(statsMap);
        }
      })
      .catch(console.error)
      .finally(() => setLoadingStats(false));
  }, []);

  // Load saved visibility from localStorage
  React.useEffect(() => {
    const saved = localStorage.getItem("rams-project-visibility");
    if (saved) {
      setVisibility(JSON.parse(saved));
    } else {
      // All visible by default
      const defaultVisibility: Record<string, boolean> = {};
      RAMS_PROJECTS.forEach((p) => {
        defaultVisibility[p.id] = true;
      });
      setVisibility(defaultVisibility);
    }
  }, []);

  // Escape key to go back
  React.useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      if (e.key === "Escape") {
        router.push("/");
      }
    };
    document.addEventListener("keydown", handleKeyDown);
    return () => document.removeEventListener("keydown", handleKeyDown);
  }, [router]);

  // Toggle project visibility
  const toggleProject = (id: string) => {
    setVisibility((prev) => ({
      ...prev,
      [id]: !prev[id],
    }));
    setSaved(false);
  };

  // Select all
  const selectAll = () => {
    const newVisibility: Record<string, boolean> = {};
    RAMS_PROJECTS.forEach((p) => {
      newVisibility[p.id] = true;
    });
    setVisibility(newVisibility);
    setSaved(false);
  };

  // Deselect all
  const deselectAll = () => {
    const newVisibility: Record<string, boolean> = {};
    RAMS_PROJECTS.forEach((p) => {
      newVisibility[p.id] = false;
    });
    setVisibility(newVisibility);
    setSaved(false);
  };

  // Select only projects with media (using real file stats)
  const selectWithMedia = () => {
    const newVisibility: Record<string, boolean> = {};
    RAMS_PROJECTS.forEach((p) => {
      const stats = mediaStats[p.id];
      const hasRealMedia = stats ? (stats.videos > 0 || stats.photos > 0) : false;
      newVisibility[p.id] = hasRealMedia;
    });
    setVisibility(newVisibility);
    setSaved(false);
  };

  // Save settings
  const saveSettings = () => {
    localStorage.setItem("rams-project-visibility", JSON.stringify(visibility));
    setSaved(true);
    setTimeout(() => setSaved(false), 2000);
  };

  // Go back to main app
  const goBack = () => {
    router.push("/");
  };

  // Count visible projects
  const visibleCount = Object.values(visibility).filter(Boolean).length;

  // Calculate total media stats from real files
  const totalVideos = Object.values(mediaStats).reduce((sum, s) => sum + s.videos, 0);
  const totalPhotos = Object.values(mediaStats).reduce((sum, s) => sum + s.photos, 0);
  const projectsWithMedia = Object.values(mediaStats).filter(s => s.videos > 0 || s.photos > 0).length;
  const projectsWithLogo = Object.values(mediaStats).filter(s => s.hasLogo).length;

  return (
    <div className="min-h-screen bg-gray-900 text-white p-8">
      {/* Header */}
      <div className="max-w-6xl mx-auto">
        <div className="flex items-center justify-between mb-8">
          <div>
            <h1 className="text-3xl font-bold">{t("projectSettings")}</h1>
            <p className="text-gray-400 mt-1">
              {t("selectProjectsDescription")}
            </p>
          </div>
          <div className="flex items-center gap-4">
            <span className="text-gray-400">
              {visibleCount} {t("of")} {RAMS_PROJECTS.length} {t("projectsCount")}
            </span>
            <button
              onClick={goBack}
              className="px-4 py-2 bg-gray-700 hover:bg-gray-600 rounded-lg transition-colors"
            >
              {t("close")}
            </button>
          </div>
        </div>

        {/* Media Stats Summary */}
        <div className="grid grid-cols-2 md:grid-cols-4 gap-4 mb-6">
          <div className="bg-gray-800 rounded-xl p-4">
            <div className="text-3xl font-bold text-blue-400">{totalVideos}</div>
            <div className="text-sm text-gray-400">{t("videos")}</div>
          </div>
          <div className="bg-gray-800 rounded-xl p-4">
            <div className="text-3xl font-bold text-green-400">{totalPhotos}</div>
            <div className="text-sm text-gray-400">{t("photos")}</div>
          </div>
          <div className="bg-gray-800 rounded-xl p-4">
            <div className="text-3xl font-bold text-purple-400">{projectsWithLogo}</div>
            <div className="text-sm text-gray-400">{t("withLogo")}</div>
          </div>
          <div className="bg-gray-800 rounded-xl p-4">
            <div className="text-3xl font-bold text-primary">{projectsWithMedia}</div>
            <div className="text-sm text-gray-400">{t("withMedia")}</div>
          </div>
        </div>

        {/* Actions */}
        <div className="flex items-center gap-4 mb-6">
          <button
            onClick={selectAll}
            className="px-4 py-2 bg-green-600 hover:bg-green-500 rounded-lg transition-colors"
          >
            {t("selectAll")}
          </button>
          <button
            onClick={deselectAll}
            className="px-4 py-2 bg-red-600 hover:bg-red-500 rounded-lg transition-colors"
          >
            {t("deselectAll")}
          </button>
          <button
            onClick={selectWithMedia}
            className="px-4 py-2 bg-blue-600 hover:bg-blue-500 rounded-lg transition-colors"
          >
            {t("onlyWithMedia")}
          </button>
          <button
            onClick={saveSettings}
            className={`px-6 py-2 rounded-lg transition-colors font-semibold ${
              saved
                ? "bg-green-500 text-white"
                : "bg-primary hover:bg-primary/90 text-white"
            }`}
          >
            {saved ? t("saved") : t("save")}
          </button>
        </div>

        {/* Project Grid */}
        <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-4">
          {RAMS_PROJECTS.map((project, index) => {
            const isVisible = visibility[project.id] !== false;

            // Get real media stats from API
            const stats = mediaStats[project.id];
            const videoCount = stats?.videos || 0;
            const photoCount = stats?.photos || 0;
            const hasLogo = stats?.hasLogo || false;
            const totalMedia = videoCount + photoCount;

            return (
              <motion.div
                key={project.id}
                initial={{ opacity: 0, y: 20 }}
                animate={{ opacity: 1, y: 0 }}
                transition={{ delay: index * 0.02 }}
                onClick={() => toggleProject(project.id)}
                className={`relative p-4 rounded-xl cursor-pointer transition-all border-2 ${
                  isVisible
                    ? "bg-gray-800 border-primary"
                    : "bg-gray-800/50 border-gray-700 opacity-50"
                }`}
              >
                {/* Checkbox */}
                <div
                  className={`absolute top-3 right-3 w-6 h-6 rounded-md flex items-center justify-center ${
                    isVisible ? "bg-primary" : "bg-gray-600"
                  }`}
                >
                  {isVisible && (
                    <svg
                      className="w-4 h-4 text-white"
                      fill="none"
                      viewBox="0 0 24 24"
                      stroke="currentColor"
                    >
                      <path
                        strokeLinecap="round"
                        strokeLinejoin="round"
                        strokeWidth={3}
                        d="M5 13l4 4L19 7"
                      />
                    </svg>
                  )}
                </div>

                {/* Project Number */}
                <div className="text-xs text-gray-500 mb-1">
                  #{index + 1}
                </div>

                {/* Project Name */}
                <h3 className="font-bold text-lg leading-tight">
                  {project.name}
                </h3>
                <p className="text-sm text-gray-400">{project.title}</p>

                {/* Status Badge */}
                <div
                  className={`mt-2 inline-block px-2 py-1 rounded text-xs font-medium ${
                    isProjectUnderConstruction(project.status)
                      ? "bg-yellow-500/20 text-yellow-400"
                      : "bg-green-500/20 text-green-400"
                  }`}
                >
                  {getLocalizedStatus(project.status, language)}
                </div>

                {/* Media Stats */}
                <div className="mt-3 pt-3 border-t border-gray-700">
                  <div className="flex items-center gap-3 text-xs">
                    {/* Video count */}
                    <div className={`flex items-center gap-1 ${videoCount > 0 ? 'text-blue-400' : 'text-gray-600'}`}>
                      <svg className="w-3.5 h-3.5" fill="currentColor" viewBox="0 0 20 20">
                        <path d="M2 6a2 2 0 012-2h6a2 2 0 012 2v8a2 2 0 01-2 2H4a2 2 0 01-2-2V6zM14.553 7.106A1 1 0 0014 8v4a1 1 0 00.553.894l2 1A1 1 0 0018 13V7a1 1 0 00-1.447-.894l-2 1z" />
                      </svg>
                      <span>{videoCount}</span>
                    </div>

                    {/* Photo count */}
                    <div className={`flex items-center gap-1 ${photoCount > 0 ? 'text-green-400' : 'text-gray-600'}`}>
                      <svg className="w-3.5 h-3.5" fill="currentColor" viewBox="0 0 20 20">
                        <path fillRule="evenodd" d="M4 3a2 2 0 00-2 2v10a2 2 0 002 2h12a2 2 0 002-2V5a2 2 0 00-2-2H4zm12 12H4l4-8 3 6 2-4 3 6z" clipRule="evenodd" />
                      </svg>
                      <span>{photoCount}</span>
                    </div>

                    {/* Logo indicator */}
                    <div className={`flex items-center gap-1 ${hasLogo ? 'text-purple-400' : 'text-gray-600'}`} title={hasLogo ? t("hasLogo") : t("noLogo")}>
                      <svg className="w-3.5 h-3.5" fill="currentColor" viewBox="0 0 20 20">
                        <path fillRule="evenodd" d="M4 4a2 2 0 012-2h8a2 2 0 012 2v12a2 2 0 01-2 2H6a2 2 0 01-2-2V4zm3 1h6v4H7V5zm6 6H7v2h6v-2z" clipRule="evenodd" />
                      </svg>
                      {hasLogo ? '✓' : '✗'}
                    </div>
                  </div>

                  {/* Total media badge */}
                  {totalMedia === 0 && (
                    <div className="mt-2 text-xs text-red-400 bg-red-500/10 px-2 py-1 rounded">
                      {t("noMedia")}
                    </div>
                  )}
                </div>
              </motion.div>
            );
          })}
        </div>

        {/* Footer */}
        <div className="mt-8 text-center text-gray-500 text-sm">
          <p>{t("openSettingsShortcut")}</p>
          <p>{t("escapeToReturn")}</p>
        </div>
      </div>
    </div>
  );
}
