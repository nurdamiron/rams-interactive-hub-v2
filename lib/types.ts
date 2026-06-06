/**
 * RAMS Interactive Hub - TypeScript Types
 */

export type ProjectStatus = "В продаже" | "Сдан" | "Строится" | "Скоро" | "Сдана 1 очередь" | "Сдана 2 очередь" | "Сдана 3 очередь" | "Сдана 4 очередь" | "Реализован" | string;

export type SceneType = "Фасад" | "Лобби" | "Двор" | "Офис" | "Панорама" | "Завод" | "Видео" | "Фото" | string;

export type ModelElement =
  | "Подсветка"
  | "Ландшафт"
  | "Паркинг"
  | "Фасады"
  | "Кровля"
  | "Лифты";

export interface Location {
  label: string;
  distance: string;
  progress: number;
}

export interface ProjectInfo {
  class: string;
  floors: number;
  units: number;
  ceilingHeight: string;
  deadline: string;
  quarter?: string;
}

export interface Scene {
  id: string;
  type: SceneType;
  title: string;
  image: string;
  video?: string;
  isActive?: boolean;
}

export interface Project {
  id: string;
  slug: string;
  name: string;
  title: string;
  subtitle?: string;
  description: string;
  status: ProjectStatus;
  statusBadge?: string;
  image: string;
  logo?: string;
  heroImage?: string;
  info: ProjectInfo;
  locations: Location[];
  scenes: Scene[];
  features?: string[];
  concept?: string;
  salesProgress?: number;
  constructionTimeline?: string;
  hardwareId?: number;
  presentationUrl?: string; // Link to video/presentation/booklet
}

export interface ModelControlState {
  element: ModelElement;
  isActive: boolean;
  icon?: string;
}

export interface ConnectionStatus {
  isConnected: boolean;
  latency?: number;
  battery?: number;
  lastUpdate?: Date;
}

export interface AudioGuideState {
  isPlaying: boolean;
  currentTrack?: string;
  volume?: number;
}

export interface AppState {
  currentProject: Project | null;
  currentScene: Scene | null;
  modelControls: ModelControlState[];
  connectionStatus: ConnectionStatus;
  audioGuide: AudioGuideState;
  isFullscreenVideo: boolean;
  isInfoModalOpen: boolean;
  theme: "light" | "dark";
  language: "ru" | "en" | "kk";
}

export interface NavigationItem {
  label: string;
  icon?: string;
  href?: string;
  onClick?: () => void;
  isActive?: boolean;
}

export interface StatCardData {
  label: string;
  value: string;
  sublabel?: string;
  icon?: string;
}

export interface TimelineEvent {
  date: string;
  title: string;
  description?: string;
  isCompleted: boolean;
}
