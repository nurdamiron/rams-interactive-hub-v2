# BUILD & RELEASE GUIDE

## Что было сделано ✅

### 1. Исправлен маппинг блоков и проектов
- **Проблема**: HYUNDAI, LUKOIL, ORTAU MARRIOTT BC не активировали блоки
- **Решение**: Изменили с индекс-based на ID-based lookup через `gallery-config.ts`
- **Результат**: Теперь несколько проектов могут управлять одним блоком (HAVAL + HYUNDAI + LUKOIL → Block 2)

### 2. Ускорены анимации
- UP: 6s → 2s
- DOWN: 6s → 3s
- LED fade: 3s → 2s

### 3. Добавлена поддержка OTA для ESP32
- Добавлен `#include <ESPmDNS.h>`
- Добавлен `MDNS.begin()` перед `ArduinoOTA.begin()`
- Создан гайд `/firmware/OTA_FIX_GUIDE.md`

### 4. Изменения запушены на GitHub
- Коммит: `71ace9f` - "Fix project-to-block mapping + Add OTA support"
- Repository: https://github.com/nurdamiron/rams-projects

## Автоматический билд (GitHub Actions) 🤖

### Windows Build
GitHub Actions автоматически запустит билд после пуша на main ветку.

**Workflow файл**: `.github/workflows/build-windows.yml`

**Что билдится**:
- Setup Installer (NSIS) - `RAMS Interactive Hub-2.0.0-win-x64.exe`
- Portable версия - `RAMS Interactive Hub-2.0.0-win-x64-Portable.exe`

**Где найти билды**:
1. Открой: https://github.com/nurdamiron/rams-projects/actions
2. Найди workflow run "Build Windows Release"
3. Скачай артефакты:
   - `Windows-Installer`
   - `Windows-Portable`

### Проверка статуса билда

```bash
# Открой в браузере
open "https://github.com/nurdamiron/rams-projects/actions"

# Или через gh CLI
gh run list --repo nurdamiron/rams-projects
gh run view <run-id> --repo nurdamiron/rams-projects
```

## Ручной билд (если нужен)

### Windows (на Mac через CI)
Уже настроено в GitHub Actions - просто запуши в main

### Mac (локально)
```bash
cd /Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub/rams-interactive-hub

# Build для Mac
npm run electron:build:mac

# Результат в dist/
# RAMS Interactive Hub-2.0.0-mac-universal.dmg
```

### Все платформы сразу
```bash
npm run electron:build:all
```

## Создание релиза с тэгом 📦

Чтобы создать официальный релиз на GitHub:

```bash
# 1. Создай git tag
cd /Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub
git tag -a v2.0.0 -m "Release v2.0.0 - Fixed block mapping + OTA support"

# 2. Запуши тэг
git push origin v2.0.0

# 3. GitHub Actions автоматически создаст Release на GitHub
# с билдами для Windows
```

После этого на https://github.com/nurdamiron/rams-projects/releases появится новый релиз с:
- Setup installer
- Portable версия
- latest.yml (для auto-update)
- Release notes (сгенерированные автоматически)

## Файлы в билде

```
dist/
├── RAMS Interactive Hub-2.0.0-win-x64 Setup.exe  # Installer
├── RAMS Interactive Hub-2.0.0-win-x64-Portable.exe  # Portable
├── latest.yml  # Auto-update metadata
└── builder-debug.yml  # Debug info
```

## Обновление версии

Перед билдом обнови версию в `package.json`:

```json
{
  "version": "2.0.0"  // <- измени здесь
}
```

## Структура проекта после билда

```
📦 RAMS Interactive Hub-2.0.0-win-x64 Setup.exe
   └── Устанавливается в C:\Program Files\RAMS Interactive Hub\
       ├── RAMS Interactive Hub.exe
       ├── resources\
       │   ├── app.asar  (Next.js app)
       │   └── media\
       │       └── projects\  (видео + изображения проектов)
       └── Uninstall.exe
```

## Проверка билда

После скачивания билда из GitHub Actions:

1. **Проверь размер файлов**:
   - Installer: ~50-200 MB (зависит от медиа)
   - Portable: ~50-200 MB

2. **Запусти на Windows**:
   ```
   # Setup installer
   .\RAMS Interactive Hub-2.0.0-win-x64 Setup.exe

   # Или portable
   .\RAMS Interactive Hub-2.0.0-win-x64-Portable.exe
   ```

3. **Проверь функциональность**:
   - Запускается ли приложение
   - Работает ли навигация по проектам
   - Работает ли маппинг блоков (Admin Panel - Ctrl+Shift+A)
   - Подключение к ESP32 (если доступен)

## Текущий IP ESP32

Приложение сейчас настроено на:
- **IP**: `192.168.110.65` (в `.env.local`, не попадет в билд)
- **Hostname**: `RAMS-ESP32.local`

⚠️ **Важно**: После билда приложение будет пытаться подключиться к `192.168.4.1` (дефолт из `.env.production`).

Чтобы изменить IP в билде, обнови `rams-interactive-hub/.env.production`:
```env
NEXT_PUBLIC_ESP32_HOST=192.168.110.65
NEXT_PUBLIC_ESP32_PORT=80
```

## Troubleshooting

### Build failed на GitHub Actions
- Проверь логи: https://github.com/nurdamiron/rams-projects/actions
- Частые проблемы:
  - Не хватает `package-lock.json`
  - Ошибки в `package.json`
  - Недостаточно места на GitHub runner

### Билд работает но размер слишком большой
Проверь что медиа файлы правильно игнорятся в `.gitignore`:
```
public/projects/*/videos/
public/projects/*/images/
```

### Auto-update не работает
Убедись что:
1. `latest.yml` включен в релиз
2. В `package.json` настроен `publish`
3. Версия в `package.json` обновлена

## Next Steps

1. ✅ Запушено на GitHub
2. ⏳ Дождись окончания GitHub Actions билда (~10-15 минут)
3. 📥 Скачай билды из Artifacts
4. 🧪 Протестируй на Windows
5. 🏷️ Создай релиз с тэгом (если все ОК)
6. 🚀 Установи на продакшн машину

---

**Текущий статус**: Изменения запушены, GitHub Actions запущен ✅

Проверить статус: https://github.com/nurdamiron/rams-projects/actions
