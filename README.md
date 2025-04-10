# ChaosTheory (CT)

ChaosTheory (CT) is a modular C++ game framework built using SFML, spdlog, and GoogleTest.
It provides a clean foundation for building interactive and audio-enabled games on Windows, with full (in the works) support for static or dynamic builds, test automation, and packaging.

---

## Features

-   Written in modern C++17
-   Uses [SFML](https://www.sfml-dev.org/) for graphics, input, and audio
-   Unit tested with [GoogleTest](https://github.com/google/googletest)
-   Logging with [spdlog](https://github.com/gabime/spdlog)
-   Batch scripts for build, test, run, and install
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
script\init.bat
```

### Build the project

```
script\build.bat           :: builds Debug
script\build.bat release   :: builds Release
```

### Run tests

```
script\test.bat
```

### Run the game

```
script\run.bat
```

### Project Structure

```
CT/
├── .vscode/          → launch and task configs for VS Code
├── build/            → build output (CMake-generated)
├── external/         → git submodules (SFML, spdlog, googletest)
|                      [SFML and spdlog are hard copy dlls]
├── include/          → project headers
├── install/          → final game bundle output
├── script/           → build/test/run/install batch files
├── src/              → main game logic
├── test/             → unit tests
├── CMakeLists.txt
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
