project(Asn1ModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES asn1modelbuilder.cpp)
set(INCLUDES asn1modelbuilder.h)

add_library(Asn1ModelBuilder STATIC)
target_sources(Asn1ModelBuilder
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(Asn1ModelBuilder
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(Asn1ModelBuilder
    PUBLIC      asn1library
    PRIVATE     CommonProjectWarnings)

add_format_target(Asn1ModelBuilder)

add_library(ConversionTests::Asn1ModelBuilder ALIAS Asn1ModelBuilder)
