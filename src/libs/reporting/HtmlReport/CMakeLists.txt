project(HtmlReport VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    htmlreportpresenter.cpp
    htmlreportbuilder.cpp
    tracebuilder.cpp)

set(INCLUDES
    htmlreportpresenter.h
    htmlreportbuilder.h
    tracebuilder.h)

set(RESOURCES
    htmlreportbuilder.qrc)

set(CMAKE_AUTORCC ON)

add_library(HtmlReport STATIC)
target_sources(HtmlReport
    PRIVATE    "${SOURCES}"
               "${INCLUDES}"
               "${RESOURCES}")

target_include_directories(HtmlReport
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(HtmlReport
    PUBLIC      Qt${QT_VERSION_MAJOR}::Core
                Tmc::Report
                Grantlee5::TextDocument
                Grantlee5::Templates
    PRIVATE     CommonProjectWarnings)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/template.html
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_format_target(HtmlReport)
add_tidy_target(HtmlReport)

add_library(Tmc::HtmlReport ALIAS HtmlReport)
