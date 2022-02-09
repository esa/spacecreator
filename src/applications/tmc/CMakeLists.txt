project(tmc VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp)

set(INCLUDES )

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

add_format_target(tmc)

install(TARGETS tmc DESTINATION bin)
