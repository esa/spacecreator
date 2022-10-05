project(SpinTrailImporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES spintrailimporter.cpp)

set(INCLUDES spintrailimporter.h)

add_library(SpinTrailImporter STATIC)
target_sources(SpinTrailImporter
	PRIVATE "${SOURCES}"
	"${INCLUDES}")

target_include_directories(SpinTrailImporter
	PUBLIC .
	PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SpinTrailImporter
	PUBLIC Conversion::Common
	SpinTrail::Model
	PRIVATE CommonProjectWarnings)

add_format_target(SpinTrailImporter)
add_tidy_target(SpinTrailImporter)

add_library(SpinTrail::Importer ALIAS SpinTrailImporter)
