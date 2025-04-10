@echo off
setlocal

set ROOT=%~dp0..
cd /d %ROOT%

if exist build\Debug\CT.exe (
    start "" build\Debug\CT.exe
) else (
    echo Executable not found at build\Debug\CT.exe
    echo Try running: script\build.bat
)

endlocal
