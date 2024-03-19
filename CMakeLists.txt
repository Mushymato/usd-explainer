cmake_minimum_required(VERSION 3.12)
project(usd-explainer LANGUAGES CXX)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()
qt_add_executable(
    ${CMAKE_PROJECT_NAME}
    src/Main.cpp
    src/MainWindow.h src/MainWindow.cpp
    src/UsdHighlighter.h src/UsdHighlighter.cpp
    src/UsdHighlighterSyntax.h src/UsdHighlighterSyntax.cpp
    src/UsdTextEdit.h src/UsdTextEdit.cpp
)

set_target_properties(
    ${CMAKE_PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

install(
    TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install/${QT_DEPLOY_BIN_DIR}
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/install/${QT_DEPLOY_LIB_DIR}
)
