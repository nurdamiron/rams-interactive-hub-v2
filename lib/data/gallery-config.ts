/**
 * Gallery Configuration - 15 cards with some having multiple logos
 */

import { RAMS_PROJECTS, getProjectById } from "./projects";

export interface GalleryCard {
  id: string;
  projectIds: string[]; // Can have multiple projects (for multi-logo cards)
  name: string;
  title?: string;
  blockNumber: number; // Physical block number on the kinetic table (1-15)
}

// 15 cards configuration - Original brand names (no translation)
// blockNumber corresponds to the physical block on the kinetic table (1-15)
export const GALLERY_CARDS: GalleryCard[] = [
  { id: "1", projectIds: ["09-rams-garden-almaty"], name: "RAMS GARDEN", title: "ALMATY", blockNumber: 1 },
  { id: "2", projectIds: ["23-haval", "27-hyundai", "24-lukoil"], name: "HAVAL • HYUNDAI • LUKOIL", blockNumber: 2 },
  { id: "3", projectIds: ["22-almaty-museum"], name: "ALMATY MUSEUM OF ARTS", blockNumber: 3 },
  { id: "4", projectIds: ["18-lamiya"], name: "LAMIYA", blockNumber: 4 },
  { id: "5", projectIds: ["11-rams-signature"], name: "RAMS SIGNATURE", blockNumber: 5 },
  { id: "6", projectIds: ["20-ile-de-france"], name: "ILE DE FRANCE", blockNumber: 6 },
  { id: "7", projectIds: ["12-rams-saiahat"], name: "RAMS SAIAHAT", blockNumber: 7 },
  { id: "8", projectIds: ["13-rams-garden-atyrau"], name: "RAMS GARDEN", title: "ATYRAU", blockNumber: 8 },
  { id: "9", projectIds: ["15-rams-evo"], name: "RAMS EVO", blockNumber: 9 },
  { id: "10", projectIds: ["21-forum-residence"], name: "FORUM RESIDENCE", blockNumber: 10 },
  { id: "11", projectIds: ["08-vostochny-park"], name: "VOSTOCHNY PARK", blockNumber: 11 },
  { id: "12", projectIds: ["19-la-verde"], name: "LA VERDE", blockNumber: 12 },
  { id: "13", projectIds: ["05-dom-na-abaya"], name: "DOM NA ABAYA", blockNumber: 13 },
  { id: "14", projectIds: ["17-ortau", "14-ortau-marriott-bc"], name: "ORTAU", blockNumber: 14 },
  { id: "15", projectIds: ["26-marriott-issykkul"], name: "MARRIOTT ISSYK-KUL", blockNumber: 15 },
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
// Returns the blockNumber of the gallery card that contains this project
export const getBlockNumberForProject = (projectId: string): number | undefined => {
  const card = GALLERY_CARDS.find(c => c.projectIds.includes(projectId));
  return card?.blockNumber;
};
