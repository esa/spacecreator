project(TmcConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    converter.cpp)

set(INCLUDES
    converter.h)

add_library(TmcConverter STATIC)
target_sources(TmcConverter
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(TmcConverter
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(TmcConverter
    PUBLIC     Conversion::Common
               Conversion::Converter
               Conversion::Registry
               Conversion::IvRegistrar
               Conversion::Asn1Registrar
               Conversion::PromelaRegistrar
               Tmc::SdlToPromelaConverter
    PRIVATE    CommonProjectWarnings
               Tmc::InterfaceViewOptimizer)

add_format_target(TmcConverter)
add_tidy_target(TmcConverter)

add_library(Tmc::Converter ALIAS TmcConverter)
