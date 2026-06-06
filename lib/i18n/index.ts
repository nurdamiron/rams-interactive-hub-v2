export { LanguageProvider, useLanguage } from "./context";
export { translations, languageNames, languageFlags } from "./translations";
export type { Language, TranslationKey } from "./translations";
export {
  projectTranslations,
  getLocalizedProject,
  getLocalizedStatus,
  getLocalizedSceneType,
  getLocalizedSceneTitle,
  getLocalizedClass,
  getLocalizedQuarter,
  getLocalizedLocation,
  getLocalizedMeasurement,
  getLocalizedCeilingHeight,
  getLocalizedDistance,
  isProjectUnderConstruction,
  statusTranslations,
  sceneTypeTranslations,
  classTranslations,
  quarterTranslations,
  locationTranslations,
  unitTranslations,
} from "./project-translations";
export type { LocalizedText, ProjectTranslation } from "./project-translations";
