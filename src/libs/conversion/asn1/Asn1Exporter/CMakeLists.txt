project(Asn1Exporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exporter.cpp
            visitors/acnnodereconstructingvisitor.cpp
            visitors/acntypecomponentreconstructingvisitor.cpp
            visitors/asn1nodereconstructingvisitor.cpp
            visitors/asn1typecomponentreconstructingvisitor.cpp
            visitors/typeconstraintsreconstructingvisitor.cpp)
set(INCLUDES exporter.h
             visitors/acnnodereconstructingvisitor.h
             visitors/acntypecomponentreconstructingvisitor.h
             visitors/asn1nodereconstructingvisitor.h
             visitors/asn1typecomponentreconstructingvisitor.h
             visitors/typeconstraintsreconstructingvisitor.h)

add_library(Asn1Exporter STATIC)
target_sources(Asn1Exporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(Asn1Exporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(Asn1Exporter
    PUBLIC      Conversion::Common
                asn1library
    PRIVATE     CommonProjectWarnings)

add_format_target(Asn1Exporter)
add_tidy_target(Asn1Exporter)

add_library(ASN1::Exporter ALIAS Asn1Exporter)
