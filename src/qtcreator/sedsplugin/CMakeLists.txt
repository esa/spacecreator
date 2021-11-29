set(PLUGIN_NAME SedsPlugin)

set(SC_PLUGIN_SOURCES
    sedsplugin.cpp
    sedsplugin.h
)

add_library(${PLUGIN_NAME} SHARED)

target_sources(${PLUGIN_NAME} PRIVATE ${SC_PLUGIN_SOURCES})

target_link_libraries(${PLUGIN_NAME} PUBLIC Qt5::Gui)

target_include_directories(${PLUGIN_NAME}
   PUBLIC . ${CMAKE_SOURCE_DIR}/src/libs)
