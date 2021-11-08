project(SdlRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(SdlRegistrar STATIC)
target_sources(SdlRegistrar
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SdlRegistrar
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlRegistrar
    PRIVATE     SDL::Exporter
                Conversion::SedsToSdlTranslator
    PUBLIC      Conversion::Common
                CommonProjectWarnings)

add_format_target(SdlRegistrar)
add_tidy_target(SdlRegistrar)

add_library(Conversion::SdlRegistrar ALIAS SdlRegistrar)
