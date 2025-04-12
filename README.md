# ChaosTheory (CT)

ChaosTheory (CT) is a modular C++ game framework built using SFML, spdlog, and GoogleTest.
It provides a clean foundation for building interactive and audio-enabled games on Windows, with full (in the works) support for static or dynamic builds, test automation, and packaging.

---

## Features

-   Written in modern C++17
-   Uses [SFML](https://www.sfml-dev.org/) for graphics, input, and audio
-   Unit tested with [GoogleTest](https://github.com/google/googletest)
-   Logging with [spdlog](https://github.com/gabime/spdlog)
-   Batch scripts for init, build, clean, test, run, and install
-   Full [VS Code](https://code.visualstudio.com/) integration

---

## Quick Start

### Prerequisites

-   Git
-   CMake ≥ 3.16
-   Visual Studio (x64 support)
-   SFML **64-bit binaries**

---

### First-Time Setup

```bash
git clone --recurse-submodules https://github.com/MarioMigliacio/chaostheory.git
cd chaostheory

*Support included for agnostic script execution location*
.\script\init.bat

or

cd script
.\init.bat
```

### Build the project

```
*Support included for agnostic script execution location*

.\script\build.bat           :: builds Debug
.\script\build.bat release   :: builds Release

or

cd script
.\build.bat                  :: builds Debug
.\build.bat release          :: builds Release

[CLEAN] options included:
.\script\clean.bat           :: removes build, log, and install artifacts
```

### Run tests

```
*Support included for agnostic script execution location*

script\test.bat

or

cd script
.\test.bat
```

### Run the game

```
*Support included for agnostic script execution location*

script\run.bat

or

cd script
.\run.bat
```

### Project Structure

```
CT/
├── .vscode/          → launch and task configs for VS Code
├── build/            → *[optional]* build output (CMake-generated)
├── external/         → git submodules (SFML, spdlog, googletest)
|                       [SFML and spdlog are hard copy dlls]
├── install/          → *[optional]* final game bundle output
├── log/              → *[optional]* upon execution, default log file trace output
├── script/           → build/clean/init/install/run/test batch files
├── src/              → main game logic
├────core/            → CT essential library core functionality
├── test/             → unit tests
└── README.md
```

### Notes

```
Batch file support only for the time being
```

### License

```
Free to do as you please with the code, but an aknowledgement would be appreciated.
```
