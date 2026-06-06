# RAMS Interactive Hub - Установка на Windows

## Структура файлов

```
C:\RAMS\
├── RAMS-App\              ← Код приложения (из rams-hub-code.zip)
│   ├── app\
│   ├── components\
│   ├── electron\
│   ├── lib\
│   ├── public\
│   └── package.json
│
└── RAMS-Media\            ← Медиа файлы (4.3 GB)
    └── projects\
        ├── 01-rams-beyond-st-regis\
        ├── 02-rams-central\
        └── ... (все проекты)
```

## Шаг 1: Установка Node.js

1. Скачать Node.js LTS: https://nodejs.org/
2. Установить с настройками по умолчанию
3. Проверить: открыть CMD и ввести `node -v`

## Шаг 2: Установка кода приложения

1. Создать папку `C:\RAMS\RAMS-App`
2. Распаковать `rams-hub-code.zip` в эту папку
3. Открыть CMD в папке `C:\RAMS\RAMS-App\rams-interactive-hub`
4. Выполнить:
   ```cmd
   npm install
   ```

## Шаг 3: Копирование медиа файлов

1. Создать папку `C:\RAMS\RAMS-Media`
2. Скопировать папку `projects` в `C:\RAMS\RAMS-Media\`
3. Результат: `C:\RAMS\RAMS-Media\projects\01-rams-beyond-st-regis\...`

## Шаг 4: Запуск приложения

### Режим разработки (с DevTools):
```cmd
cd C:\RAMS\RAMS-App\rams-interactive-hub
npm run electron:dev
```

### Режим киоска (продакшн):
```cmd
cd C:\RAMS\RAMS-App\rams-interactive-hub
npm run build
npm run electron:build
```
Установщик появится в папке `dist\`

## Горячие клавиши

| Клавиши | Действие |
|---------|----------|
| **Ctrl+Shift+A** | Открыть настройки проектов |
| **Ctrl+Q** | Закрыть приложение |
| **Escape** | Закрыть модальное окно |

## Обновление приложения

### Только код (без медиа):
1. Получить новый `rams-hub-code.zip`
2. Удалить `C:\RAMS\RAMS-App\rams-interactive-hub` (кроме node_modules)
3. Распаковать новый архив
4. Перезапустить приложение

### Добавление медиа:
1. Скопировать новые папки проектов в `C:\RAMS\RAMS-Media\projects\`
2. Обновить `lib\data\projects.ts` если нужно добавить новый проект

## Размеры

| Компонент | Размер |
|-----------|--------|
| Код (zip) | 4.1 MB |
| node_modules | ~1.1 GB (устанавливается) |
| Медиа файлы | 4.3 GB |

## Troubleshooting

### Приложение не запускается
- Проверить что Node.js установлен: `node -v`
- Проверить что npm install выполнен без ошибок
- Проверить путь к медиа: `C:\RAMS\RAMS-Media\projects`

### Нет медиа файлов
- Убедиться что путь правильный: `C:\RAMS\RAMS-Media\projects\[project-id]\`
- Проверить права доступа к папке

### Ctrl+Shift+A не работает
- В режиме киоска шорткат работает через Electron
- Попробовать нажать несколько раз
