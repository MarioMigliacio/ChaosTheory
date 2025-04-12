::=============================================================================
:: File        : Build.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-11
:: Description : window batch file to build everything.
:: Note        : Run init.bat prior to this file. Clean prior to build for 
::               consistant results.
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

if not exist build (
    mkdir build
)

cd build
cmake -DCMAKE_BUILD_TYPE=%CONFIG% ..
cmake --build . --config %CONFIG%
cd ..

REM ==== Copy SFML DLLs into build output folder ====
set TARGET_DIR=%ROOT%\build\%CONFIG%
set SFML_BIN=%ROOT%\external\sfml\bin

echo Copying SFML DLLs to %TARGET_DIR%...
copy "%SFML_BIN%\sfml-graphics-2.dll" "%TARGET_DIR%" >nul
copy "%SFML_BIN%\sfml-window-2.dll" "%TARGET_DIR%" >nul
copy "%SFML_BIN%\sfml-system-2.dll" "%TARGET_DIR%" >nul
copy "%SFML_BIN%\sfml-audio-2.dll" "%TARGET_DIR%" >nul
copy "%SFML_BIN%\freetype.dll" "%TARGET_DIR%" >nul
copy "%SFML_BIN%\openal32.dll" "%TARGET_DIR%" >nul


echo Build complete. Run your app from %TARGET_DIR%\CT.exe

endlocal