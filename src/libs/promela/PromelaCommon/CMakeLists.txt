project(PromelaCommon VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  namehelper.cpp
)
set(INCLUDES
  namehelper.h
  constants.h
)

add_library(PromelaCommon STATIC)
target_sources(PromelaCommon
  PRIVATE "${SOURCES}"
  "${INCLUDES}")

target_include_directories(PromelaCommon
  PUBLIC    .
  PRIVATE   ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(PromelaCommon
  PUBLIC     Qt${QT_VERSION_MAJOR}::Core
  PRIVATE    CommonProjectWarnings)

add_format_target(PromelaCommon)
add_tidy_target(PromelaCommon)

add_library(Promela::Common ALIAS PromelaCommon)
