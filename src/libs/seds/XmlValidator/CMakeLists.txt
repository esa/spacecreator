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
                CommonProjectWarnings
                stdc++fs)

add_executable(XmlValidator)
target_sources(XmlValidator
    PRIVATE     main.cpp)
target_include_directories(XmlValidator
    PRIVATE     .
                ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(XmlValidator
    PRIVATE     CommonProjectWarnings
                SEDS::XmlValidator)
set_target_properties(XmlValidator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "xmlvalidator")

add_format_target(SedsXmlValidator)
add_tidy_target(SedsXmlValidator)
add_format_target(XmlValidator)

add_library(SEDS::XmlValidator ALIAS SedsXmlValidator)
