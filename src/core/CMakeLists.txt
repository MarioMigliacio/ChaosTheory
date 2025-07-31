# src/core/CMakeLists.txt

file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(core STATIC ${CORE_SRC})

target_precompile_headers(core PRIVATE ${PROJECT_SOURCE_DIR}/src/core/pch.h)

target_include_directories(core PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/core/common
    ${PROJECT_SOURCE_DIR}/src/core/guns
    ${PROJECT_SOURCE_DIR}/src/core/managers
    ${PROJECT_SOURCE_DIR}/src/core/scenes
    ${PROJECT_SOURCE_DIR}/src/core/ships
    ${PROJECT_SOURCE_DIR}/src/core/ui
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/sfml/include
)

target_link_libraries(core
    sfml-graphics${SFML_LIB_SUFFIX}
    sfml-window${SFML_LIB_SUFFIX}
    sfml-system${SFML_LIB_SUFFIX}
    sfml-audio${SFML_LIB_SUFFIX}
)
