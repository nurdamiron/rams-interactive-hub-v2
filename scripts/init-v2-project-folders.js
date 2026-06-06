/**
 * init-v2-project-folders.js
 *
 * Создаёт скелет папок для 13 зон RAMS KRUG2 (v2) по соглашению media-scanner.ts.
 * Папки public/projects/** в .gitignore (медиа не хранится в git), поэтому
 * этот скрипт нужно прогнать на каждой машине, где раскладываешь медиа.
 *
 * Запуск:  node scripts/init-v2-project-folders.js
 *
 * После запуска просто кидай файлы в созданные папки:
 *   public/projects/<id>/images/main.jpg
 *   public/projects/<id>/images/logo/logo.svg
 *   public/projects/<id>/images/scenes/01.jpg, 02.jpg ...
 *   public/projects/<id>/videos/main.mp4
 * Сканер подхватит их сам — перечислять файлы в коде не нужно.
 */

const fs = require("fs");
const path = require("path");

// 13 зон в порядке блоков актуаторов (см. docs/V2-SETUP/README.md).
// id = префикс блока + slug. blockNumber = физический блок на столе.
const ZONES = [
  { id: "01-nomad",                     name: "NOMAD",                    block: 1,  mega: 1, actuators: 2 },
  { id: "02-grande-vie",                name: "GRANDE VIE",               block: 2,  mega: 1, actuators: 2 },
  { id: "03-keruen-city",               name: "KERUEN CITY",              block: 3,  mega: 1, actuators: 2 },
  { id: "04-rams-garden-bahcelievler",  name: "RAMS GARDEN BAHCELIEVLER", block: 4,  mega: 1, actuators: 2 },
  { id: "05-rams-resort-bodrum",        name: "RAMS RESORT BODRUM",       block: 5,  mega: 1, actuators: 2 },
  { id: "06-rams-city-halic-2",         name: "RAMS CITY HALIC 2",        block: 6,  mega: 1, actuators: 2 },
  { id: "07-park-house-maslak",         name: "PARK HOUSE MASLAK",        block: 7,  mega: 1, actuators: 3 },
  { id: "08-sakura",                    name: "SAKURA",                   block: 8,  mega: 2, actuators: 2 },
  { id: "09-rams-city-halic-1",         name: "RAMS CITY HALIC 1",        block: 9,  mega: 2, actuators: 2 },
  { id: "10-rams-city-gaziantep",       name: "RAMS CITY GAZIANTEP",      block: 10, mega: 2, actuators: 2 },
  { id: "11-baiterek-school",           name: "BAITEREK SCHOOL",          block: 11, mega: 2, actuators: 2 },
  { id: "12-hyatt-regency",             name: "HYATT REGENCY",            block: 12, mega: 2, actuators: 2 },
  { id: "13-rams-city-almaty",          name: "RAMS CITY ALMATY",         block: 13, mega: 2, actuators: 2 },
];

const ROOT = path.join(process.cwd(), "public", "projects");
const SUBDIRS = ["images/logo", "images/scenes", "videos"];

console.log(`Создаю скелет ${ZONES.length} зон в ${ROOT}\n`);

for (const z of ZONES) {
  for (const sub of SUBDIRS) {
    fs.mkdirSync(path.join(ROOT, z.id, sub), { recursive: true });
  }
  const readme = [
    `${z.name}`,
    `id: ${z.id}`,
    `block: ${z.block} (Mega ${z.mega}) | actuators: ${z.actuators}`,
    ``,
    `Куда класть файлы:`,
    `  images/main.jpg            - главный кадр (обязательно)`,
    `  images/logo/logo.svg       - лого (svg или png)`,
    `  images/scenes/01.jpg ...   - доп. фото по порядку`,
    `  videos/main.mp4            - видео-презентация (опц.)`,
    ``,
    `Сканер lib/media-scanner.ts подхватит файлы автоматически.`,
  ].join("\n");
  fs.writeFileSync(path.join(ROOT, z.id, "_README.txt"), readme);
  console.log(`  ✓ ${z.id}  (блок ${z.block}, Mega ${z.mega}, ${z.actuators} акт.)`);
}

console.log(`\nГотово. Кидай медиа в public/projects/<id>/...`);
console.log(`Для production (Windows) те же папки положи в media/projects/ рядом с .exe`);
