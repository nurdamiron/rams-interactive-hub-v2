/**
 * Video Preload Hook
 * Preloads project videos for instant playback
 */

import { useEffect } from "react";
import { Project } from "@/lib/types";
import { getMediaUrl } from "@/lib/media-utils";

export function useVideoPreload(projects: Project[]) {
  useEffect(() => {
    // Get all video URLs from projects
    const videoUrls: string[] = [];

    projects.forEach((project) => {
      project.scenes.forEach((scene) => {
        if (scene.video) {
          videoUrls.push(getMediaUrl(scene.video));
        }
      });
    });

    // Preload each video
    videoUrls.forEach((url) => {
      const link = document.createElement("link");
      link.rel = "preload";
      link.as = "video";
      link.href = url;
      document.head.appendChild(link);
    });

    // Cleanup on unmount
    return () => {
      document.querySelectorAll('link[rel="preload"][as="video"]').forEach((link) => {
        link.remove();
      });
    };
  }, [projects]);
}

/**
 * Preload a single video programmatically
 */
export function preloadVideo(url: string): Promise<void> {
  return new Promise((resolve, reject) => {
    const video = document.createElement("video");
    video.preload = "auto";
    video.src = url;

    video.oncanplaythrough = () => {
      resolve();
    };

    video.onerror = () => {
      reject(new Error(`Failed to preload video: ${url}`));
    };

    // Start loading
    video.load();
  });
}
