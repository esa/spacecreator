project(MscToSdlConverter VERSION 1.0.0 LANGUAGES CXX)

add_executable(MscToSdlConverter)
target_sources(MscToSdlConverter
    PRIVATE     main.cpp)
target_include_directories(MscToSdlConverter
    PRIVATE     .
                ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(MscToSdlConverter
    PRIVATE     CommonProjectWarnings
                msccore
                ASN1::Importer
                IV::XmlImporter
                MSC::Importer
                SDL::Exporter
                shared
                Conversion::MscToSdlTranslator)
set_target_properties(MscToSdlConverter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "msctosdlconverter")

add_format_target(MscToSdlConverter)
