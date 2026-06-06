# 🐳 СБОРКА ЧЕРЕЗ DOCKER НА MAC

## Установка Docker

```bash
# Скачай Docker Desktop для Mac:
# https://www.docker.com/products/docker-desktop/

# Или через Homebrew:
brew install --cask docker
```

## Сборка Windows .exe

```bash
cd /Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub

# Собери через Docker (автоматически скачает Windows окружение)
docker run --rm -ti \
  --env ELECTRON_CACHE="/root/.cache/electron" \
  --env ELECTRON_BUILDER_CACHE="/root/.cache/electron-builder" \
  -v $(pwd):/project \
  -v ~/.cache/electron:/root/.cache/electron \
  -v ~/.cache/electron-builder:/root/.cache/electron-builder \
  electronuserland/builder:wine \
  /bin/bash -c "cd /project && npm install && npm run build && npm run electron:build"
```

⏱️ **Первый раз:** 20-30 минут (скачает образ)
⏱️ **Следующие разы:** 5-10 минут

## Готовые файлы

```
dist/
├── RAMS Interactive Hub Setup 0.1.0.exe
└── RAMS-Interactive-Hub-Portable.exe
```

## Плюсы
- ✅ Работает на Mac
- ✅ Не нужен Windows
- ✅ Повторяемая сборка

## Минусы
- ❌ Нужен Docker (~500 MB)
- ❌ Долго первый раз
