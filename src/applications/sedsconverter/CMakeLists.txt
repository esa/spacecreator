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

set (LIBS CommonProjectWarnings
    Conversion::Converter
    Conversion::Registry
    Conversion::Asn1Registrar
    Conversion::IvRegistrar
    Conversion::SedsRegistrar
    Conversion::SdlRegistrar
    Conversion::Asn1ToSedsTranslator
)

if(NOT APPLE)
    set (LIBS ${LIBS} "stdc++fs")
endif()

target_link_libraries(SedsConverter PRIVATE ${LIBS})

set_target_properties(SedsConverter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "sedsconverter"
)

add_format_target(SedsConverter)
