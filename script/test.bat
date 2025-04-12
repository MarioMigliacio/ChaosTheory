:: ============================================================================
:: File        : test.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-11
:: Description : Window batch file to run existing CT_Test executable file
:: Note        : Build a clean solution prior to running this script.
:: License     : N/A Open source
::               Copyright (c) 2025 Mario Migliacio
:: ============================================================================

@echo off
setlocal

set ROOT=%~dp0..
cd /d %ROOT%

if exist build\Debug\CT_tests.exe (
    build\Debug\CT_tests.exe
) else (
    echo Test executable not found at build\Debug\CT_tests.exe
    echo Try running: script\build.bat
)

endlocal