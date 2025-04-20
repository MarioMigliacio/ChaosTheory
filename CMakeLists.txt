# root CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(CT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Optional: disables compiler-specific extensions

# Use unicode output in MSVC builds
if (MSVC)
    add_compile_options("/utf-8")
endif()

# Set output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)

include_directories(${PROJECT_SOURCE_DIR}/external/json/include)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src)
add_subdirectory(external/googletest)
add_subdirectory(test)

# Useful to see in console
message(STATUS "[INFO] Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "[INFO] Build output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")