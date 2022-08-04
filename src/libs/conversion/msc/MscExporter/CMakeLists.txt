project(MscExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exporter.cpp)
set(INCLUDES exporter.h)

add_library(MscExporter STATIC)
target_sources(MscExporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(MscExporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(MscExporter
    PUBLIC      Conversion::Common
    PRIVATE     msccore
                CommonProjectWarnings)

add_format_target(MscExporter)
add_tidy_target(MscExporter)

add_library(MSC::Exporter ALIAS MscExporter)
