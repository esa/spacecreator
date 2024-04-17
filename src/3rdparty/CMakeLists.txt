include(FetchContent)
FetchContent_Declare(tasteqtwidgets
    GIT_REPOSITORY https://github.com/vikingsoftware-com/TasteQtWidgets.git
    GIT_TAG           main
    SOURCE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/tasteqtwidgets"
)
FetchContent_MakeAvailable(tasteqtwidgets)

add_subdirectory(antlr/cpp_runtime/runtime)
add_subdirectory(qobjectlistmodel)
