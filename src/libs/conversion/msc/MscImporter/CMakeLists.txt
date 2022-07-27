project(MscImporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES importer.cpp)

set(INCLUDES importer.h)

add_library(MscImporter STATIC)
target_sources(MscImporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(MscImporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(MscImporter
    PUBLIC      Conversion::Common
                msccore
    PRIVATE     CommonProjectWarnings)

add_format_target(MscImporter)
add_tidy_target(MscImporter)

add_library(MSC::Importer ALIAS MscImporter)
