/**
 * Media Scanner Utility
 * Automatically scans project folders for media files
 * Works on server-side (Node.js) or in Electron
 */

import { Scene } from "./types";

interface MediaFiles {
  mainImage: string | null;
  logo: string | null;
  scenes: string[];
  videos: string[];
}

/**
 * Get real media files for a project by scanning the folder
 * This runs on the server side during build/SSR
 */
export async function scanProjectMedia(projectId: string): Promise<MediaFiles> {
  // Only works on server
  if (typeof window !== "undefined") {
    return { mainImage: null, logo: null, scenes: [], videos: [] };
  }

  const fs = await import("fs");
  const path = await import("path");

  const projectPath = path.join(process.cwd(), "public", "projects", projectId);
  const result: MediaFiles = {
    mainImage: null,
    logo: null,
    scenes: [],
    videos: [],
  };

  try {
    // Check if project folder exists
    if (!fs.existsSync(projectPath)) {
      return result;
    }

    // Check for main image
    const imagesPath = path.join(projectPath, "images");
    if (fs.existsSync(imagesPath)) {
      const mainImagePath = path.join(imagesPath, "main.jpg");
      if (fs.existsSync(mainImagePath)) {
        result.mainImage = `/projects/${projectId}/images/main.jpg`;
      }

      // Check for logo
      const logoPath = path.join(imagesPath, "logo");
      if (fs.existsSync(logoPath)) {
        const logoFiles = fs.readdirSync(logoPath);
        const logoFile = logoFiles.find(f => f.startsWith("logo."));
        if (logoFile) {
          result.logo = `/projects/${projectId}/images/logo/${logoFile}`;
        }
      }

      // Check for scenes
      const scenesPath = path.join(imagesPath, "scenes");
      if (fs.existsSync(scenesPath)) {
        const sceneFiles = fs.readdirSync(scenesPath)
          .filter(f => /\.(jpg|jpeg|png|webp)$/i.test(f))
          .sort();
        result.scenes = sceneFiles.map(f => `/projects/${projectId}/images/scenes/${f}`);
      }
    }

    // Check for videos
    const videosPath = path.join(projectPath, "videos");
    if (fs.existsSync(videosPath)) {
      const videoFiles = fs.readdirSync(videosPath)
        .filter(f => /\.(mp4|webm|mov)$/i.test(f))
        .sort();
      result.videos = videoFiles.map(f => `/projects/${projectId}/videos/${f}`);
    }

  } catch (error) {
    console.error(`Error scanning media for ${projectId}:`, error);
  }

  return result;
}

/**
 * Generate scenes array from scanned media
 */
export function generateScenes(media: MediaFiles, placeholderImage: string): Scene[] {
  const scenes: Scene[] = [];
  let idCounter = 1;

  // Add videos first
  media.videos.forEach((videoPath) => {
    const fileName = videoPath.split("/").pop()?.replace(/\.[^.]+$/, "") || "Видео";
    scenes.push({
      id: `v${idCounter}`,
      type: "Видео",
      title: fileName === "main" ? "Презентация" : fileName,
      image: media.mainImage || placeholderImage,
      video: videoPath,
      isActive: scenes.length === 0, // First scene is active
    });
    idCounter++;
  });

  // Add main image if exists
  if (media.mainImage) {
    scenes.push({
      id: String(idCounter++),
      type: "Фото",
      title: "Главный вид",
      image: media.mainImage,
      isActive: scenes.length === 0,
    });
  }

  // Add scene images
  media.scenes.forEach((scenePath, index) => {
    scenes.push({
      id: String(idCounter++),
      type: "Фото",
      title: `Вид ${index + 1}`,
      image: scenePath,
      isActive: false,
    });
  });

  // If no media at all, add placeholder
  if (scenes.length === 0) {
    scenes.push({
      id: "1",
      type: "Фото",
      title: "Главный вид",
      image: placeholderImage,
      isActive: true,
    });
  }

  return scenes;
}

/**
 * Count media files for a project
 */
export function countMedia(scenes: Scene[]): { videos: number; photos: number } {
  return {
    videos: scenes.filter(s => s.video).length,
    photos: scenes.filter(s => !s.video).length,
  };
}
