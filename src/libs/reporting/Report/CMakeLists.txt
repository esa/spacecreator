project(Report VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    report.cpp)

set(INCLUDES
    report.h)

add_library(Report STATIC)
target_sources(Report
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")

#add_format_target(Report)
#add_tidy_target(Report)

