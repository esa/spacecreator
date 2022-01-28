project(CsvImporter VERSION 1.0.0 LANGUAGES CXX)

add_library(CsvImporter STATIC)

target_sources(CsvImporter
    PRIVATE     csvimporter.cpp
                csvimporter.h
                importerexception.cpp
                importerexception.h
                ../CsvOptions/options.cpp
                ../CsvOptions/options.h
)

target_include_directories(CsvImporter
    PUBLIC      .
    PRIVATE     ..
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(CsvImporter
    PUBLIC      CsvModel
                Qt${QT_VERSION_MAJOR}::Core
    PRIVATE     CommonProjectWarnings
)

add_format_target(CsvImporter)
add_tidy_target(CsvImporter)
