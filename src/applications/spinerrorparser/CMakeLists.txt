project(spinerrorparser VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp)

set(INCLUDES )

add_executable(spinerrorparser)
target_sources(spinerrorparser
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(spinerrorparser
    PRIVATE    .
               ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(spinerrorparser
    PUBLIC      ivcore
                libiveditor
                HtmlReport
                Report
    PRIVATE     CommonProjectWarnings)

add_format_target(spinerrorparser)

set_target_properties(spinerrorparser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "spinerrorparser"
)

install(TARGETS spinerrorparser DESTINATION bin)
