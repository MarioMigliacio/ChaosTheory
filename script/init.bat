:: ============================================================================
:: File        : init.bat
:: Project     : ChaosTheory (CT)
:: Author      : Mario Migliacio
:: Created     : 2025-04-11
:: Description : Window batch file to Pull required googletest submodule
:: Note        : This is a requirement in order to utilize tests for project
::               Run this prior to cleaning, building, or running.
:: License     : N/A Open source
::               Copyright (c) 2025 Mario Migliacio
:: ============================================================================

@echo off
setlocal

set ROOT=%~dp0..
cd /d %ROOT%

echo Adding googletest submodule...
git submodule add https://github.com/google/googletest.git external/googletest

echo Initializing git submodules...
git submodule update --init --recursive

echo Done.
endlocal