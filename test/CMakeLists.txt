enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(CT_tests ${TEST_SOURCES})

target_include_directories(CT_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/sfml/include
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
)

# Link directories for SFML
target_link_directories(CT_tests PRIVATE ${PROJECT_SOURCE_DIR}/external/sfml/lib)

# Select debug/release SFML libs
set(SFML_LIB_SUFFIX $<$<CONFIG:Debug>:-d>)

target_link_libraries(CT_tests
    PRIVATE
    core
    gtest
    gtest_main
    sfml-graphics-d
    sfml-window-d
    sfml-system-d
    sfml-audio-d
    opengl32
    freetype
    winmm
    gdi32
    user32
    advapi32
)

include(GoogleTest)
add_test(NAME RunAllTests COMMAND CT_tests)
