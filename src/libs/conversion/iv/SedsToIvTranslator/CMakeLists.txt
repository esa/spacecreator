project(SedsToIvTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES translator.cpp)
set(INCLUDES translator.h)

add_library(SedsToIvTranslator STATIC)
target_sources(SedsToIvTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsToIvTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsToIvTranslator
    PUBLIC      Conversion::Common
                ivcore
                SEDS::Model)

add_format_target(SedsToIvTranslator)

add_library(Conversion::SedsToIvTranslator ALIAS SedsToIvTranslator)
