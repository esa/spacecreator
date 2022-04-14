set(TEST_NAME tst_asn1topromelatranslator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     tst_asn1topromelatranslator.cpp
                tst_integersubset.cpp
                tst_integergenerator.cpp
                tst_asn1topromelatranslator_env.cpp
                tst_asn1topromelatranslator_values.cpp
                main.cpp
                tst_asn1topromelatranslator.h
                tst_integersubset.h
                tst_integergenerator.h
                tst_asn1topromelatranslator_env.h
                tst_asn1topromelatranslator_values.h)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::Asn1ToPromelaTranslator
                Asn1Exporter
                PromelaExporter
                PluginCommon
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(resources/myModule.asn resources/myModule.asn COPYONLY)
configure_file(resources/myModule_typesInSeq.asn resources/myModule_typesInSeq.asn COPYONLY)
