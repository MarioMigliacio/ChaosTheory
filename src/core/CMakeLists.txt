# src/core/CMakeLists.txt

file(GLOB_RECURSE CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(core STATIC ${CORE_SOURCES})

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/external/spdlog/include
        ${PROJECT_SOURCE_DIR}/external/sfml/include
)

target_link_directories(core PRIVATE ${PROJECT_SOURCE_DIR}/external/sfml/lib)

target_link_libraries(core
    PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)