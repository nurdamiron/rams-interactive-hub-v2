/**
 * Scene Selector Component
 * Carousel for switching between different project scenes
 */

"use client";

import * as React from "react";
import { motion } from "framer-motion";
import { Scene } from "@/lib/types";
import { cn } from "@/lib/utils";
import { Icon } from "@/components/ui/icon";
import { getMediaUrl } from "@/lib/media-utils";

interface SceneSelectorProps {
  scenes: Scene[];
  activeScene: Scene;
  onSceneChange: (scene: Scene) => void;
  theme?: "light" | "dark";
}

export const SceneSelector: React.FC<SceneSelectorProps> = ({
  scenes,
  activeScene,
  onSceneChange,
  theme = "dark",
}) => {
  return (
    <div className="w-full h-full flex items-center overflow-x-auto custom-scrollbar px-4 pb-2 pointer-events-auto">
      <div className="flex gap-4 mx-auto">
        {scenes.map((scene) => {
          const isActive = scene.id === activeScene.id;
          const sceneImage = getMediaUrl(scene.image);

          return (
            <motion.button
              key={scene.id}
              onClick={() => onSceneChange(scene)}
              className={cn(
                "relative flex-shrink-0 group overflow-hidden rounded-xl transition-all duration-300",
                "h-24 w-36 border-2",
                isActive
                  ? "border-primary shadow-lg shadow-primary/50 scale-110 z-50"
                  : "border-white/40 opacity-80 hover:opacity-100 hover:border-white/60 hover:z-40 hover:scale-105"
              )}
              whileHover={{ scale: isActive ? 1.1 : 1.05 }}
              whileTap={{ scale: 0.98 }}
            >
              {/* Background Image */}
              <div
                className="absolute inset-0 bg-cover bg-center transition-transform duration-500 group-hover:scale-110"
                style={{ backgroundImage: `url('${sceneImage}')` }}
              />

              {/* Overlay */}
              <div className={cn(
                "absolute inset-0 transition-colors duration-300",
                isActive ? "bg-black/10" : "bg-black/30 group-hover:bg-black/20"
              )} />

              {/* Video Indicator */}
              {scene.video && (
                <div className="absolute top-2 right-2 w-6 h-6 rounded-full bg-primary/90 flex items-center justify-center shadow-lg z-10">
                  <Icon name="play_arrow" size="sm" className="text-white ml-0.5 !text-[16px]" />
                </div>
              )}
            </motion.button>
          );
        })}
      </div>
    </div>
  );
};
