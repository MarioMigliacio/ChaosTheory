# src/core/CMakeLists.txt

file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# for IDE projects
source_group("Header Files" FILES ${CORE_HEADERS})
source_group("Source Files" FILES ${CORE_SRC})

add_library(core STATIC ${CORE_SRC})

target_include_directories(core PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/core/scenes
    ${PROJECT_SOURCE_DIR}/src/core/common
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
