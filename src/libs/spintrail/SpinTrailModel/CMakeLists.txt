project(SpinTrailModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES spintrailmodel.h
	trailevent.h
	channelevent.h
	continuoussignal.h)

set(INCLUDES spintrailmodel.cpp
	channelevent.cpp
	continuoussignal.cpp)

add_library(SpinTrailModel STATIC)
target_sources(SpinTrailModel
	PRIVATE    "${SOURCES}"
	           "${INCLUDES}")
target_include_directories(SpinTrailModel
	PUBLIC     .
	PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SpinTrailModel
	PUBLIC     Conversion::Common
	PRIVATE    CommonProjectWarnings)

add_format_target(SpinTrailModel)
add_tidy_target(SpinTrailModel)

add_library(SpinTrail::Model ALIAS SpinTrailModel)
