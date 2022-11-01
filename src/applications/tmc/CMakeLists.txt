project(tmc VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp
    executor.cpp)

set(INCLUDES executor.h)

add_executable(tmc)
target_sources(tmc
	PRIVATE    "${SOURCES}"
	           "${INCLUDES}")
target_include_directories(tmc
	PRIVATE    .
	           ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(tmc
    PRIVATE    Tmc::Verifier
	           CommonProjectWarnings)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp PROPERTIES COMPILE_FLAGS "-Wno-useless-cast")
add_format_target(tmc)

set_target_properties(tmc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "tmc"
)

install(TARGETS tmc DESTINATION bin)
