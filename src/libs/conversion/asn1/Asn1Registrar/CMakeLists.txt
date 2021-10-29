project(Asn1Registrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(Asn1Registrar STATIC)
target_sources(Asn1Registrar
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(Asn1Registrar
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(Asn1Registrar
    PUBLIC      Conversion::Common
                ASN1::Exporter
                ASN1::Importer
                Conversion::SedsToAsn1Translator
    PRIVATE     CommonProjectWarnings)

add_format_target(Asn1Registrar)
add_tidy_target(Asn1Registrar)

add_library(Conversion::Asn1Registrar ALIAS Asn1Registrar)
