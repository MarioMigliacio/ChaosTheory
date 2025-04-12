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

set CONFIG=Debug
if /I "%1"=="release" (
    set CONFIG=Release
)

set BIN_DIR=%ROOT%\build\%CONFIG%
set INSTALL_DIR=%ROOT%\install\%CONFIG%
set SFML_BIN=%ROOT%\external\sfml\bin

if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
)

echo Copying executable and DLLs to install folder...
copy "%BIN_DIR%\CT.exe" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-graphics-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-window-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-system-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\sfml-audio-2.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\freetype.dll" "%INSTALL_DIR%" >nul
copy "%SFML_BIN%\openal32.dll" "%INSTALL_DIR%" >nul

echo Install folder ready: %INSTALL_DIR%
endlocal