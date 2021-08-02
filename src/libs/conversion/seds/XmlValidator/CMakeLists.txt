project(XmlValidator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            validator.cpp)
set(INCLUDES exceptions.h
             validator.h)

add_library(XmlValidator STATIC)
target_sources(XmlValidator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(XmlValidator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs/conversion)
target_link_libraries(XmlValidator
    PRIVATE     Conversion::Converter
                LibXml2::LibXml2
    PUBLIC      Qt5::Core)

add_format_target(XmlValidator)

add_library(SEDS::XmlValidator ALIAS XmlValidator)
