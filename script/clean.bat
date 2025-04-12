:: ============================================================================
:: File        : clean.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-11
:: Description : Window batch file to remove any existing build artifacts.
:: Note        : Removes any existing  build/ install/ or log/ directories.
:: License     : N/A Open source
::               Copyright (c) 2025 Mario Migliacio
:: ============================================================================

@echo off
setlocal

set ROOT=%~dp0..
cd /d %ROOT%

if exist build (
    rmdir /s /q build
    echo Build folder removed.
) else (
    echo No build folder found.
)

if exist install (
    rmdir /s /q install
    echo Install folder removed.
) else (
    echo No install folder found.
)

if exist log (
    rmdir /s /q log
    echo Log folder removed.
) else (
    echo No log folder found.
)

endlocal