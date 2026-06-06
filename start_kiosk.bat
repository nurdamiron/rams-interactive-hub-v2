@echo off
:: RAMS Interactive Hub - Kiosk Startup Script (Windows)
:: Для автозапуска: поместите ярлык этого файла в shell:startup
:: Или запустите install_autostart.bat

title RAMS Interactive Hub
echo ========================================
echo   RAMS Interactive Hub - Starting...
echo ========================================
echo.

:: Автоопределение директории (там где лежит .bat)
cd /d "%~dp0"

:: Проверка Node.js
where node >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Node.js не найден!
    echo Установите Node.js LTS: https://nodejs.org/
    timeout /t 10
    exit /b 1
)

echo [OK] Node.js найден
echo.

:: Ожидание сети (ESP32 может быть ещё не готов)
echo [WAIT] Ожидание сети (5 сек)...
timeout /t 5 /nobreak >nul

:start_app
echo [START] Запуск приложения...
npm run electron:dev

:: Если приложение упало — перезапуск через 5 сек
echo.
echo [RESTART] Приложение завершилось. Перезапуск через 5 сек...
echo Нажмите Ctrl+C для отмены.
timeout /t 5
goto start_app
