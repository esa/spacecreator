project(Asn1AcnExporter VERSION 1.0.0 LANGUAGES CXX)

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

add_library(Asn1AcnExporter STATIC)
target_sources(Asn1AcnExporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(Asn1AcnExporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(Asn1AcnExporter
    PUBLIC      Conversion::Common
                asn1library
    PRIVATE     CommonProjectWarnings)

add_format_target(Asn1AcnExporter)
add_tidy_target(Asn1AcnExporter)

add_library(Conversion::Asn1AcnExporter ALIAS Asn1AcnExporter)
