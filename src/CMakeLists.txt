# src/CMakeLists.txt

add_subdirectory(core)

file(GLOB_RECURSE CT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(CT ${CT_SOURCES})

set_target_properties(CT PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
)

target_include_directories(CT PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/external/sfml/include
)

target_link_directories(CT PRIVATE ${PROJECT_SOURCE_DIR}/external/sfml/lib)

target_link_libraries(CT
    PRIVATE
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
