/**
 * Placeholder Image URLs
 * Using Unsplash for high-quality architectural images
 */

const architectureImages = [
  "photo-1545324418-cc1a3fa10c00", // Modern facade 1
  "photo-1600607687939-ce8a6c25118c", // Modern lobby
  "photo-1600607687644-c7171b42498f", // Courtyard
  "photo-1600607687920-4e2a09cf159d", // Office space
  "photo-1600585154340-be6161a56a0c", // Luxury building
  "photo-1600047509807-ba8f99d2cdde", // Modern building exterior
  "photo-1512917774080-9991f1c4c750", // Luxury building night
  "photo-1580587771525-78b9dba3b914", // Modern residential
  "photo-1600607687920-4e2a09cf159d", // Business center
  "photo-1600566753190-17f0baa2a6c3", // Tall building
];

export const getPlaceholderImage = (index: number, width: number = 800, height: number = 1000): string => {
  const imageId = architectureImages[index % architectureImages.length];
  return `https://images.unsplash.com/${imageId}?w=${width}&h=${height}&fit=crop&q=80`;
};

export const getProjectPlaceholder = (projectIndex: number) => ({
  card: getPlaceholderImage(projectIndex, 800, 1000),
  hero: getPlaceholderImage(projectIndex, 1920, 1080),
  scenes: {
    facade: getPlaceholderImage(projectIndex, 1920, 1080),
    lobby: getPlaceholderImage(projectIndex + 1, 1920, 1080),
    courtyard: getPlaceholderImage(projectIndex + 2, 1920, 1080),
    office: getPlaceholderImage(projectIndex + 3, 1920, 1080),
    panorama: getPlaceholderImage(projectIndex + 4, 1920, 1080),
  },
});
