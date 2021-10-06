project(Asn1Importer VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES importer.cpp)

set(INCLUDES importer.h)

add_library(Asn1Importer STATIC)
target_sources(Asn1Importer
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(Asn1Importer
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(Asn1Importer
    PUBLIC      Conversion::Common
    PRIVATE     asn1library
                CommonProjectWarnings)

add_format_target(Asn1Importer)
add_tidy_target(Asn1Importer)

add_library(Conversion::Asn1Importer ALIAS Asn1Importer)
