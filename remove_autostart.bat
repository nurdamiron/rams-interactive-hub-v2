@echo off
:: Удаляет автозапуск RAMS Interactive Hub

set "SHORTCUT=%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\RAMS Interactive Hub.lnk"

if exist "%SHORTCUT%" (
    del "%SHORTCUT%"
    echo [OK] Автозапуск удалён.
) else (
    echo Автозапуск не был установлен.
)

pause
