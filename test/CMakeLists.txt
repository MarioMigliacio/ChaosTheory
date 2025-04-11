# test/CMakeLists.txt

enable_testing()

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(CT_tests ${TEST_SOURCES})

set_target_properties(CT_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
)

target_include_directories(CT_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
        ${PROJECT_SOURCE_DIR}/external/spdlog/include
        ${PROJECT_SOURCE_DIR}/external/sfml/include
)

target_link_directories(CT_tests PRIVATE ${PROJECT_SOURCE_DIR}/external/sfml/lib)

target_link_libraries(CT_tests
    PRIVATE
        gtest
        gtest_main
        core
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        opengl32
        freetype
        winmm
        gdi32
        user32
        advapi32
)

include(GoogleTest)
add_test(NAME RunAllTests COMMAND CT_tests)