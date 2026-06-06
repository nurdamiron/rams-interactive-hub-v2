/**
 * Gallery Configuration - 13 зон круга KRUG2 (v2)
 * blockNumber = физический блок актуаторов на столе (см. docs/V2-SETUP/README.md §3)
 *   Mega #1 = блоки 1-7, Mega #2 = блоки 8-13
 */

import { getProjectById } from "./projects";

export interface GalleryCard {
  id: string;
  projectIds: string[]; // обычно одна зона; массив на случай мульти-лого
  name: string;
  title?: string;
  blockNumber: number; // физический блок (1-13)
}

export const GALLERY_CARDS: GalleryCard[] = [
  { id: "1",  projectIds: ["01-nomad"],                    name: "NOMAD",            blockNumber: 1 },
  { id: "2",  projectIds: ["02-grande-vie"],               name: "GRANDE VIE",       blockNumber: 2 },
  { id: "3",  projectIds: ["03-keruen-city"],              name: "KERUEN CITY",      blockNumber: 3 },
  { id: "4",  projectIds: ["04-rams-garden-bahcelievler"], name: "RAMS GARDEN", title: "BAHCELIEVLER", blockNumber: 4 },
  { id: "5",  projectIds: ["05-rams-resort-bodrum"],       name: "RAMS RESORT", title: "BODRUM",       blockNumber: 5 },
  { id: "6",  projectIds: ["06-rams-city-halic-2"],        name: "RAMS CITY",   title: "HALIC 2",      blockNumber: 6 },
  { id: "7",  projectIds: ["07-park-house-maslak"],        name: "PARK HOUSE",  title: "MASLAK",       blockNumber: 7 },
  { id: "8",  projectIds: ["08-sakura"],                   name: "SAKURA",           blockNumber: 8 },
  { id: "9",  projectIds: ["09-rams-city-halic-1"],        name: "RAMS CITY",   title: "HALIC 1",      blockNumber: 9 },
  { id: "10", projectIds: ["10-rams-city-gaziantep"],      name: "RAMS CITY",   title: "GAZIANTEP",    blockNumber: 10 },
  { id: "11", projectIds: ["11-baiterek-school"],          name: "BAITEREK",    title: "SCHOOL",       blockNumber: 11 },
  { id: "12", projectIds: ["12-hyatt-regency"],            name: "HYATT REGENCY",    blockNumber: 12 },
  { id: "13", projectIds: ["13-rams-city-almaty"],         name: "RAMS CITY",   title: "ALMATY",       blockNumber: 13 },
];

// Helper to get projects for a gallery card
export const getProjectsForCard = (card: GalleryCard) => {
  return card.projectIds.map(id => getProjectById(id)).filter(Boolean);
};

// Get all gallery cards with their projects
export const getGalleryData = () => {
  return GALLERY_CARDS.map(card => ({
    ...card,
    projects: getProjectsForCard(card),
  }));
};

// Get block number for a given project ID
export const getBlockNumberForProject = (projectId: string): number | undefined => {
  const card = GALLERY_CARDS.find(c => c.projectIds.includes(projectId));
  return card?.blockNumber;
};
