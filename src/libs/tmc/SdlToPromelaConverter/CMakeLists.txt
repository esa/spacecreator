project(SdlToPromelaConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    processmetadata.cpp
    converter.cpp)

set(INCLUDES
    processmetadata.h
    converter.h)

add_library(SdlToPromelaConverter STATIC)
target_sources(SdlToPromelaConverter
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(SdlToPromelaConverter
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlToPromelaConverter
    PUBLIC     Conversion::Common
               Tmc::Converter
    PRIVATE    CommonProjectWarnings)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp PROPERTIES COMPILE_FLAGS "-Wno-useless-cast")
endif()

add_format_target(SdlToPromelaConverter)
add_tidy_target(SdlToPromelaConverter)

add_library(Tmc::SdlToPromelaConverter ALIAS SdlToPromelaConverter)
