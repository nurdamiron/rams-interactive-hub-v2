/**
 * Admin Modal - Project Visibility + Diagnostics
 * Access via Ctrl+Shift+A
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { RAMS_PROJECTS } from "@/lib/data/projects";
import { GALLERY_CARDS } from "@/lib/data/gallery-config";
import { Icon } from "@/components/ui/icon";
import { Button } from "@/components/ui/button";
import { useLanguage, getLocalizedStatus, isProjectUnderConstruction } from "@/lib/i18n";
import { hardwareService } from "@/lib/hardware-service";

interface MediaStats {
  projectId: string;
  videos: number;
  photos: number;
  hasLogo: boolean;
  hasMainImage: boolean;
}

interface DiagnosticCandidate {
  label: string;
  path: string;
  projectsExists: boolean;
  sampleFiles: string[];
}

interface DiagnosticInfo {
  platform: string;
  isPackaged: boolean;
  appPath: string;
  exePath: string;
  exeDir: string;
  resourcesPath: string;
  mediaRoot: string;
  mediaRootExists: boolean;
  candidates: DiagnosticCandidate[];
}

interface AdminModalProps {
  isOpen: boolean;
  onClose: () => void;
  onSave?: () => void;
}

export const AdminModal: React.FC<AdminModalProps> = ({
  isOpen,
  onClose,
  onSave,
}) => {
  const [visibility, setVisibility] = React.useState<Record<string, boolean>>({});
  const [saved, setSaved] = React.useState(false);
  const [mediaStats, setMediaStats] = React.useState<Record<string, MediaStats>>({});
  const [loadingStats, setLoadingStats] = React.useState(true);
  const [activeTab, setActiveTab] = React.useState<"projects" | "diagnostics" | "hardware">("projects");
  const [diagnostics, setDiagnostics] = React.useState<DiagnosticInfo | null>(null);
  const [hwIP, setHwIP] = React.useState("");
  const [hwConnected, setHwConnected] = React.useState(false);
  const [hwPinging, setHwPinging] = React.useState(false);
  const [hwSavedIP, setHwSavedIP] = React.useState(false);
  const [hwLedMode, setHwLedMode] = React.useState("RAINBOW");
  const [hwLedAutoCycle, setHwLedAutoCycle] = React.useState(false);
  const [hwBlockMapping, setHwBlockMapping] = React.useState<Record<string, number>>({});
  const [hwMappingSaved, setHwMappingSaved] = React.useState(false);
  // TV state
  const [tvIP, setTvIP] = React.useState("");
  const [tvConnected, setTvConnected] = React.useState(false);
  const [tvSavedIP, setTvSavedIP] = React.useState(false);
  const [tvConnecting, setTvConnecting] = React.useState(false);
  const { t, language } = useLanguage();

  const isElectron = typeof window !== "undefined" && (window as any).electron?.isElectron;

  // Load media stats (via Electron IPC or fallback)
  React.useEffect(() => {
    if (!isOpen) return;
    setLoadingStats(true);

    const loadStats = async () => {
      try {
        if (isElectron && (window as any).electron?.getMediaStats) {
          const stats = await (window as any).electron.getMediaStats();
          const statsMap: Record<string, MediaStats> = {};
          (stats || []).forEach((s: MediaStats) => { statsMap[s.projectId] = s; });
          setMediaStats(statsMap);
        }
      } catch (e) {
        console.error("Failed to load media stats:", e);
      } finally {
        setLoadingStats(false);
      }
    };

    loadStats();
  }, [isOpen, isElectron]);

  // Load diagnostics
  React.useEffect(() => {
    if (!isOpen || activeTab !== "diagnostics") return;

    const loadDiag = async () => {
      try {
        if (isElectron && (window as any).electron?.getDiagnostics) {
          const diag = await (window as any).electron.getDiagnostics();
          setDiagnostics(diag);
        } else {
          setDiagnostics({
            platform: "browser",
            isPackaged: false,
            appPath: window.location.href,
            exePath: "-",
            exeDir: "-",
            resourcesPath: "-",
            mediaRoot: "/public",
            mediaRootExists: true,
            candidates: [],
          });
        }
      } catch (e) {
        console.error("Failed to load diagnostics:", e);
      }
    };

    loadDiag();
  }, [isOpen, activeTab, isElectron]);

  // Load hardware status + block mapping + TV status
  React.useEffect(() => {
    if (!isOpen || activeTab !== "hardware") return;

    const loadHwStatus = async () => {
      const status = await hardwareService.getStatus();
      setHwIP(status.ip || "192.168.1.100");
      setHwConnected(status.connected);
    };

    const loadTvStatus = async () => {
      const status = await hardwareService.tvGetStatus();
      setTvIP(status.ip || "");
      setTvConnected(status.connected);
    };

    const loadMapping = async () => {
      const mapping = await hardwareService.getBlockMapping();
      setHwBlockMapping(mapping);
    };

    loadHwStatus();
    loadTvStatus();
    loadMapping();
    const interval = setInterval(() => {
      loadHwStatus();
      loadTvStatus();
    }, 3000);
    return () => clearInterval(interval);
  }, [isOpen, activeTab]);

  // Load saved visibility from localStorage
  React.useEffect(() => {
    if (!isOpen) return;
    const savedVisibility = localStorage.getItem("rams-project-visibility");
    if (savedVisibility) {
      setVisibility(JSON.parse(savedVisibility));
    } else {
      const defaultVisibility: Record<string, boolean> = {};
      RAMS_PROJECTS.forEach((p) => { defaultVisibility[p.id] = true; });
      setVisibility(defaultVisibility);
    }
  }, [isOpen]);

  // Escape key to close
  React.useEffect(() => {
    if (!isOpen) return;
    const handleKeyDown = (e: KeyboardEvent) => { if (e.key === "Escape") onClose(); };
    document.addEventListener("keydown", handleKeyDown);
    return () => document.removeEventListener("keydown", handleKeyDown);
  }, [isOpen, onClose]);

  const toggleProject = (id: string) => {
    setVisibility((prev) => ({ ...prev, [id]: !prev[id] }));
    setSaved(false);
  };

  const selectAll = () => {
    const v: Record<string, boolean> = {};
    RAMS_PROJECTS.forEach((p) => { v[p.id] = true; });
    setVisibility(v);
    setSaved(false);
  };

  const deselectAll = () => {
    const v: Record<string, boolean> = {};
    RAMS_PROJECTS.forEach((p) => { v[p.id] = false; });
    setVisibility(v);
    setSaved(false);
  };

  const selectWithMedia = () => {
    const v: Record<string, boolean> = {};
    RAMS_PROJECTS.forEach((p) => {
      const stats = mediaStats[p.id];
      v[p.id] = stats ? (stats.videos > 0 || stats.photos > 0) : false;
    });
    setVisibility(v);
    setSaved(false);
  };

  const saveSettings = () => {
    localStorage.setItem("rams-project-visibility", JSON.stringify(visibility));
    setSaved(true);
    onSave?.();
    setTimeout(() => setSaved(false), 2000);
  };

  const visibleCount = Object.values(visibility).filter(Boolean).length;
  const totalVideos = Object.values(mediaStats).reduce((sum, s) => sum + s.videos, 0);
  const totalPhotos = Object.values(mediaStats).reduce((sum, s) => sum + s.photos, 0);
  const projectsWithMedia = Object.values(mediaStats).filter(s => s.videos > 0 || s.photos > 0).length;
  const projectsWithLogo = Object.values(mediaStats).filter(s => s.hasLogo).length;

  return (
    <AnimatePresence>
      {isOpen && (
        <>
          <motion.div
            className="fixed inset-0 z-[100] bg-black/80 backdrop-blur-md"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            onClick={onClose}
          />

          <motion.div
            className="fixed inset-4 z-[101] bg-gray-900 rounded-3xl overflow-hidden flex flex-col shadow-2xl"
            initial={{ opacity: 0, scale: 0.95 }}
            animate={{ opacity: 1, scale: 1 }}
            exit={{ opacity: 0, scale: 0.95 }}
            transition={{ type: "spring", damping: 25, stiffness: 300 }}
          >
            {/* Header */}
            <div className="flex items-center justify-between p-6 border-b border-gray-800">
              <div className="flex items-center gap-4">
                <h1 className="text-2xl font-bold text-white">{t("projectSettings")}</h1>
                {/* Tabs */}
                <div className="flex bg-gray-800 rounded-lg p-1">
                  <button
                    onClick={() => setActiveTab("projects")}
                    className={`px-4 py-1.5 rounded-md text-sm font-medium transition-colors ${
                      activeTab === "projects" ? "bg-primary text-white" : "text-gray-400 hover:text-white"
                    }`}
                  >
                    Проекты
                  </button>
                  <button
                    onClick={() => setActiveTab("hardware")}
                    className={`px-4 py-1.5 rounded-md text-sm font-medium transition-colors ${
                      activeTab === "hardware" ? "bg-primary text-white" : "text-gray-400 hover:text-white"
                    }`}
                  >
                    {t("hardware")}
                  </button>
                  <button
                    onClick={() => setActiveTab("diagnostics")}
                    className={`px-4 py-1.5 rounded-md text-sm font-medium transition-colors ${
                      activeTab === "diagnostics" ? "bg-primary text-white" : "text-gray-400 hover:text-white"
                    }`}
                  >
                    Диагностика
                  </button>
                </div>
              </div>
              <Button
                variant="ghost"
                size="icon"
                onClick={onClose}
                className="w-12 h-12 rounded-full bg-white/5 hover:bg-white/10"
              >
                <Icon name="close" className="text-white" />
              </Button>
            </div>

            {activeTab === "projects" ? (
              <>
                {/* Media Stats Summary */}
                <div className="grid grid-cols-4 gap-3 p-6 pb-0">
                  <div className="bg-gray-800 rounded-xl p-3 text-center">
                    <div className="text-2xl font-bold text-blue-400">{totalVideos}</div>
                    <div className="text-xs text-gray-400">{t("videos")}</div>
                  </div>
                  <div className="bg-gray-800 rounded-xl p-3 text-center">
                    <div className="text-2xl font-bold text-green-400">{totalPhotos}</div>
                    <div className="text-xs text-gray-400">{t("photos")}</div>
                  </div>
                  <div className="bg-gray-800 rounded-xl p-3 text-center">
                    <div className="text-2xl font-bold text-purple-400">{projectsWithLogo}</div>
                    <div className="text-xs text-gray-400">{t("withLogo")}</div>
                  </div>
                  <div className="bg-gray-800 rounded-xl p-3 text-center">
                    <div className="text-2xl font-bold text-primary">{projectsWithMedia}</div>
                    <div className="text-xs text-gray-400">{t("withMedia")}</div>
                  </div>
                </div>

                {/* Actions */}
                <div className="flex items-center gap-3 p-6">
                  <button onClick={selectAll} className="px-4 py-2 bg-green-600 hover:bg-green-500 rounded-lg transition-colors text-white text-sm font-medium">
                    {t("selectAll")}
                  </button>
                  <button onClick={deselectAll} className="px-4 py-2 bg-red-600 hover:bg-red-500 rounded-lg transition-colors text-white text-sm font-medium">
                    {t("deselectAll")}
                  </button>
                  <button onClick={selectWithMedia} className="px-4 py-2 bg-blue-600 hover:bg-blue-500 rounded-lg transition-colors text-white text-sm font-medium">
                    {t("onlyWithMedia")}
                  </button>
                  <div className="flex-1" />
                  <span className="text-gray-400 text-sm">{visibleCount} / {RAMS_PROJECTS.length}</span>
                  <button
                    onClick={saveSettings}
                    className={`px-6 py-2 rounded-lg transition-colors font-semibold ${
                      saved ? "bg-green-500 text-white" : "bg-primary hover:bg-primary/90 text-white"
                    }`}
                  >
                    {saved ? t("saved") : t("save")}
                  </button>
                </div>

                {/* Project Grid */}
                <div className="flex-1 overflow-y-auto p-6 pt-0">
                  <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 gap-3">
                    {RAMS_PROJECTS.map((project, index) => {
                      const isVisible = visibility[project.id] !== false;
                      const stats = mediaStats[project.id];
                      const videoCount = stats?.videos || 0;
                      const photoCount = stats?.photos || 0;
                      const hasLogo = stats?.hasLogo || false;
                      const totalMedia = videoCount + photoCount;

                      return (
                        <motion.div
                          key={project.id}
                          initial={{ opacity: 0, y: 10 }}
                          animate={{ opacity: 1, y: 0 }}
                          transition={{ delay: index * 0.01 }}
                          onClick={() => toggleProject(project.id)}
                          className={`relative p-3 rounded-xl cursor-pointer transition-all border-2 ${
                            isVisible ? "bg-gray-800 border-primary" : "bg-gray-800/50 border-gray-700 opacity-50"
                          }`}
                        >
                          <div className={`absolute top-2 right-2 w-5 h-5 rounded flex items-center justify-center ${isVisible ? "bg-primary" : "bg-gray-600"}`}>
                            {isVisible && (
                              <svg className="w-3 h-3 text-white" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={3} d="M5 13l4 4L19 7" />
                              </svg>
                            )}
                          </div>
                          <div className="text-[10px] text-gray-500 mb-0.5">#{index + 1}</div>
                          <h3 className="font-bold text-sm leading-tight text-white pr-6">{project.name}</h3>
                          <p className="text-xs text-gray-400 truncate">{project.title}</p>
                          <div className={`mt-1.5 inline-block px-1.5 py-0.5 rounded text-[10px] font-medium ${
                            isProjectUnderConstruction(project.status) ? "bg-yellow-500/20 text-yellow-400" : "bg-green-500/20 text-green-400"
                          }`}>
                            {getLocalizedStatus(project.status, language)}
                          </div>
                          <div className="mt-2 pt-2 border-t border-gray-700">
                            <div className="flex items-center gap-2 text-[10px]">
                              <div className={`flex items-center gap-0.5 ${videoCount > 0 ? 'text-blue-400' : 'text-gray-600'}`}>
                                <Icon name="videocam" size="sm" /><span>{videoCount}</span>
                              </div>
                              <div className={`flex items-center gap-0.5 ${photoCount > 0 ? 'text-green-400' : 'text-gray-600'}`}>
                                <Icon name="photo" size="sm" /><span>{photoCount}</span>
                              </div>
                              <div className={`${hasLogo ? 'text-purple-400' : 'text-gray-600'}`}>
                                {hasLogo ? '✓' : '✗'}
                              </div>
                            </div>
                            {totalMedia === 0 && (
                              <div className="mt-1 text-[10px] text-red-400">{t("noMedia")}</div>
                            )}
                          </div>
                        </motion.div>
                      );
                    })}
                  </div>
                </div>
              </>
            ) : activeTab === "hardware" ? (
              /* Hardware Tab */
              <div className="flex-1 overflow-y-auto p-6">
                <div className="max-w-2xl mx-auto space-y-6">
                  {/* Connection Status */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <h3 className="text-lg font-bold text-white mb-4">{t("connectionStatus")}</h3>
                    <div className="flex items-center gap-3 mb-4">
                      <div className={`w-4 h-4 rounded-full ${hwConnected ? "bg-green-500 shadow-[0_0_10px_rgba(34,197,94,0.5)]" : "bg-red-500 shadow-[0_0_10px_rgba(239,68,68,0.5)]"}`} />
                      <span className={`text-sm font-semibold ${hwConnected ? "text-green-400" : "text-red-400"}`}>
                        {hwConnected ? t("connected") : t("disconnected")}
                      </span>
                    </div>
                  </div>

                  {/* LG TV Connection */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <h3 className="text-lg font-bold text-white mb-4">{t("tvSettings")}</h3>
                    <div className="flex items-center gap-3 mb-4">
                      <div className={`w-4 h-4 rounded-full ${tvConnected ? "bg-green-500 shadow-[0_0_10px_rgba(34,197,94,0.5)]" : "bg-red-500 shadow-[0_0_10px_rgba(239,68,68,0.5)]"}`} />
                      <span className={`text-sm font-semibold ${tvConnected ? "text-green-400" : "text-red-400"}`}>
                        LG TV: {tvConnected ? t("connected") : t("disconnected")}
                      </span>
                    </div>
                    <div className="flex gap-3 mb-3">
                      <input
                        type="text"
                        value={tvIP}
                        onChange={(e) => { setTvIP(e.target.value); setTvSavedIP(false); }}
                        className="flex-1 bg-gray-700 text-white px-4 py-2.5 rounded-lg border border-gray-600 focus:border-primary focus:outline-none font-mono text-sm"
                        placeholder="192.168.1.200"
                      />
                      <button
                        onClick={async () => {
                          await hardwareService.tvSetIP(tvIP);
                          setTvSavedIP(true);
                          setTimeout(() => setTvSavedIP(false), 2000);
                        }}
                        className={`px-5 py-2.5 rounded-lg font-semibold text-sm transition-colors ${
                          tvSavedIP ? "bg-green-500 text-white" : "bg-primary hover:bg-primary/90 text-white"
                        }`}
                      >
                        {tvSavedIP ? t("saved") : t("save")}
                      </button>
                    </div>
                    <div className="flex gap-3">
                      <button
                        onClick={async () => {
                          setTvConnecting(true);
                          await hardwareService.tvConnect();
                          setTimeout(async () => {
                            const status = await hardwareService.tvGetStatus();
                            setTvConnected(status.connected);
                            setTvConnecting(false);
                          }, 3000);
                        }}
                        disabled={tvConnecting || !tvIP}
                        className="px-6 py-2.5 bg-blue-600 hover:bg-blue-500 disabled:bg-blue-800 disabled:opacity-50 rounded-lg text-white font-semibold text-sm transition-colors"
                      >
                        {tvConnecting ? "..." : t("tvConnect")}
                      </button>
                      {tvConnected && (
                        <button
                          onClick={() => hardwareService.tvDisconnect()}
                          className="px-6 py-2.5 bg-gray-700 hover:bg-gray-600 rounded-lg text-white font-semibold text-sm transition-colors"
                        >
                          {t("tvDisconnect")}
                        </button>
                      )}
                    </div>
                    <p className="text-gray-500 text-xs mt-3">{t("tvHint")}</p>
                  </div>

                  {/* IP Configuration */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <h3 className="text-lg font-bold text-white mb-4">{t("espIpAddress")}</h3>
                    <div className="flex gap-3">
                      <input
                        type="text"
                        value={hwIP}
                        onChange={(e) => { setHwIP(e.target.value); setHwSavedIP(false); }}
                        className="flex-1 bg-gray-700 text-white px-4 py-2.5 rounded-lg border border-gray-600 focus:border-primary focus:outline-none font-mono text-sm"
                        placeholder="192.168.1.100"
                      />
                      <button
                        onClick={async () => {
                          await hardwareService.setIP(hwIP);
                          setHwSavedIP(true);
                          setTimeout(() => setHwSavedIP(false), 2000);
                        }}
                        className={`px-5 py-2.5 rounded-lg font-semibold text-sm transition-colors ${
                          hwSavedIP ? "bg-green-500 text-white" : "bg-primary hover:bg-primary/90 text-white"
                        }`}
                      >
                        {hwSavedIP ? t("saved") : t("save")}
                      </button>
                    </div>
                  </div>

                  {/* Ping */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <div className="flex items-center gap-3">
                      <button
                        onClick={async () => {
                          setHwPinging(true);
                          await hardwareService.ping();
                          // Wait a moment for the PONG to arrive
                          setTimeout(async () => {
                            const status = await hardwareService.getStatus();
                            setHwConnected(status.connected);
                            setHwPinging(false);
                          }, 1500);
                        }}
                        disabled={hwPinging}
                        className="px-6 py-2.5 bg-blue-600 hover:bg-blue-500 disabled:bg-blue-800 rounded-lg text-white font-semibold text-sm transition-colors"
                      >
                        {hwPinging ? "..." : t("ping")}
                      </button>
                      <span className="text-gray-400 text-sm">
                        UDP → {hwIP}:4210
                      </span>
                    </div>
                  </div>

                  {/* Emergency Stop */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <button
                      onClick={() => hardwareService.emergencyStop()}
                      className="w-full px-6 py-4 bg-red-600 hover:bg-red-500 active:bg-red-700 rounded-xl text-white font-black text-lg uppercase tracking-wider transition-colors shadow-[0_0_20px_rgba(239,68,68,0.3)]"
                    >
                      {t("emergencyStop")}
                    </button>
                  </div>

                  {/* LED Mode */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <h3 className="text-lg font-bold text-white mb-4">{t("ledMode")}</h3>
                    <div className="grid grid-cols-3 gap-2">
                      {/* AUTO button — prominent at the top */}
                      <button
                        onClick={async () => {
                          const newState = !hwLedAutoCycle;
                          setHwLedAutoCycle(newState);
                          if (newState) {
                            setHwLedMode("AUTO");
                            await hardwareService.setLedMode("AUTO");
                          } else {
                            // Stop auto-cycle by sending AUTO again (toggle)
                            await hardwareService.setLedMode("AUTO");
                          }
                        }}
                        className={`col-span-3 px-3 py-3 rounded-lg text-sm font-bold transition-colors flex items-center justify-center gap-2 ${
                          hwLedAutoCycle
                            ? "bg-green-600 text-white shadow-[0_0_15px_rgba(34,197,94,0.4)]"
                            : "bg-gradient-to-r from-cyan-600 to-blue-600 text-white hover:from-cyan-500 hover:to-blue-500"
                        }`}
                      >
                        <span style={hwLedAutoCycle ? { display: "inline-block", animation: "spin 3s linear infinite" } : undefined}>&#x21bb;</span>
                        {hwLedAutoCycle ? "AUTO ON (каждые 60с)" : "AUTO — смена эффектов"}
                      </button>

                      {["RAINBOW", "WAVE", "PULSE", "SPARKLE", "FIRE", "OFF"].map((mode) => (
                        <button
                          key={mode}
                          onClick={async () => {
                            setHwLedMode(mode);
                            setHwLedAutoCycle(false);
                            await hardwareService.setLedMode(mode);
                          }}
                          className={`px-3 py-2.5 rounded-lg text-sm font-semibold transition-colors ${
                            hwLedMode === mode && !hwLedAutoCycle
                              ? "bg-primary text-white"
                              : "bg-gray-700 text-gray-300 hover:bg-gray-600"
                          }`}
                        >
                          {mode}
                        </button>
                      ))}
                    </div>
                  </div>

                  {/* Block Mapping */}
                  <div className="bg-gray-800 rounded-xl p-5">
                    <div className="flex items-center justify-between mb-4">
                      <h3 className="text-lg font-bold text-white">{t("blockMapping")}</h3>
                      <div className="flex gap-2">
                        <button
                          onClick={() => {
                            setHwBlockMapping({});
                            setHwMappingSaved(false);
                          }}
                          className="px-3 py-1.5 bg-gray-700 hover:bg-gray-600 rounded-lg text-gray-300 text-xs font-medium transition-colors"
                        >
                          {t("resetToDefault")}
                        </button>
                        <button
                          onClick={async () => {
                            await hardwareService.setBlockMapping(hwBlockMapping);
                            setHwMappingSaved(true);
                            setTimeout(() => setHwMappingSaved(false), 2000);
                          }}
                          className={`px-4 py-1.5 rounded-lg font-semibold text-xs transition-colors ${
                            hwMappingSaved ? "bg-green-500 text-white" : "bg-primary hover:bg-primary/90 text-white"
                          }`}
                        >
                          {hwMappingSaved ? t("saved") : t("save")}
                        </button>
                      </div>
                    </div>
                    <p className="text-gray-400 text-xs mb-4">{t("blockMappingHint")}</p>
                    <div className="space-y-2 max-h-[400px] overflow-y-auto pr-2">
                      {GALLERY_CARDS.map((card) => {
                        const customBlock = hwBlockMapping[card.id];
                        const effectiveBlock = customBlock !== undefined ? customBlock : card.blockNumber;
                        const isOverridden = customBlock !== undefined;
                        return (
                          <div
                            key={card.id}
                            className={`flex items-center gap-3 p-3 rounded-lg border transition-colors ${
                              isOverridden ? "bg-primary/10 border-primary/30" : "bg-gray-700/50 border-gray-700"
                            }`}
                          >
                            <span className="text-gray-500 text-xs font-mono w-6 text-right">#{card.id}</span>
                            <span className="flex-1 text-white text-sm font-medium truncate">
                              {card.name}{card.title ? ` ${card.title}` : ""}
                            </span>
                            <span className="text-gray-500 text-xs">{t("block")}:</span>
                            <input
                              type="number"
                              min={1}
                              max={15}
                              value={effectiveBlock}
                              onChange={(e) => {
                                const val = parseInt(e.target.value);
                                if (val >= 1 && val <= 15) {
                                  setHwBlockMapping((prev) => ({ ...prev, [card.id]: val }));
                                  setHwMappingSaved(false);
                                }
                              }}
                              className="w-16 bg-gray-900 text-white text-center px-2 py-1.5 rounded-lg border border-gray-600 focus:border-primary focus:outline-none font-mono text-sm"
                            />
                            <button
                              onClick={async () => {
                                await hardwareService.selectProject(card.projectIds[0]);
                              }}
                              className="px-2 py-1.5 bg-blue-600 hover:bg-blue-500 rounded-lg text-white text-xs font-medium transition-colors"
                              title="Test UP"
                            >
                              TEST
                            </button>
                          </div>
                        );
                      })}
                    </div>
                  </div>
                </div>
              </div>
            ) : (
              /* Diagnostics Tab */
              <div className="flex-1 overflow-y-auto p-6">
                {diagnostics ? (
                  <div className="space-y-6">
                    {/* System Info */}
                    <div>
                      <h3 className="text-lg font-bold text-white mb-3">Системная информация</h3>
                      <div className="bg-gray-800 rounded-xl p-4 font-mono text-sm space-y-2">
                        <div className="flex justify-between">
                          <span className="text-gray-400">Platform:</span>
                          <span className="text-white">{diagnostics.platform}</span>
                        </div>
                        <div className="flex justify-between">
                          <span className="text-gray-400">Packaged:</span>
                          <span className={diagnostics.isPackaged ? "text-green-400" : "text-yellow-400"}>
                            {diagnostics.isPackaged ? "Да (production)" : "Нет (development)"}
                          </span>
                        </div>
                        <div className="flex justify-between">
                          <span className="text-gray-400">Electron:</span>
                          <span className={isElectron ? "text-green-400" : "text-yellow-400"}>
                            {isElectron ? "Да" : "Нет (браузер)"}
                          </span>
                        </div>
                      </div>
                    </div>

                    {/* Paths */}
                    <div>
                      <h3 className="text-lg font-bold text-white mb-3">Пути</h3>
                      <div className="bg-gray-800 rounded-xl p-4 font-mono text-xs space-y-3">
                        <div>
                          <div className="text-gray-500 mb-1">App Path:</div>
                          <div className="text-white break-all">{diagnostics.appPath}</div>
                        </div>
                        <div>
                          <div className="text-gray-500 mb-1">Exe Path:</div>
                          <div className="text-white break-all">{diagnostics.exePath}</div>
                        </div>
                        <div>
                          <div className="text-gray-500 mb-1">Resources Path:</div>
                          <div className="text-white break-all">{diagnostics.resourcesPath}</div>
                        </div>
                        <div>
                          <div className="text-gray-500 mb-1">Media Root (активный):</div>
                          <div className={`break-all font-bold ${diagnostics.mediaRootExists ? "text-green-400" : "text-red-400"}`}>
                            {diagnostics.mediaRoot}
                            {diagnostics.mediaRootExists ? " ✓" : " ✗ НЕ НАЙДЕН"}
                          </div>
                        </div>
                      </div>
                    </div>

                    {/* Candidates */}
                    {diagnostics.candidates.length > 0 && (
                      <div>
                        <h3 className="text-lg font-bold text-white mb-3">Поиск медиа (candidates)</h3>
                        <div className="space-y-2">
                          {diagnostics.candidates.map((c, i) => (
                            <div key={i} className={`bg-gray-800 rounded-xl p-4 border-2 ${
                              c.projectsExists ? "border-green-500/50" : "border-red-500/30"
                            }`}>
                              <div className="flex items-center justify-between mb-2">
                                <span className="text-white font-bold">{c.label}</span>
                                <span className={`text-sm font-bold ${c.projectsExists ? "text-green-400" : "text-red-400"}`}>
                                  {c.projectsExists ? "✓ НАЙДЕН" : "✗ НЕТ"}
                                </span>
                              </div>
                              <div className="text-xs text-gray-400 font-mono break-all">{c.path}/projects/</div>
                              {c.sampleFiles.length > 0 && (
                                <div className="mt-2 text-xs text-gray-500">
                                  Папки: {c.sampleFiles.join(", ")}
                                </div>
                              )}
                            </div>
                          ))}
                        </div>
                      </div>
                    )}

                    {/* Instructions */}
                    <div>
                      <h3 className="text-lg font-bold text-white mb-3">Инструкция</h3>
                      <div className="bg-blue-900/30 border border-blue-500/30 rounded-xl p-4 text-sm text-blue-200">
                        <p className="mb-2">Медиа файлы должны быть в одной из этих папок:</p>
                        <ul className="list-disc list-inside space-y-1 font-mono text-xs">
                          <li>[exe папка]/media/projects/</li>
                          <li>[exe папка]/../media/projects/</li>
                          <li>resources/media/projects/</li>
                        </ul>
                        <p className="mt-3 text-xs text-blue-300">
                          Структура: media/projects/[id]/images/scenes/01.jpg
                        </p>
                      </div>
                    </div>
                  </div>
                ) : (
                  <div className="flex items-center justify-center h-full text-gray-400">
                    Загрузка диагностики...
                  </div>
                )}
              </div>
            )}

            {/* Footer */}
            <div className="p-4 border-t border-gray-800 text-center text-gray-500 text-xs">
              {t("adminShortcut")}
            </div>
          </motion.div>
        </>
      )}
    </AnimatePresence>
  );
};
