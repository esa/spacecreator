project(Report VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    spinerrorparser.cpp
    spinerrorreport.cpp)

set(INCLUDES
    spinerrorparser.h
    spinerrorreport.h)

add_library(Report STATIC)
target_sources(Report
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")

target_link_libraries(Report
    PUBLIC      ivcore
                libiveditor
    PRIVATE     CommonProjectWarnings)

add_format_target(Report)
add_tidy_target(Report)

