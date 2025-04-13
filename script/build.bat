::=============================================================================
:: File        : build.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-11
:: Description : Batch script to configure and build CT with asset handling
:: License     : Open Source (c) 2025 Mario Migliacio
::=============================================================================

@echo off
setlocal

set ROOT=%~dp0..
cd /d %ROOT%

set CONFIG=Debug
if /I "%1"=="release" (
    set CONFIG=Release
)

echo [INFO] Configuration set to %CONFIG%

:: Create build directory if needed
if not exist build (
    echo [INFO] Creating build directory...
    mkdir build
)

:: CMake configure and build
cd build
cmake -DCMAKE_BUILD_TYPE=%CONFIG% ..
cmake --build . --config %CONFIG%
cd ..

:: Set target output and SFML binary locations
set TARGET_DIR=%ROOT%\build\%CONFIG%
set SFML_BIN=%ROOT%\external\sfml\bin

:: Copy ALL SFML DLLs to build output
echo [INFO] Copying SFML DLLs to %TARGET_DIR%...
xcopy /Y /E /I "%SFML_BIN%\*.dll" "%TARGET_DIR%\" >nul

:: === Copy all asset folders (audio, fonts, sprites, etc.) ===
echo [INFO] Copying all assets to %TARGET_DIR%\assets...
powershell -NoProfile -Command "Get-ChildItem -Path '%ROOT%\assets' -Directory | ForEach-Object { $src = $_.FullName; $dest = Join-Path '%TARGET_DIR%\assets' $_.Name; Copy-Item -Path $src -Destination $dest -Recurse -Force; Get-ChildItem -Path $dest -Recurse | Unblock-File }"

echo [SUCCESS] Build complete. Run your app from:
echo           %TARGET_DIR%\CT.exe

endlocal
