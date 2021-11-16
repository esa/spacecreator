project(SedsRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(SedsRegistrar STATIC)
target_sources(SedsRegistrar
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsRegistrar
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsRegistrar
    PRIVATE     SEDS::XmlImporter
                Conversion::IvToSedsTranslator
    PUBLIC      Conversion::Common
                CommonProjectWarnings)

add_format_target(SedsRegistrar)
add_tidy_target(SedsRegistrar)

add_library(Conversion::SedsRegistrar ALIAS SedsRegistrar)
