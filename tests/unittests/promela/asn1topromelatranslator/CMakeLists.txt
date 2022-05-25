set(TEST_NAME tst_asn1topromelatranslator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     tst_asn1topromelatranslator.cpp
                tst_integersubset.cpp
                tst_integergenerator.cpp
                tst_asn1topromelatranslator_env.cpp
                tst_asn1topromelatranslator_rangechecks.cpp
                tst_asn1topromelatranslator_values.cpp
                main.cpp
                tst_asn1topromelatranslator.h
                tst_integersubset.h
                tst_integergenerator.h
                tst_asn1topromelatranslator_env.h
                tst_asn1topromelatranslator_rangechecks.h
                tst_asn1topromelatranslator_values.h)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::Asn1ToPromelaTranslator
                PromelaExporter
                CommonTools
                TextCheckerAndConsumer
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(resources/sequence.asn resources/sequence.asn COPYONLY)
configure_file(resources/sequence.pml.out resources/sequence.pml.out COPYONLY)

configure_file(resources/sequence-embedded.asn resources/sequence-embedded.asn COPYONLY)
configure_file(resources/sequence-embedded.pml.out resources/sequence-embedded.pml.out COPYONLY)

configure_file(resources/sequence-nested.asn resources/sequence-nested.asn COPYONLY)
configure_file(resources/sequence-nested.pml.out resources/sequence-nested.pml.out COPYONLY)

configure_file(resources/sequence-nested-anonymous.asn resources/sequence-nested-anonymous.asn COPYONLY)
configure_file(resources/sequence-nested-anonymous.pml.out resources/sequence-nested-anonymous.pml.out COPYONLY)

configure_file(resources/sequenceof.asn resources/sequenceof.asn COPYONLY)
configure_file(resources/sequenceof.pml.out resources/sequenceof.pml.out COPYONLY)

configure_file(resources/sequenceof-variable-size.asn resources/sequenceof-variable-size.asn COPYONLY)
configure_file(resources/sequenceof-variable-size.pml.out resources/sequenceof-variable-size.pml.out COPYONLY)

configure_file(resources/sequenceof-nested.asn resources/sequenceof-nested.asn COPYONLY)
configure_file(resources/sequenceof-nested.pml.out resources/sequenceof-nested.pml.out COPYONLY)

configure_file(resources/choice.asn resources/choice.asn COPYONLY)
configure_file(resources/choice.pml.out resources/choice.pml.out COPYONLY)

configure_file(resources/choice-anonymous.asn resources/choice-anonymous.asn COPYONLY)
configure_file(resources/choice-anonymous.pml.out resources/choice-anonymous.pml.out COPYONLY)

configure_file(resources/choice-sub.asn resources/choice-sub.asn COPYONLY)
configure_file(resources/choice-sub.pml.out resources/choice-sub.pml.out COPYONLY)

configure_file(resources/octetstring.asn resources/octetstring.asn COPYONLY)
configure_file(resources/octetstring.pml.out resources/octetstring.pml.out COPYONLY)

configure_file(resources/octetstring-variable-size.asn resources/octetstring-variable-size.asn COPYONLY)
configure_file(resources/octetstring-variable-size.pml.out resources/octetstring-variable-size.pml.out COPYONLY)
