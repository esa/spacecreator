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
    PRIVATE     CommonProjectWarnings)

add_format_target(HtmlReport)
add_tidy_target(HtmlReport)

