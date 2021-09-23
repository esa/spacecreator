project(SdlExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exporter.cpp
            visitors/sdlvisitor.cpp)
set(INCLUDES exporter.h
             visitors/sdlvisitor.h)

add_library(SdlExporter STATIC)
target_sources(SdlExporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SdlExporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlExporter
    PUBLIC      Conversion::Common
                asn1library
    PRIVATE     CommonProjectWarnings)

add_format_target(SdlExporter)
add_tidy_target(SdlExporter)

add_library(Conversion::SdlExporter ALIAS SdlExporter)
