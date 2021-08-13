project(XmlPreprocessor VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            preprocessor.cpp)
set(INCLUDES exceptions.h
             preprocessor.h)

add_library(XmlPreprocessor STATIC)
target_sources(XmlPreprocessor
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(XmlPreprocessor
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(XmlPreprocessor
    PRIVATE     LibXml2::LibXml2
                Qt5::Xml
    PUBLIC      Conversion::Common)

add_format_target(XmlPreprocessor)

add_library(SEDS::XmlPreprocessor ALIAS XmlPreprocessor)
