project(PluginCommon VERSION 1.0.0 LANGUAGES CXX)

add_library(PluginCommon STATIC)

target_sources(PluginCommon
    PRIVATE     modelloader/modelloader.cpp
                modelloader/modelloader.h
                ivtools/ivtools.cpp
                ivtools/ivtools.h
)

target_include_directories(PluginCommon
    PUBLIC      .
                ${CMAKE_SOURCE_DIR}/src/qtcreator/common/modelloader
                ${CMAKE_SOURCE_DIR}/src/qtcreator/common/ivtools
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(PluginCommon
    PRIVATE     ConversionCommon
                CommonProjectWarnings
                IvXmlImporter
                SedsXmlImporter
                ivcore
                SedsModel
)

add_format_target(PluginCommon)
add_tidy_target(PluginCommon)
