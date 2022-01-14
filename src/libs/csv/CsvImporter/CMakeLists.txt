project(CsvImporter VERSION 1.0.0 LANGUAGES CXX)

add_library(CsvImporter STATIC)

target_sources(CsvImporter
    PRIVATE     csvimporter.cpp
                csvimporter.h
)

target_include_directories(CsvImporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
    PRIVATE     ..
)

target_link_libraries(CsvImporter
    PUBLIC      Conversion::Common
                CsvModel
    PRIVATE     CommonProjectWarnings)

add_format_target(CsvImporter)
add_tidy_target(CsvImporter)
