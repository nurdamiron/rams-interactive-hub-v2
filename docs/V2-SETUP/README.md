# RAMS Interactive Hub v2 — Инструкция по сборке и заливке

Версия для нового круга **KRUG2**: 13 зон, лента **WS2815 (12V)**, 2× Arduino Mega + ESP32.

Репозиторий: https://github.com/nurdamiron/rams-interactive-hub-v2

---

## 0. Архитектура за 1 минуту

```
┌─────────────────────────┐     WiFi (HTTP)      ┌──────────────┐
│  Windows-мини-ПК         │  ───────────────▶   │    ESP32     │
│  Electron + Next.js (UI) │   BLOCK:5:UP:10000  │  WebServer   │
│  (киоск на экране)       │                     │  + LED WS2815│
└─────────────────────────┘                     └──────┬───────┘
                                                Serial │ (текст-протокол)
                                          ┌────────────┴────────────┐
                                     ┌────▼─────┐             ┌──────▼────┐
                                     │ Mega #1  │             │  Mega #2  │
                                     │ блоки 1-7│             │ блоки 8-13│
                                     │ реле→акт.│             │ реле→акт. │
                                     └──────────┘             └───────────┘
```

- **UI** (этот репозиторий, Next.js + Electron) рисует галерею и шлёт команды.
- **ESP32** держит Wi-Fi WebServer, рулит лентой WS2815 и роутит команды на Mega.
- **Mega #1/#2** двигают актуаторы через релейные модули.

---

## 1. Что куда заливать (firmware)

Все скетчи — в папке `firmware/`. **Источник истины по пинам** — `ACTUATOR_CONFIG.h` (один файл, копируется к каждому скетчу).

| Плата | Скетч (Arduino IDE) | Назначение |
|-------|---------------------|------------|
| **ESP32** | `firmware/PRODUCTION_v3.2_FINAL/esp32/rams_controller_v3/` | Wi-Fi, лента WS2815, роутинг команд |
| **Arduino Mega #1** | `firmware/PRODUCTION_v3.2_FINAL/mega1/` (`actuator_mega1_v3.ino`) | блоки **1–7** |
| **Arduino Mega #2** | `firmware/PRODUCTION_v3.2_FINAL/mega2/` (`actuator_mega2_v3.ino`) | блоки **8–13** |

> `firmware/esp32/led_test/` — отдельный тест ленты (подсветка зон) без актуаторов. Удобно проверить пайку WS2815 перед боевым скетчем.

### Библиотеки (Arduino IDE → Library Manager)
- **FastLED** (лента WS2815)
- **WiFi**, **WebServer**, **ESPmDNS**, **ArduinoOTA** — входят в пакет **ESP32 board** (Boards Manager → «esp32» by Espressif)

### Настройки платы в Arduino IDE
- ESP32: Board = «ESP32 Dev Module», Upload Speed 921600, Partition «Default 4MB with spiffs».
- Mega: Board = «Arduino Mega or Mega 2560», Processor «ATmega2560».

### Порядок заливки
1. Залей **обе Mega** (mega1 → плата #1, mega2 → плата #2). Перепутать нельзя — у них разные блоки/пины.
2. Залей **ESP32**. Пропиши Wi-Fi в скетче (см. `PRODUCTION_v3.2_FINAL/WIFI_SETUP.md`).
3. ESP32 поднимет WebServer; UI коннектится к нему по IP/`.local`.

---

## 2. Лента WS2815 (важно!)

Новая лента **WS2815, 12V, dual-signal**: два сигнальных входа — **DI** (данные) и **BI** (резерв).

- Управляющий пин ESP32 → **DI**.
- **BI** → перемычкой на тот же DI в начале ленты (даёт резервирование: один битый диод не гасит ленту). На питание BI не подавать.
- В прошивке уже стоит:
  ```cpp
  #define LED_TYPE WS2815   // если FastLED не знает — заменить на WS2812B
  #define COLOR_ORDER GRB
  ```
- Питание ленты — отдельный **12V** БП, общий GND с ESP32.
- 9 дата-линий, у каждой свой GPIO (см. `firmware/esp32/.../ZONE_CONFIG.h`).

> ⚠️ Старая лента была WS2811 (1 адрес = 3 диода). У WS2815 **1 адрес = 1 диод** — это меняет расчёт `NUM_LEDS`.

---

## 3. Карта блоков, актуаторов и пинов (KRUG2)

13 зон, **27 актуаторов = 54 реле-канала**. Каждый актуатор = 2 пина (UP/DOWN), реле инверсное (LOW = вкл).

### Mega #1 — блоки 1–7 (15 актуаторов, пины 22–51)

| Блок | Зона | Акт | Пины (UP,DOWN) |
|:--:|------|:--:|----|
| 1 | NOMAD | 2 | 22,23 / 24,25 |
| 2 | GRANDE VIE | 2 | 26,27 / 28,29 |
| 3 | KERUEN CITY | 2 | 30,31 / 32,33 |
| 4 | RAMS GARDEN BAHCELIEVLER | 2 | 34,35 / 36,37 |
| 5 | RAMS RESORT BODRUM | 2 | 38,39 / 40,41 |
| 6 | RAMS CITY HALIC 2 | 2 | 42,43 / 44,45 |
| 7 | PARK HOUSE MASLAK | 3 | 46,47 / 48,49 / 50,51 |

### Mega #2 — блоки 8–13 (12 актуаторов, пины 22–45)

| Блок | Зона | Акт | Пины (UP,DOWN) |
|:--:|------|:--:|----|
| 8 | SAKURA | 2 | 22,23 / 24,25 |
| 9 | RAMS CITY HALIC 1 | 2 | 26,27 / 28,29 |
| 10 | RAMS CITY GAZIANTEP | 2 | 30,31 / 32,33 |
| 11 | BAITEREK SCHOOL | 2 | 34,35 / 36,37 |
| 12 | HYATT REGENCY | 2 | 38,39 / 40,41 |
| 13 | RAMS CITY ALMATY | 2 | 42,43 / 44,45 |

Одновременно активны максимум **2 блока** (`MAX_ACTIVE_BLOCKS`). На плату — 2× 16-канальных реле-модуля.

> Статус правок: смена ленты на WS2815 уже в коде. `TOTAL_BLOCKS=13` и `BLOCK_CONFIGS[]` под эту таблицу **ещё нужно вписать** в `ACTUATOR_CONFIG.h` (ждёт подтверждения).

---

## 4. Зоны и медиа (UI)

### Как программа собирает галерею
- **Список зон — статический**, задаётся в коде:
  - `lib/data/projects.ts` — карточки проектов (id, название, описание).
  - `lib/data/gallery-config.ts` — карточки галереи + `blockNumber` (привязка к актуатору).
- **Медиа внутри зоны — автоматически** через `lib/media-scanner.ts`. Перечислять файлы в коде не нужно.

### Структура папки зоны
```
public/projects/<id>/
├── images/
│   ├── main.jpg            # главный кадр (обязательно)
│   ├── logo/logo.svg       # лого (svg/png)
│   └── scenes/01.jpg ...   # доп. фото по порядку
└── videos/main.mp4         # видео (опц., идёт первым в галерее)
```
Создать скелет всех 13 папок: `node scripts/init-v2-project-folders.js`

### Dev vs Production (медиа НЕ в git — `.gitignore`)
- **DEV (Mac/разработка):** `public/projects/<id>/...`
- **PROD (Windows .exe):** `media/projects/<id>/...` **рядом с .exe**.
  Electron-builder специально не кладёт медиа в exe (`!public/projects/**`), а протокол `media://` в `electron/main.js` берёт их из папки `media/` рядом с программой.

### Добавить/изменить зону
1. Добавь запись в `lib/data/projects.ts` (id = `XX-slug`).
2. Добавь карточку в `lib/data/gallery-config.ts` с нужным `blockNumber`.
3. Создай папку `public/projects/XX-slug/...` и накидай медиа.

---

## 5. Сборка UI

### Требования
- Node.js 18+ и npm.
- `npm install` в корне репозитория.

### Разработка (Mac/Win)
```bash
npm run dev            # Next.js на http://localhost:3000
npm run electron:dev   # Next + Electron-окно (киоск)
```

### Production-сборка под Windows (.exe)
```bash
npm run electron:build:win
```
Результат — в `dist/`. Рядом с .exe создай папку `media/projects/...` и положи медиа.

Подробности по Windows и автозапуску-киоску — в корневых файлах:
`WINDOWS_INSTALL.md`, `BUILD_ON_WINDOWS.md`, `WINDOWS_BUILD_INSTRUCTIONS.md`, `start_kiosk.bat`, `install_autostart.bat`.

---

## 6. Чек-лист запуска «с нуля»

- [ ] Залить mega1 (блоки 1–7) и mega2 (блоки 8–13).
- [ ] Спаять WS2815: DI ← GPIO, BI ↔ DI (перемычка), 12V + общий GND.
- [ ] Прогнать `led_test` — проверить зоны/цвета (GRB).
- [ ] Залить ESP32, прописать Wi-Fi, проверить WebServer (IP в Serial Monitor).
- [ ] Вписать `TOTAL_BLOCKS=13` + `BLOCK_CONFIGS[]` в `ACTUATOR_CONFIG.h` (см. §3).
- [ ] Обновить `projects.ts` + `gallery-config.ts` на 13 зон.
- [ ] Создать папки (`node scripts/init-v2-project-folders.js`) и накидать медиа.
- [ ] `npm run electron:build:win`, положить `media/` рядом с .exe.
- [ ] Проверить связь UI ↔ ESP32 ↔ Mega (подъём/спуск блока).
```
