project(SedsConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp
            sedsconvertercli.cpp
            sedsconverter.cpp)
set(INCLUDES sedsconvertercli.h
             sedsconverter.h)

add_executable(SedsConverter)
target_sources(SedsConverter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsConverter
    PRIVATE     .
                ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsConverter
    PRIVATE     Conversion::Converter
                Conversion::Registry
                Conversion::Asn1Registrar
                Conversion::IvRegistrar
                Conversion::SedsRegistrar
                Conversion::SdlRegistrar
                "stdc++fs")

set_target_properties(SedsConverter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "sedsconverter"
)

add_format_target(SedsConverter)
