project(TmcVerifier VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    verifier.cpp)

set(INCLUDES
    verifier.h)

add_library(TmcVerifier STATIC)
target_sources(TmcVerifier
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(TmcVerifier
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(TmcVerifier
    PUBLIC     Conversion::Common
               Tmc::Converter
               Tmc::HtmlReport
               ${QT_CORE}
    PRIVATE    CommonProjectWarnings)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp PROPERTIES COMPILE_FLAGS "-Wno-useless-cast")
endif()

add_format_target(TmcVerifier)
add_tidy_target(TmcVerifier)

add_library(Tmc::Verifier ALIAS TmcVerifier)
