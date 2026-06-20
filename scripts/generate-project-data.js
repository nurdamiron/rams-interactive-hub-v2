/**
 * scripts/generate-project-data.js
 *
 * Scans public/projects/<id> folders for main.jpg, scenes, videos, and logo.
 * Combines this with rich metadata (status, info, features, locations)
 * to output a fully populated lib/data/projects.ts and public/projects.json.
 */

const fs = require("fs");
const path = require("path");

const ZONES = [
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

const METADATA = {
  "01-nomad": {
    status: "Сдан",
    info: { class: "Comfort", floors: 13, units: 200, ceilingHeight: "2.7м", deadline: "2022", quarter: "Завершен" },
    locations: [{ label: "Алмалинский район", distance: "300м", progress: 100 }],
    features: [
      "Стадионы и спортивные площадки по близости",
      "Развитая инфраструктура",
      "Лучшее предложение в своем сегменте",
      "Центр города",
      "Детский сад на 200 мест на территории ЖК",
      "Уютный двор на стилобате",
      "Большой подземный паркинг на 352 места",
      "Современный дизайн",
      "Идеально подходящие семейные планировки",
      "Сейсмостойкость"
    ]
  },
  "02-grande-vie": {
    status: "Сдан",
    info: { class: "Business+", floors: 12, units: 168, ceilingHeight: "3.0м", deadline: "2022", quarter: "Завершен" },
    locations: [{ label: "Ерменсай", distance: "400м", progress: 100 }],
    features: [
      "Прекрасная локация рядом с центром, в окружении гор",
      "ТРЦ Esentai Mall и VILLA в пяти минутах",
      "Элитные школы рядом",
      "Чистый горный воздух",
      "Собственный парк с фонтанами и скульптурами",
      "Панорамные окна с террасами",
      "12 зданий малой этажности",
      "Клубный формат с консьерж-сервисом"
    ]
  },
  "03-keruen-city": {
    status: "Строится",
    info: { class: "Business", floors: 20, units: 500, ceilingHeight: "2.9м", deadline: "2026", quarter: "III" },
    locations: [{ label: "Астана", distance: "0м", progress: 35 }],
    features: [
      "Зоны для отдыха",
      "Игровые и спортивные площадки",
      "Высокие потолки",
      "Авторский дизайн",
      "Рядом ТРЦ Aport Mall",
      "Рынок Алтын Орда поблизости",
      "Новая станция метро в будущем",
      "Современный подземный паркинг",
      "Закрытые дворы без машин",
      "Видеонаблюдение 24/7",
      "Смарт-замки"
    ]
  },
  "04-rams-garden-bahcelievler": {
    status: "Строится",
    info: { class: "Premium", floors: 15, units: 796, ceilingHeight: "3.1м", deadline: "2025", quarter: "IV" },
    locations: [{ label: "Бахчелиэвлер, Стамбул", distance: "100м", progress: 65 }],
    features: [
      "Расположен в центре Стамбула",
      "Огромный парк во внутреннем дворе",
      "Богатая инфраструктура для всей семьи",
      "Материалы премиум-класса",
      "Экологически чистое благоустройство",
      "Круглосуточная охрана и видеонаблюдение",
      "Просторные планировки",
      "Детские игровые и спортивные площадки"
    ]
  },
  "05-rams-resort-bodrum": {
    status: "Строится",
    info: { class: "Elite", floors: 3, units: 112, ceilingHeight: "3.2м", deadline: "2026", quarter: "III" },
    locations: [{ label: "Бодрум, Эгейское море", distance: "50м", progress: 45 }],
    features: [
      "Элитный курорт на побережье",
      "Виллы и резиденции с панорамным видом",
      "Собственный чистый пляж",
      "Бассейны-инфинити",
      "Сервис 5-звездочного отеля",
      "Приватная охраняемая территория",
      "Уникальная архитектура"
    ]
  },
  "06-rams-city-halic-2": {
    status: "Строится",
    info: { class: "Business+", floors: 16, units: 540, ceilingHeight: "3.0м", deadline: "2026", quarter: "IV" },
    locations: [{ label: "Халич, Стамбул", distance: "150м", progress: 40 }],
    features: [
      "Панорамный вид на Золотой Рог (Халич)",
      "Близость к историческому центру",
      "Развитая инфраструктура",
      "Современная архитектура",
      "Зеленые зоны отдыха во дворе",
      "Круглосуточный консьерж-сервис",
      "Фитнес и СПА центры"
    ]
  },
  "07-park-house-maslak": {
    status: "Строится",
    info: { class: "Premium", floors: 12, units: 180, ceilingHeight: "3.0м", deadline: "2026", quarter: "II" },
    locations: [{ label: "Маслак, Стамбул", distance: "200м", progress: 50 }],
    features: [
      "Престижное расположение в деловом центре",
      "Рядом с Белградским лесом",
      "Высокотехнологичные инженерные системы",
      "Собственная прогулочная аллея",
      "Панорамное остекление",
      "Удобные выезды на автомагистрали"
    ]
  },
  "08-sakura": {
    status: "Сдан",
    info: { class: "Business+", floors: 8, units: 350, ceilingHeight: "3.3м", deadline: "2024", quarter: "Завершен" },
    locations: [{ label: "Ремизовка", distance: "500м", progress: 100 }],
    features: [
      "Живописная закрытая территория площадью 7 гектаров",
      "Два собственных бассейна",
      "Зона для пикников и барбекю",
      "Крытый паркинг с местом для каждого жильца",
      "Высота потолков 3,3 метра",
      "Сейсмоустойчивость до 10 баллов",
      "13 жилых домов с панорамным остеклением",
      "Атмосфера элитного соседства",
      "Чистый горный воздух"
    ]
  },
  "09-rams-city-halic-1": {
    status: "Строится",
    info: { class: "Business+", floors: 16, units: 600, ceilingHeight: "3.0м", deadline: "2025", quarter: "IV" },
    locations: [{ label: "Халич, Стамбул", distance: "100м", progress: 70 }],
    features: [
      "Первая очередь элитного комплекса на побережье",
      "Панорамное остекление с шикарными видами",
      "Современная система безопасности",
      "All-in-One концепция благоустройства",
      "Близость к метро и транспортным узлам",
      "Внутренний ландшафтный сад"
    ]
  },
  "10-rams-city-gaziantep": {
    status: "Строится",
    info: { class: "Business", floors: 14, units: 420, ceilingHeight: "2.9м", deadline: "2026", quarter: "II" },
    locations: [{ label: "Газиантеп", distance: "100м", progress: 35 }],
    features: [
      "Премиальное качество сборки",
      "Просторные семейные квартиры",
      "Собственный торговый променад",
      "Детские развивающие центры",
      "Энергоэффективные строительные материалы",
      "Закрытый двор без машин"
    ]
  },
  "11-baiterek-school": {
    status: "Сдан",
    info: { class: "Образовательный", floors: 4, units: 0, ceilingHeight: "3.6м", deadline: "2023", quarter: "Завершен" },
    locations: [{ label: "Алматы", distance: "0м", progress: 100 }],
    features: [
      "Передовое техническое оснащение классов",
      "Спортивные залы и бассейн",
      "Современные научные лаборатории",
      "Просторный актовый зал",
      "Безопасная охраняемая территория",
      "Комфортная столовая и зоны отдыха"
    ]
  },
  "12-hyatt-regency": {
    status: "Строится",
    info: { class: "Premium+", floors: 18, units: 220, ceilingHeight: "3.4м", deadline: "2026", quarter: "IV" },
    locations: [{ label: "Алматы", distance: "0м", progress: 25 }],
    features: [
      "Обслуживание под всемирным брендом Hyatt 5*",
      "Премиальные жилые резиденции",
      "Панорамный вид на Заилийский Алатау",
      "Конференц-залы мирового уровня",
      "СПА, фитнес-центр и закрытый бассейн",
      "Эксклюзивный ресторан высокой кухни"
    ]
  },
  "13-rams-city-almaty": {
    status: "Сдана 1 очередь",
    info: { class: "Comfort+", floors: 12, units: 400, ceilingHeight: "2.8м", deadline: "2024", quarter: "II" },
    locations: [{ label: "Жандосова", distance: "500м", progress: 70 }],
    features: [
      "Отличная локация у реки Большая Алматинка",
      "Школа во дворе",
      "Близость к ТРЦ ADK и Mega",
      "В пяти минутах Парк Первого Президента",
      "Зеленый RAMS бульвар протяженностью более километра",
      "Современные детские и workout-площадки",
      "Комнаты развлечений для взрослых и детей",
      "Фонтан как место встреч",
      "Собственный бульвар для прогулок"
    ]
  }
};

const projectDirRoot = path.join(__dirname, "..", "public", "projects");
const projectsOutput = [];

for (const z of ZONES) {
  const projPath = path.join(projectDirRoot, z.id);
  const metadata = METADATA[z.id] || {
    status: "В продаже",
    info: { class: "", floors: 0, units: 0, ceilingHeight: "", deadline: "" },
    locations: [],
    features: []
  };

  let mainImage = `/projects/${z.id}/images/main.jpg`;
  let logo = `/projects/${z.id}/images/logo/logo.svg`;

  // Scan logo file
  const logoDir = path.join(projPath, "images", "logo");
  if (fs.existsSync(logoDir)) {
    const files = fs.readdirSync(logoDir);
    const foundLogo = files.find(f => f.startsWith("logo."));
    if (foundLogo) {
      logo = `/projects/${z.id}/images/logo/${foundLogo}`;
    }
  }

  // Scan videos
  const videoDir = path.join(projPath, "videos");
  const videos = [];
  if (fs.existsSync(videoDir)) {
    const files = fs.readdirSync(videoDir)
      .filter(f => /\.(mp4|webm|mov)$/i.test(f))
      .sort();
    files.forEach(f => {
      videos.push(`/projects/${z.id}/videos/${f}`);
    });
  }

  // Scan scenes
  const scenesDir = path.join(projPath, "images", "scenes");
  const scenesList = [];
  if (fs.existsSync(scenesDir)) {
    const files = fs.readdirSync(scenesDir)
      .filter(f => /\.(jpg|jpeg|png|webp)$/i.test(f))
      .sort();
    files.forEach(f => {
      scenesList.push(`/projects/${z.id}/images/scenes/${f}`);
    });
  }

  // Construct scenes list
  const scenes = [];
  let sceneIdCounter = 1;

  // Add videos first
  videos.forEach(v => {
    const fileName = path.basename(v, path.extname(v));
    scenes.push({
      id: `v${sceneIdCounter++}`,
      type: "Видео",
      title: fileName === "main" ? "Презентация" : fileName,
      image: mainImage,
      video: v,
      isActive: scenes.length === 0
    });
  });

  // Add main image
  scenes.push({
    id: String(sceneIdCounter++),
    type: "Фото",
    title: "Главный вид",
    image: mainImage,
    isActive: scenes.length === 0
  });

  // Add other scenes
  scenesList.forEach((s, idx) => {
    scenes.push({
      id: String(sceneIdCounter++),
      type: "Фото",
      title: `Вид ${idx + 1}`,
      image: s,
      isActive: false
    });
  });

  const projData = {
    id: z.id,
    slug: z.id.replace(/^\d+-/, ""),
    name: z.name,
    title: z.title,
    description: "",
    status: metadata.status,
    image: mainImage,
    logo: logo,
    info: metadata.info,
    locations: metadata.locations,
    features: metadata.features,
    scenes: scenes
  };

  projectsOutput.push(projData);
}

// Generate TS contents
const tsContent = `/**
 * RAMS Interactive Hub - Project Data (v2 / KRUG2)
 * AUTO-GENERATED BY scripts/generate-project-data.js
 * DO NOT EDIT MANUALLY! Run "node scripts/generate-project-data.js" to update.
 */

import { Project } from "../types";

export const RAMS_PROJECTS: Project[] = ${JSON.stringify(projectsOutput, null, 2)};

export const getProjectBySlug = (slug: string) => RAMS_PROJECTS.find((p) => p.slug === slug);
export const getProjectById = (id: string) => RAMS_PROJECTS.find((p) => p.id === id);
export const getActiveProjects = () => RAMS_PROJECTS.filter((p) => p.status === "Строится" || p.status.includes("очередь"));
export const getCompletedProjects = () => RAMS_PROJECTS.filter((p) => p.status === "Сдан");
export const getAllProjects = () => RAMS_PROJECTS;
`;

fs.writeFileSync(path.join(__dirname, "..", "lib", "data", "projects.ts"), tsContent, "utf8");
console.log("✓ Successfully generated lib/data/projects.ts");

// Generate JSON contents
fs.writeFileSync(path.join(__dirname, "..", "public", "projects.json"), JSON.stringify(projectsOutput, null, 2), "utf8");
console.log("✓ Successfully generated public/projects.json");
