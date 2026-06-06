/**
 * Fullscreen Video Player Component
 * Custom controls, loading states, keyboard shortcuts, proper cleanup
 */

"use client";

import * as React from "react";
import { motion, AnimatePresence } from "framer-motion";
import { cn } from "@/lib/utils";
import { Icon } from "@/components/ui/icon";
import { getMediaUrl } from "@/lib/media-utils";

export interface FullscreenVideoProps {
  isOpen: boolean;
  videoSrc?: string;
  posterSrc?: string;
  onClose: () => void;
}

function formatTime(seconds: number): string {
  if (!isFinite(seconds) || seconds < 0) return "0:00";
  const m = Math.floor(seconds / 60);
  const s = Math.floor(seconds % 60);
  return `${m}:${s.toString().padStart(2, "0")}`;
}

export const FullscreenVideo: React.FC<FullscreenVideoProps> = ({
  isOpen,
  videoSrc,
  posterSrc,
  onClose,
}) => {
  const videoRef = React.useRef<HTMLVideoElement>(null);
  const progressRef = React.useRef<HTMLDivElement>(null);
  const controlsTimerRef = React.useRef<ReturnType<typeof setTimeout>>(undefined);
  const containerRef = React.useRef<HTMLDivElement>(null);

  const [isPlaying, setIsPlaying] = React.useState(false);
  const [isLoading, setIsLoading] = React.useState(false);
  const loadingTimerRef = React.useRef<ReturnType<typeof setTimeout>>(undefined);
  const [hasError, setHasError] = React.useState(false);
  const [errorMessage, setErrorMessage] = React.useState("");
  const [currentTime, setCurrentTime] = React.useState(0);
  const [duration, setDuration] = React.useState(0);
  const [buffered, setBuffered] = React.useState(0);
  const [volume, setVolume] = React.useState(1);
  const [isMuted, setIsMuted] = React.useState(false);
  const [showControls, setShowControls] = React.useState(true);
  const [isSeeking, setIsSeeking] = React.useState(false);
  const decodeRetryCount = React.useRef(0);
  const MAX_DECODE_RETRIES = 2;

  const resolvedVideoSrc = React.useMemo(() => getMediaUrl(videoSrc), [videoSrc]);
  const resolvedPosterSrc = React.useMemo(() => getMediaUrl(posterSrc), [posterSrc]);

  // Debug: log video source
  React.useEffect(() => {
    if (isOpen) {
      console.log("[VideoPlayer] Opening with src:", videoSrc, "→ resolved:", resolvedVideoSrc);
    }
  }, [isOpen, videoSrc, resolvedVideoSrc]);

  // Reset state when opening/closing
  React.useEffect(() => {
    if (isOpen) {
      setIsLoading(false);
      setHasError(false);
      setErrorMessage("");
      setCurrentTime(0);
      setDuration(0);
      setBuffered(0);
      setIsPlaying(false);
      setShowControls(true);
      decodeRetryCount.current = 0;
      clearTimeout(loadingTimerRef.current);
      // Autoplay is handled by handleCanPlay — no need for a separate setTimeout
    } else {
      const video = videoRef.current;
      if (video) {
        video.pause();
      }
      setIsPlaying(false);
      setIsLoading(false);
      clearTimeout(loadingTimerRef.current);
    }
  }, [isOpen]);

  // Recover playback when window regains visibility (kiosk mode)
  React.useEffect(() => {
    if (!isOpen) return;
    const handleVisibility = () => {
      if (document.visibilityState === "visible") {
        const video = videoRef.current;
        if (video && video.paused && isPlaying) {
          video.play().catch(() => {});
        }
      }
    };
    document.addEventListener("visibilitychange", handleVisibility);
    return () => document.removeEventListener("visibilitychange", handleVisibility);
  }, [isOpen, isPlaying]);

  // Auto-hide controls after 3 seconds of no mouse movement
  const resetControlsTimer = React.useCallback(() => {
    setShowControls(true);
    if (controlsTimerRef.current) clearTimeout(controlsTimerRef.current);
    controlsTimerRef.current = setTimeout(() => {
      if (videoRef.current && !videoRef.current.paused) {
        setShowControls(false);
      }
    }, 3000);
  }, []);

  // Keyboard shortcuts
  React.useEffect(() => {
    if (!isOpen) return;

    const handleKeyDown = (e: KeyboardEvent) => {
      const video = videoRef.current;
      if (!video) return;

      switch (e.key) {
        case "Escape":
          e.preventDefault();
          onClose();
          break;
        case " ":
          e.preventDefault();
          if (video.paused) video.play();
          else video.pause();
          resetControlsTimer();
          break;
        case "ArrowLeft":
          e.preventDefault();
          video.currentTime = Math.max(0, video.currentTime - 10);
          resetControlsTimer();
          break;
        case "ArrowRight":
          e.preventDefault();
          video.currentTime = Math.min(video.duration, video.currentTime + 10);
          resetControlsTimer();
          break;
        case "ArrowUp":
          e.preventDefault();
          video.volume = Math.min(1, video.volume + 0.1);
          setVolume(video.volume);
          resetControlsTimer();
          break;
        case "ArrowDown":
          e.preventDefault();
          video.volume = Math.max(0, video.volume - 0.1);
          setVolume(video.volume);
          resetControlsTimer();
          break;
        case "m":
        case "M":
          e.preventDefault();
          video.muted = !video.muted;
          setIsMuted(video.muted);
          resetControlsTimer();
          break;
      }
    };

    document.addEventListener("keydown", handleKeyDown);
    return () => document.removeEventListener("keydown", handleKeyDown);
  }, [isOpen, onClose, resetControlsTimer]);

  // Video event handlers
  const handleLoadedMetadata = () => {
    const video = videoRef.current;
    if (!video) return;
    setDuration(video.duration);
    setIsLoading(false);
  };

  const handleCanPlay = () => {
    setIsLoading(false);
    const video = videoRef.current;
    // Only autoplay on initial open (currentTime near 0), not after every seek
    if (video && video.paused && isOpen && video.currentTime < 0.5) {
      video.play().catch(() => {});
    }
  };

  const handlePlay = () => setIsPlaying(true);
  const handlePause = () => setIsPlaying(false);

  const handleTimeUpdate = () => {
    const video = videoRef.current;
    if (!video || isSeeking) return;
    setCurrentTime(video.currentTime);
  };

  const handleProgress = () => {
    const video = videoRef.current;
    if (!video || !video.buffered.length) return;
    const end = video.buffered.end(video.buffered.length - 1);
    setBuffered(end);
  };

  const handleWaiting = () => {
    clearTimeout(loadingTimerRef.current);
    loadingTimerRef.current = setTimeout(() => setIsLoading(true), 2000);
  };
  const handlePlaying = () => {
    clearTimeout(loadingTimerRef.current);
    setIsLoading(false);
    setIsPlaying(true);
    decodeRetryCount.current = 0;
  };

  const handleError = () => {
    const video = videoRef.current;
    const error = video?.error;
    console.error("[VideoPlayer] Error:", error?.code, error?.message, "src:", video?.src);

    // Ignore transient decode errors during seeking — just retry playback
    if (error?.code === 3 && isSeeking) {
      console.log("[VideoPlayer] Ignoring decode error during seek");
      return;
    }

    // For decode errors (code 3), try to recover by reloading (max 2 retries)
    if (error?.code === 3 && video && decodeRetryCount.current < MAX_DECODE_RETRIES) {
      decodeRetryCount.current++;
      console.log(`[VideoPlayer] Decode error — recovery attempt ${decodeRetryCount.current}/${MAX_DECODE_RETRIES}`);
      const savedTime = video.currentTime;
      video.load();
      video.addEventListener('loadedmetadata', () => {
        video.currentTime = Math.max(0, savedTime - 1);
        video.play().catch(() => {});
      }, { once: true });
      return;
    }

    setIsLoading(false);
    setHasError(true);
    if (error) {
      const messages: Record<number, string> = {
        1: "Воспроизведение прервано",
        2: "Ошибка сети",
        4: "Видео не найдено или формат не поддерживается",
      };
      setErrorMessage(messages[error.code] || "Неизвестная ошибка");
    } else {
      setErrorMessage("Не удалось загрузить видео");
    }
  };

  const handleEnded = () => {
    setIsPlaying(false);
    setShowControls(true);
  };

  // Progress bar click & drag (unified via pointerdown)
  const handleProgressPointerDown = (e: React.PointerEvent<HTMLDivElement>) => {
    e.preventDefault();
    e.stopPropagation();
    const video = videoRef.current;
    const bar = progressRef.current;
    if (!video || !bar || !duration) return;

    setIsSeeking(true);
    const wasPaused = video.paused;
    if (!wasPaused) video.pause();

    const rect = bar.getBoundingClientRect();

    // Immediately seek to click position
    const initX = Math.max(0, Math.min(1, (e.clientX - rect.left) / rect.width));
    video.currentTime = initX * duration;
    setCurrentTime(initX * duration);

    const onMove = (moveEvent: PointerEvent) => {
      const newRect = bar.getBoundingClientRect();
      const x = Math.max(0, Math.min(1, (moveEvent.clientX - newRect.left) / newRect.width));
      video.currentTime = x * duration;
      setCurrentTime(x * duration);
    };

    const onUp = () => {
      setIsSeeking(false);
      if (!wasPaused) video.play();
      window.removeEventListener("pointermove", onMove);
      window.removeEventListener("pointerup", onUp);
    };

    window.addEventListener("pointermove", onMove);
    window.addEventListener("pointerup", onUp);
    resetControlsTimer();
  };

  // Play/Pause toggle
  const togglePlay = () => {
    const video = videoRef.current;
    if (!video) return;
    if (video.paused) video.play().catch(() => {});
    else video.pause();
    resetControlsTimer();
  };

  // Volume toggle
  const toggleMute = () => {
    const video = videoRef.current;
    if (!video) return;
    video.muted = !video.muted;
    setIsMuted(video.muted);
    resetControlsTimer();
  };

  // Retry on error
  const handleRetry = () => {
    const video = videoRef.current;
    if (!video) return;
    setHasError(false);
    setIsLoading(true);
    setErrorMessage("");
    video.load();
  };

  const progress = duration > 0 ? (currentTime / duration) * 100 : 0;
  const bufferedProgress = duration > 0 ? (buffered / duration) * 100 : 0;

  return (
    <AnimatePresence>
      {isOpen && (
        <motion.div
          ref={containerRef}
          className="fixed inset-0 z-50 bg-black select-none"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          exit={{ opacity: 0 }}
          transition={{ duration: 0.3 }}
          onMouseMove={resetControlsTimer}
          onClick={(e) => {
            // Click on backdrop (not controls) toggles play
            if (e.target === e.currentTarget || (e.target as HTMLElement).tagName === "VIDEO") {
              togglePlay();
            }
          }}
        >
          {/* Video Element */}
          {resolvedVideoSrc && !hasError ? (
            <video
              ref={videoRef}
              className="w-full h-full object-contain"
              src={resolvedVideoSrc}
              poster={resolvedPosterSrc}
              preload="metadata"
              playsInline
              onLoadedMetadata={handleLoadedMetadata}
              onCanPlay={handleCanPlay}
              onPlay={handlePlay}
              onPause={handlePause}
              onTimeUpdate={handleTimeUpdate}
              onProgress={handleProgress}
              onWaiting={handleWaiting}
              onPlaying={handlePlaying}
              onError={handleError}
              onEnded={handleEnded}
            />
          ) : !hasError ? (
            // Poster fallback when no video source
            <motion.div
              className="w-full h-full bg-cover bg-center"
              style={{ backgroundImage: `url('${resolvedPosterSrc}')` }}
              animate={{ scale: [1, 1.02, 1] }}
              transition={{ duration: 20, repeat: Infinity }}
            />
          ) : null}

          {/* Loading Spinner — only shows after 2s delay for slow network/large files */}
          <AnimatePresence>
            {isLoading && !hasError && (
              <motion.div
                className="absolute inset-0 flex items-center justify-center pointer-events-none z-10"
                initial={{ opacity: 0 }}
                animate={{ opacity: 1 }}
                exit={{ opacity: 0 }}
              >
                <div className="w-12 h-12 border-3 border-white/20 border-t-white rounded-full animate-spin" />
              </motion.div>
            )}
          </AnimatePresence>

          {/* Error State */}
          {hasError && (
            <div className="absolute inset-0 flex items-center justify-center z-10">
              <div className="flex flex-col items-center gap-6 text-center px-8">
                <div className="w-20 h-20 rounded-full bg-red-500/20 flex items-center justify-center">
                  <Icon name="error" className="!text-5xl text-red-400" />
                </div>
                <div>
                  <p className="text-white text-xl font-bold mb-2">Ошибка воспроизведения</p>
                  <p className="text-white/50 text-sm">{errorMessage}</p>
                </div>
                <button
                  onClick={handleRetry}
                  className="px-6 py-3 rounded-full bg-white/10 hover:bg-white/20 border border-white/20 text-white font-semibold transition-all hover:scale-105 active:scale-95"
                >
                  Попробовать снова
                </button>
              </div>
            </div>
          )}

          {/* Top Gradient + Close Button */}
          <motion.div
            className="absolute top-0 left-0 right-0 z-20"
            initial={false}
            animate={{ opacity: showControls ? 1 : 0 }}
            transition={{ duration: 0.3 }}
            style={{ pointerEvents: showControls ? "auto" : "none" }}
          >
            <div className="h-28 bg-gradient-to-b from-black/70 to-transparent" />
            <motion.button
              className={cn(
                "absolute top-5 right-5 w-14 h-14 rounded-full",
                "bg-white/10 hover:bg-white/25 backdrop-blur-md border border-white/20",
                "flex items-center justify-center transition-all cursor-pointer"
              )}
              onClick={(e) => {
                e.stopPropagation();
                onClose();
              }}
              whileHover={{ scale: 1.1 }}
              whileTap={{ scale: 0.9 }}
            >
              <Icon name="close" size="xl" className="text-white" />
            </motion.button>
          </motion.div>

          {/* Bottom Controls */}
          {resolvedVideoSrc && !hasError && (
            <motion.div
              className="absolute bottom-0 left-0 right-0 z-20"
              initial={false}
              animate={{ opacity: showControls ? 1 : 0 }}
              transition={{ duration: 0.3 }}
              style={{ pointerEvents: showControls ? "auto" : "none" }}
              onClick={(e) => e.stopPropagation()}
            >
              <div className="bg-gradient-to-t from-black/80 via-black/40 to-transparent pt-20 pb-6 px-6">
                {/* Progress Bar */}
                <div
                  ref={progressRef}
                  className="relative h-2 bg-white/20 rounded-full cursor-pointer group mb-4 hover:h-3 transition-all"
                  onPointerDown={handleProgressPointerDown}
                >
                  {/* Buffered */}
                  <div
                    className="absolute inset-y-0 left-0 bg-white/30 rounded-full"
                    style={{ width: `${bufferedProgress}%` }}
                  />
                  {/* Progress */}
                  <div
                    className="absolute inset-y-0 left-0 bg-primary rounded-full"
                    style={{ width: `${progress}%` }}
                  />
                  {/* Thumb */}
                  <div
                    className="absolute top-1/2 -translate-y-1/2 w-4 h-4 bg-white rounded-full shadow-lg opacity-0 group-hover:opacity-100 transition-opacity"
                    style={{ left: `calc(${progress}% - 8px)` }}
                  />
                </div>

                {/* Controls Row */}
                <div className="flex items-center gap-4">
                  {/* Play/Pause */}
                  <button
                    onClick={togglePlay}
                    className="w-12 h-12 rounded-full bg-white/10 hover:bg-white/20 flex items-center justify-center transition-all hover:scale-110 active:scale-90"
                  >
                    <Icon
                      name={isPlaying ? "pause" : "play_arrow"}
                      className="!text-3xl text-white"
                    />
                  </button>

                  {/* Time */}
                  <span className="text-white/80 text-sm font-mono min-w-[100px]">
                    {formatTime(currentTime)} / {formatTime(duration)}
                  </span>

                  {/* Spacer */}
                  <div className="flex-1" />

                  {/* Volume */}
                  <div className="flex items-center gap-2">
                    <button
                      onClick={toggleMute}
                      className="w-10 h-10 rounded-full hover:bg-white/10 flex items-center justify-center transition-all flex-shrink-0"
                    >
                      <Icon
                        name={isMuted || volume === 0 ? "volume_off" : volume < 0.5 ? "volume_down" : "volume_up"}
                        className="!text-2xl text-white/80"
                      />
                    </button>
                    <input
                      type="range"
                      min={0}
                      max={1}
                      step={0.01}
                      value={isMuted ? 0 : volume}
                      onChange={(e) => {
                        const val = parseFloat(e.target.value);
                        const video = videoRef.current;
                        if (video) {
                          video.volume = val;
                          video.muted = val === 0;
                          setVolume(val);
                          setIsMuted(val === 0);
                        }
                        resetControlsTimer();
                      }}
                      className="w-24 cursor-pointer h-1 appearance-none bg-white/20 rounded-full [&::-webkit-slider-thumb]:appearance-none [&::-webkit-slider-thumb]:w-3 [&::-webkit-slider-thumb]:h-3 [&::-webkit-slider-thumb]:rounded-full [&::-webkit-slider-thumb]:bg-white [&::-webkit-slider-thumb]:cursor-pointer"
                    />
                  </div>

                  {/* Close */}
                  <button
                    onClick={onClose}
                    className="w-10 h-10 rounded-full hover:bg-white/10 flex items-center justify-center transition-all"
                  >
                    <Icon name="fullscreen_exit" className="!text-2xl text-white/80" />
                  </button>
                </div>
              </div>
            </motion.div>
          )}

          {/* Center Play Button (when paused and not loading) */}
          <AnimatePresence>
            {!isPlaying && !isLoading && !hasError && resolvedVideoSrc && showControls && (
              <motion.button
                className="absolute inset-0 flex items-center justify-center z-10 cursor-pointer"
                initial={{ opacity: 0, scale: 0.8 }}
                animate={{ opacity: 1, scale: 1 }}
                exit={{ opacity: 0, scale: 0.8 }}
                transition={{ duration: 0.2 }}
                onClick={togglePlay}
              >
                <div className="w-24 h-24 rounded-full bg-white/15 backdrop-blur-md border border-white/20 flex items-center justify-center hover:bg-white/25 hover:scale-110 transition-all">
                  <Icon name="play_arrow" className="!text-5xl text-white ml-1" />
                </div>
              </motion.button>
            )}
          </AnimatePresence>
        </motion.div>
      )}
    </AnimatePresence>
  );
};
