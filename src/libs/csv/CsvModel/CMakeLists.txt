project(CsvModel VERSION 1.0.0 LANGUAGES CXX)

add_library(CsvModel STATIC)

target_sources(CsvModel
    PRIVATE csvmodel.cpp
            csvmodel.h
            row.cpp
            row.h
)

target_include_directories(CsvModel
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(CsvModel
    PUBLIC      Conversion::Common
    PRIVATE     CommonProjectWarnings
)

add_format_target(CsvModel)
add_tidy_target(CsvModel)
