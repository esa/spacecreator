project(TmcConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    converter.cpp)

set(INCLUDES
    converter.h)

add_library(TmcConverter STATIC)
target_sources(TmcConverter
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(TmcConverter
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(TmcConverter
    PUBLIC     Conversion::Common
               Conversion::Converter
               Conversion::Registry
               Conversion::Asn1Registrar
               Conversion::IvRegistrar
               Conversion::MscRegistrar
               Conversion::PromelaRegistrar
               Conversion::SdlRegistrar
               Conversion::SpinTrailRegistrar
               Conversion::SimulatorTrailRegistrar
               Tmc::SdlToPromelaConverter
               ivcore
    PRIVATE    CommonProjectWarnings
               Tmc::InterfaceViewOptimizer)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp PROPERTIES COMPILE_FLAGS "-Wno-useless-cast")
endif()

add_format_target(TmcConverter)
add_tidy_target(TmcConverter)

add_library(Tmc::Converter ALIAS TmcConverter)
