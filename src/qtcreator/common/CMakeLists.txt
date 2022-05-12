project(CommonTools VERSION 1.0.0 LANGUAGES CXX)

add_library(CommonTools STATIC)

target_sources(CommonTools
    PRIVATE     modelloader/modelloader.cpp
                modelloader/modelloader.h
                ivtools/ivtools.cpp
                ivtools/ivtools.h
)

target_include_directories(CommonTools
    PUBLIC      .
                ${CMAKE_SOURCE_DIR}/src/qtcreator/common/modelloader
                ${CMAKE_SOURCE_DIR}/src/qtcreator/common/ivtools
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(CommonTools
    PUBLIC      ivcore
    PRIVATE     Asn1Importer
                CommonProjectWarnings
                ConversionCommon
                CsvImporter
                IvXmlImporter
                SedsXmlImporter
                SedsModel
)

add_format_target(CommonTools)
add_tidy_target(CommonTools)
