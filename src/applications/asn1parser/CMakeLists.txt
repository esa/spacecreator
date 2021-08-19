project(asn1parser VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp)

add_executable(asn1parser)
target_sources(asn1parser
    PRIVATE     "${SOURCES}")
target_include_directories(asn1parser
    PRIVATE     .
                ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(asn1parser
    PRIVATE     asn1library)

add_format_target(asn1parser)
