project(IvMergerApp VERSION 1.0.0 LANGUAGES CXX)

add_executable(IvMergerApp)
target_sources(IvMergerApp
    PRIVATE     main.cpp)
target_include_directories(IvMergerApp
    PRIVATE     .
                ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(IvMergerApp
    PRIVATE     CommonProjectWarnings
                ${QT_CORE}
                asn1library
                asn1editor
                IvMerger)
set_target_properties(IvMergerApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "ivmerger")

add_format_target(IvMergerApp)
