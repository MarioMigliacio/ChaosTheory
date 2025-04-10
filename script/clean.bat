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

endlocal