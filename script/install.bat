:: ============================================================================
:: File        : install.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-11
:: Description : Window batch file to Dump executable into clean folder
:: Note        : Useful for rapid sharability of project.
:: License     : N/A Open source
::               Copyright (c) 2025 Mario Migliacio
:: ============================================================================

@echo off
setlocal

set ROOT=%~dp0..
cd /d %ROOT%

set CONFIG=release
if /I "%1"=="release" (
    set CONFIG=Release
)

set BIN_DIR=%ROOT%\build\%CONFIG%
set INSTALL_DIR=%ROOT%\install\%CONFIG%
set SFML_BIN=%ROOT%\external\sfml\bin
set ASSETS_DST=%INSTALL_DIR%\assets
set ASSETS_SRC=%ROOT%\assets

if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
    echo [INFO] made dir %INSTALL_DIR%.
)

if not exist "%ASSETS_DST%" (
    mkdir "%ASSETS_DST%"
    echo [INFO] made dir %ASSETS_DST%.
)

echo [INFO] Copying executable and DLLs to install folder.
copy "%BIN_DIR%\CT.exe" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-graphics-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-window-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-system-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-audio-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\freetype.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\openal32.dll" "%INSTALL_DIR%" >nul
copy "%ROOT%\config.json" "%INSTALL_DIR%" >nul

:: ==== Copy Asset Files ====
echo [INFO] Copying assets to install folder.
powershell -Command "New-Item -ItemType Directory -Force -Path '%ASSETS_DST%' | Out-Null; Copy-Item -Path '%ASSETS_SRC%\*' -Destination '%ASSETS_DST%' -Recurse -Force; Get-ChildItem -Path '%ASSETS_DST%' -Recurse | Unblock-File"

echo [INFO] Install folder ready: %INSTALL_DIR%
endlocal