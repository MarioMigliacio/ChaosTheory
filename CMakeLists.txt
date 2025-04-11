cmake_minimum_required(VERSION 3.16)
project(CT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific compiler flags
if (MSVC)
    add_compile_options("/utf-8")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)

# Set the path to SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/sfml")

# Add external dependencies
add_subdirectory(external/googletest)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)