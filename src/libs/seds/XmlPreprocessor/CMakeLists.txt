project(SedsXmlPreprocessor VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            preprocessor.cpp)
set(INCLUDES exceptions.h
             preprocessor.h)

add_library(SedsXmlPreprocessor STATIC)
target_sources(SedsXmlPreprocessor
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsXmlPreprocessor
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsXmlPreprocessor
    PUBLIC      Conversion::Common
    PRIVATE     LibXml2::LibXml2
                Qt5::Xml
                CommonProjectWarnings)

add_format_target(SedsXmlPreprocessor)
add_tidy_target(SedsXmlPreprocessor)

add_library(SEDS::XmlPreprocessor ALIAS SedsXmlPreprocessor)
