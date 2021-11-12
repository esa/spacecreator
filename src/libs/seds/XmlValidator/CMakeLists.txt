project(SedsXmlValidator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            validator.cpp)
set(INCLUDES exceptions.h
             validator.h)

add_library(SedsXmlValidator STATIC)
target_sources(SedsXmlValidator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsXmlValidator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsXmlValidator
    PUBLIC      Conversion::Converter
                Qt5::Core
    PRIVATE     LibXml2::LibXml2
                CommonProjectWarnings)

add_format_target(SedsXmlValidator)
add_tidy_target(SedsXmlValidator)

add_library(SEDS::XmlValidator ALIAS SedsXmlValidator)
