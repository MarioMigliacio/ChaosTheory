cmake_minimum_required(VERSION 3.16)
project(CT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific compiler flags
if (MSVC)
    add_compile_options("/utf-8")
endif()

# Define SFML_STATIC for static SFML linking
# Removed for debugging environment without need to build --release
# add_definitions(-DSFML_STATIC)

# === Include directories ===
include_directories(include)
include_directories(external/spdlog/include)
include_directories(external/sfml/include)

# === Link directories for SFML static libs ===
link_directories(${CMAKE_SOURCE_DIR}/external/sfml/lib)

# === Main application target ===
file(GLOB_RECURSE CT_SOURCES src/*.cpp)
add_executable(CT ${CT_SOURCES})
target_link_libraries(CT
    sfml-graphics
    sfml-window
    sfml-system
    opengl32
    freetype
    winmm
    gdi32
    user32
    advapi32
)

# === GoogleTest ===
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/googletest/CMakeLists.txt")
    message(FATAL_ERROR "GoogleTest submodule not found! Run 'script/init.bat' to fetch it.")
endif()

add_subdirectory(external/googletest)
enable_testing()
file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
add_executable(CT_tests ${TEST_SOURCES})
target_link_libraries(CT_tests gtest gtest_main)
add_test(NAME AllTests COMMAND CT_tests)