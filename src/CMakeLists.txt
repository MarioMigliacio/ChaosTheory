set(SFML_LIB_SUFFIX $<$<CONFIG:Debug>:-d>)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)

add_executable(CT ${SRC_FILES})

target_link_directories(CT PRIVATE ${PROJECT_SOURCE_DIR}/external/sfml/lib)

target_include_directories(CT PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/core/scenes
    ${PROJECT_SOURCE_DIR}/src/core/common
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/sfml/include
)

target_link_libraries(CT
    PRIVATE
    core
    sfml-graphics${SFML_LIB_SUFFIX}
    sfml-window${SFML_LIB_SUFFIX}
    sfml-system${SFML_LIB_SUFFIX}
    sfml-audio${SFML_LIB_SUFFIX}
    opengl32
    freetype
    winmm
    gdi32
    user32
    advapi32
)
