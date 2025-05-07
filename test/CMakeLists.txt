# test/CMakeLists.txt

enable_testing()

# More explicit instead of file glob
add_executable(CT_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/AssetManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIButtonTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LogManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Main_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UISelectableButtonTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneFactoryTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneTransitionManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingsManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIFactoryTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIManagerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UISliderTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WindowManagerTest.cpp
    # add others here if needed
)

# Select debug/release SFML libs
set(SFML_LIB_SUFFIX $<$<CONFIG:Debug>:-d>)

target_include_directories(CT_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/test
    ${PROJECT_SOURCE_DIR}/test/common
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/sfml/include
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
)

# Link directories for SFML
target_link_directories(CT_tests PRIVATE ${PROJECT_SOURCE_DIR}/external/sfml/lib)

target_link_libraries(CT_tests
    PRIVATE
    core
    gtest
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

add_test(NAME RunAllTests COMMAND CT_tests)
