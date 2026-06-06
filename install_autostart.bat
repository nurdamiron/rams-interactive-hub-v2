@echo off
:: Устанавливает автозапуск RAMS Interactive Hub при включении Windows
:: Ищет portable exe в папке dist/ и создаёт ярлык в автозагрузке

echo ========================================
echo   RAMS - Установка автозапуска
echo ========================================
echo.

set "STARTUP=%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"
set "SHORTCUT=%STARTUP%\RAMS Interactive Hub.lnk"

:: Ищем portable exe
set "EXE_PATH=%~dp0dist\RAMS Interactive Hub-2.0.0-win-x64.exe"

if not exist "%EXE_PATH%" (
    echo [ERROR] Файл не найден: %EXE_PATH%
    echo Сначала соберите проект: npm run electron:build:win
    echo.
    pause
    exit /b 1
)

:: Создаём ярлык через PowerShell
powershell -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut('%SHORTCUT%'); $s.TargetPath = '%EXE_PATH%'; $s.WorkingDirectory = '%~dp0dist'; $s.WindowStyle = 7; $s.Description = 'RAMS Interactive Hub Kiosk'; $s.Save()"

if exist "%SHORTCUT%" (
    echo [OK] Автозапуск установлен!
    echo EXE: %EXE_PATH%
    echo Ярлык: %SHORTCUT%
    echo.
    echo Приложение будет запускаться при каждом входе в Windows.
    echo Режим: Kiosk (полный экран, без рамки)
) else (
    echo [ERROR] Не удалось создать ярлык.
)

echo.
pause
