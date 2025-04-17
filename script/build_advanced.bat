:: ============================================================================
:: File        : build_advanced.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-14
:: Description : Advanced Windows batch build script for CT engine
:: Options     :
::     /ct      - build main CT application only
::     /test    - build test suite only
::     /all     - build both CT and test suite
::     /clean   - clean build directory
:: ============================================================================
@echo off
setlocal ENABLEDELAYEDEXPANSION

:: ===============================
:: ChaosTheory Advanced Build Script
:: Supports: /clean /ct /tests /all [/debug or /release]
:: ===============================

set CONFIG=Debug
set BUILD_CT=false
set BUILD_TESTS=false
set CLEAN=false

:: ==== Parse Arguments ====
for %%A in (%*) do (
    if /I "%%A"=="/release" set CONFIG=Release
    if /I "%%A"=="/debug" set CONFIG=Debug
    if /I "%%A"=="/ct" set BUILD_CT=true
    if /I "%%A"=="/test" set BUILD_TESTS=true
    if /I "%%A"=="/all" (
        set BUILD_CT=true
        set BUILD_TESTS=true
    )
    if /I "%%A"=="/clean" set CLEAN=true
)

:: ==== Paths ====
set ROOT=%~dp0..
set BUILD_DIR=%ROOT%\build
set INSTALL_DIR=%ROOT%\install
set LOG_DIR=%ROOT%\log
set TARGET_DIR=%BUILD_DIR%\%CONFIG%
set SFML_BIN=%ROOT%\external\sfml\bin
set ASSETS_SRC=%ROOT%\assets
set ASSETS_DST=%TARGET_DIR%\assets

:: ==== Clean ====
if "%CLEAN%"=="true" (
    echo %GREEN%[INFO]%RESET% Cleaning build directory...
    rmdir /s /q "%BUILD_DIR%"
    echo [INFO] Cleaning install directory...
    rmdir /s /q "%INSTALL_DIR%"
    echo [INFO] Cleaning log directory...
    rmdir /s /q "%LOG_DIR%"
)

:: ==== Configure ====
if not exist "%BUILD_DIR%" (
    echo [INFO] Creating build directory...
    mkdir "%BUILD_DIR%"
)

cd /d "%BUILD_DIR%"
echo [INFO] Configuring CMake...
cmake -DCMAKE_BUILD_TYPE=%CONFIG% ..

:: ==== Build Targets ====
if "%BUILD_CT%"=="true" (
    echo [INFO] Building CT.exe...
    cmake --build . --config %CONFIG% --target CT
)

if "%BUILD_TESTS%"=="true" (
    echo [INFO] Building CT_tests.exe...
    cmake --build . --config %CONFIG% --target CT_tests
)

:: ==== Copy SFML DLLs ====
echo [INFO] Copying SFML DLLs to %TARGET_DIR%...
xcopy /Y /Q "%SFML_BIN%\*.dll" "%TARGET_DIR%\" >nul

:: ==== Copy Asset Files ====
echo [INFO] Copying assets to %ASSETS_DST%...
powershell -Command "New-Item -ItemType Directory -Force -Path '%ASSETS_DST%' | Out-Null; Copy-Item -Path '%ASSETS_SRC%\*' -Destination '%ASSETS_DST%' -Recurse -Force; Get-ChildItem -Path '%ASSETS_DST%' -Recurse | Unblock-File"

:: ==== Done ====
cd /d "%ROOT%"
echo [INFO] Build completed. Output in %TARGET_DIR%
endlocal
