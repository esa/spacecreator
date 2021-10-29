project(SedsConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp
            sedsconverter.cpp)
set(INCLUDES sedsconverter.h)

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
                Conversion::SdlRegistrar)

add_format_target(SedsConverter)
