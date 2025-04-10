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