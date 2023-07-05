project(Report VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    dataconstraintviolationreport.cpp
    mscfailurereport.cpp
    observerfailurereport.cpp
    rawerroritem.cpp
    spinerrorparser.cpp
    spinerrorreportitem.cpp
    stopconditionviolationreport.cpp)

set(INCLUDES
    dataconstraintviolationreport.h
    mscfailurereport.h
    observerfailurereport.h
    rawerroritem.h
    spinerrorparser.h
    spinerrorreportitem.h
    stopconditionviolationreport.h)

add_library(Report STATIC)
target_sources(Report
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")

target_include_directories(Report
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(Report
    PUBLIC      Qt${QT_VERSION_MAJOR}::Core
    PRIVATE     CommonProjectWarnings)

add_format_target(Report)
add_tidy_target(Report)

add_library(Tmc::Report ALIAS Report)
