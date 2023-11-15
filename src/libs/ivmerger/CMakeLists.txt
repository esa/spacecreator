project(IvMerger VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  fullivexporter.cpp
  ivmerger.cpp
)
set(INCLUDES
  fullivexporter.h
  ivmerger.h
)

add_library(IvMerger STATIC)
target_sources(IvMerger
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")

target_include_directories(IvMerger
  PUBLIC                   .
  PRIVATE                  "${CMAKE_SOURCE_DIR}/src/libs")
target_link_libraries(IvMerger
  PUBLIC              ${QT_CORE}
                      ${QT_XML}
                      ivcore
                      "stdc++fs"
  PRIVATE             CommonProjectWarnings)

add_format_target(IvMerger)
add_tidy_target(IvMerger)
