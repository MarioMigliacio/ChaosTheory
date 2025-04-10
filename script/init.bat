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