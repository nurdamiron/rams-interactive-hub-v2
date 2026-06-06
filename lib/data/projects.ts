/**
 * RAMS Interactive Hub - Project Data (v2 / KRUG2)
 * 13 зон круга KRUG2. Названия — с раскладки стола.
 * Описания/характеристики заполнить позже; медиа подтягивается
 * автоматически из public/projects/<id>/ (см. lib/media-scanner.ts).
 */

import { Project } from "../types";

interface ZoneSpec {
  id: string;        // префикс блока + slug, совпадает с папкой в public/projects
  name: string;      // верхняя строка названия
  title: string;     // вторая строка (локация/уточнение)
}

// Порядок = порядок блоков актуаторов (см. docs/V2-SETUP/README.md §3)
const ZONE_SPECS: ZoneSpec[] = [
  { id: "01-nomad",                    name: "NOMAD",         title: "" },
  { id: "02-grande-vie",               name: "GRANDE VIE",    title: "" },
  { id: "03-keruen-city",              name: "KERUEN CITY",   title: "" },
  { id: "04-rams-garden-bahcelievler", name: "RAMS GARDEN",   title: "BAHCELIEVLER" },
  { id: "05-rams-resort-bodrum",       name: "RAMS RESORT",   title: "BODRUM" },
  { id: "06-rams-city-halic-2",        name: "RAMS CITY",     title: "HALIC 2" },
  { id: "07-park-house-maslak",        name: "PARK HOUSE",    title: "MASLAK" },
  { id: "08-sakura",                   name: "SAKURA",        title: "" },
  { id: "09-rams-city-halic-1",        name: "RAMS CITY",     title: "HALIC 1" },
  { id: "10-rams-city-gaziantep",      name: "RAMS CITY",     title: "GAZIANTEP" },
  { id: "11-baiterek-school",          name: "BAITEREK",      title: "SCHOOL" },
  { id: "12-hyatt-regency",            name: "HYATT REGENCY", title: "" },
  { id: "13-rams-city-almaty",         name: "RAMS CITY",     title: "ALMATY" },
];

export const RAMS_PROJECTS: Project[] = ZONE_SPECS.map((z) => ({
  id: z.id,
  slug: z.id.replace(/^\d+-/, ""),
  name: z.name,
  title: z.title,
  description: "",
  status: "В продаже",
  image: `/projects/${z.id}/images/main.jpg`,
  logo: `/projects/${z.id}/images/logo/logo.svg`,
  info: { class: "", floors: 0, units: 0, ceilingHeight: "", deadline: "" },
  locations: [],
  scenes: [
    {
      id: "1",
      type: "Фото",
      title: "Главный вид",
      image: `/projects/${z.id}/images/main.jpg`,
      isActive: true,
    },
  ],
}));

export const getProjectBySlug = (slug: string) => RAMS_PROJECTS.find((p) => p.slug === slug);
export const getProjectById = (id: string) => RAMS_PROJECTS.find((p) => p.id === id);
export const getActiveProjects = () => RAMS_PROJECTS.filter((p) => p.status === "Строится" || p.status.includes("очередь"));
export const getCompletedProjects = () => RAMS_PROJECTS.filter((p) => p.status === "Сдан");
export const getAllProjects = () => RAMS_PROJECTS;
