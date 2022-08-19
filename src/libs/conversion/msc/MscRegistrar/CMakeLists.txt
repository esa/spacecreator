project(MscRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(MscRegistrar STATIC)
target_sources(MscRegistrar
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
            target_include_directories(MscRegistrar
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(MscRegistrar
    PUBLIC      Conversion::Common
                MSC::Importer
                MSC::Exporter
                Conversion::MscToSdlTranslator
    PRIVATE     CommonProjectWarnings)

add_format_target(MscRegistrar)
add_tidy_target(MscRegistrar)

add_library(Conversion::MscRegistrar ALIAS MscRegistrar)
