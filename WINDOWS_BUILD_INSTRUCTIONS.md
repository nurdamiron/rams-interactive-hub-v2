зап# Инструкция: Сборка Windows .exe

GitHub Actions не работает из-за лимита минут. Вот как собрать билд вручную.

---

## ✅ Способ 1: На Windows компьютере (РЕКОМЕНДУЮ)

### Требования:
- Windows 10/11
- Node.js 20+ ([скачать](https://nodejs.org/))
- Git ([скачать](https://git-scm.com/))

### Шаги:

1. **Открой PowerShell или CMD**

2. **Склонируй репозиторий:**
```bash
git clone https://github.com/nurdamiron/rams-projects.git
cd rams-projects\rams-interactive-hub
```

3. **Установи зависимости:**
```bash
npm install
```

4. **Собери Windows .exe:**
```bash
npm run electron:build:win
```

5. **Готово!** Файлы будут в папке `dist/`:
   - `RAMS Interactive Hub-2.0.0-win-x64 Setup.exe` (установщик)
   - `win-unpacked/` (portable версия)

---

## ✅ Способ 2: Скачать с предыдущего билда

Если у тебя был успешный билд раньше, можешь скачать его:

1. Открой: https://github.com/nurdamiron/rams-projects/actions
2. Найди **зеленый** (успешный) билд
3. Скачай артефакт `RAMS-Interactive-Hub-Windows.zip`

⚠️ **Важно**: Старый билд НЕ включает новую панель управления!

---

## ✅ Способ 3: Использовать CircleCI (бесплатно)

### 1. Создай файл `.circleci/config.yml`:

```yaml
version: 2.1

orbs:
  win: circleci/windows@5.0

jobs:
  build-windows:
    executor:
      name: win/default
      size: large
    working_directory: ~/rams-projects/rams-interactive-hub

    steps:
      - checkout:
          path: ~/rams-projects

      - run:
          name: Install Node.js
          command: |
            choco install nodejs-lts --version=20.11.0 -y

      - run:
          name: Install dependencies
          command: npm ci

      - run:
          name: Build Electron app
          command: npm run electron:build:win

      - store_artifacts:
          path: dist/RAMS-Interactive-Hub-win-x64.zip

workflows:
  build:
    jobs:
      - build-windows
```

### 2. Зарегистрируйся на CircleCI:
1. Иди на https://circleci.com/
2. Sign up with GitHub
3. Выбери репозиторий `rams-projects`
4. Билд запустится автоматически

---

## ✅ Способ 4: Использовать AppVeyor (бесплатно для open source)

### 1. Создай файл `appveyor.yml`:

```yaml
version: 2.0.{build}
image: Visual Studio 2022

install:
  - ps: Install-Product node 20
  - cd rams-interactive-hub
  - npm ci

build_script:
  - npm run electron:build:win

artifacts:
  - path: rams-interactive-hub\dist\*.exe
    name: WindowsInstaller
  - path: rams-interactive-hub\dist\win-unpacked\**\*
    name: WindowsPortable

deploy: off
```

### 2. Зарегистрируйся на AppVeyor:
1. Иди на https://www.appveyor.com/
2. Sign up with GitHub
3. Добавь проект `rams-projects`
4. Билд запустится автоматически

---

## 📋 Что включает новый билд

Новый билд (после коммита `059a567`) включает:

✨ **Control Panel** - новая панель управления:
- 🎛️ Статистика блоков (X/15 активно)
- 🟢 Визуальная сетка 15 блоков
- 🚨 Emergency Stop
- ⬇️ Опустить все блоки
- ⚙️ Панель управления актуаторами
- 🔄 Переподключение ESP32
- ⚙️ Админ панель

🐛 **Исправления**:
- ⚡ Время подъема: 5s → **2s**
- ⚡ LED fade: 3s → **2s**

---

## 🆘 Если что-то не работает

1. **Ошибка "npm not found"**:
   → Установи Node.js: https://nodejs.org/

2. **Ошибка "git not found"**:
   → Установи Git: https://git-scm.com/

3. **Ошибка при сборке**:
   → Удали `node_modules` и `package-lock.json`, потом `npm install` заново

4. **Нужна помощь**:
   → Напиши мне, помогу!

---

## 💡 Рекомендация

**Лучший вариант** = Способ 1 (сборка на Windows компьютере)
- Самый быстрый (5-10 минут)
- Самый надежный
- Локальный билд без зависимости от CI/CD

Если нет Windows машины → используй **CircleCI** (Способ 3)

---

**Создано**: 2026-02-19
**Коммит**: 059a567
