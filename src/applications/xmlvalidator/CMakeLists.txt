project(XmlValidator VERSION 1.0.0 LANGUAGES CXX)

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

add_format_target(XmlValidator)
