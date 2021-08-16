project(SedsToAsn1Translator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES translator.cpp)
set(INCLUDES translator.h)

add_library(SedsToAsn1Translator STATIC)
target_sources(SedsToAsn1Translator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsToAsn1Translator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsToAsn1Translator
    PUBLIC      Conversion::Common
                SEDS::Model)

add_format_target(SedsToAsn1Translator)

add_library(Conversion::SedsToAsn1Translator ALIAS SedsToAsn1Translator)
