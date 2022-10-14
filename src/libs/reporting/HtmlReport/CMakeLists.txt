project(HtmlReport VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    htmlreportbuilder.cpp)

set(INCLUDES
    htmlreportbuilder.h)

add_library(HtmlReport STATIC)
target_sources(HtmlReport
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")

target_link_libraries(HtmlReport
    PUBLIC      ivcore
                libiveditor
                Report
                Grantlee5::TextDocument
                Grantlee5::Templates
    PRIVATE     CommonProjectWarnings)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/template.html
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_format_target(HtmlReport)
add_tidy_target(HtmlReport)

