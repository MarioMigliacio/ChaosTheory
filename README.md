# ChaosTheory (CT)

ChaosTheory (CT) is a modular C++ game framework built using SFML, spdlog, and GoogleTest.
It provides a clean foundation for building interactive and audio-enabled games on Windows, with full (in the works) support for static or dynamic builds, test automation, and packaging.

---

## Features

-   Written in modern C++20
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

The new preferred build option is to use .\build_advanced.bat
with optional flags like:
        /ct      - build main CT application only
        /test    - build test suite only
        /all     - build both CT and test suite
        /clean   - clean build directory
    ex: .\build_advanced.bat /clean /all
- clears any build artifacts, then builds target for debug(default) Test AND CT executable.
```

### Run tests

```
*Support included for agnostic script execution location*

*Assumes you have ran the build for test target at least.*

script\test.bat

or

cd script
.\test.bat
```

### Run the game

```
*Support included for agnostic script execution location*

*Assumes you have ran the build for CT target at least.*

script\run.bat

or

cd script
.\run.bat
```

### Debugging the application

```
In the .vscode folder is a launch.json file that is configured by default to allow for F5 running in vs code. I have configured the preLaunchTask to be the appropriate building action for the launch. Just change the target under 'Run and Debug' tab on the left side of vs code, and either click the run manually or hit F5! Sprinkle in breakpoints in desired code.
```

### Project Structure

```
CT/
├── .vscode/          → launch and task configs for VS Code
├── assets/           → sfml asset files, audio/font/image
├── build/            → *[optional]* build output (CMake-generated)
├── external/         → git submodules (SFML, spdlog, googletest)
|                       [SFML and spdlog are hard copy dlls]
├── install/          → *[optional]* final game bundle output
├── log/              → *[optional]* upon execution, default log file trace output
├── script/           → build/clean/init/install/run/test batch files
├── src/              → main game logic
├────core/            → CT essential library core functionality
├──────common/        → CT common utility
├──────scenes/        → CT scene logic handling
├──────ui/            → CT ui elements
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
