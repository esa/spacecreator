function(addAsn1PluginTest TEST_NAME)
    set(EXTRA_SOURCES ${ARGV1})
    add_executable(${TEST_NAME}
        ${TEST_NAME}.cpp
        ${TEST_NAME}.h
        ${EXTRA_SOURCES}
    )

    set(QTC_LIBRARIES
        -L"${QTC_LIB_DIR}"
        -L"${QTC_PLUGINS_DIR}"
        Aggregation
        ExtensionSystem
        QtcSsh
        Utils
    )
    set(QTC_PLUGINS
        Core
        ProjectExplorer
        TextEditor
    )

    target_link_libraries(${TEST_NAME} PUBLIC
        asn1library
        Asn1Acn
        ${QTC_LIBRARIES}
        ${QTC_PLUGINS}
        ${QT_TEST}
        ${QT_WIDGETS}
    )

    target_include_directories(${TEST_NAME}
        PUBLIC common
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}.xml)

    set_tests_properties(${TEST_NAME} PROPERTIES
        ENVIRONMENT "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QTC_LIB_DIR}:${QTC_PLUGINS_DIR}"
    )
endfunction()

addAsn1PluginTest(tst_autocompleter)
addAsn1PluginTest(tst_combomodel "common/modeltest.cpp;common/modeltest.h")
addAsn1PluginTest(tst_displayrolevisitor)
addAsn1PluginTest(tst_documentprocessor "common/parseddocumentbuilderstub.cpp;common/parseddocumentbuilderstub.h")
addAsn1PluginTest(tst_errormessageparser)
addAsn1PluginTest(tst_filemodel "common/modeltest.cpp;common/modeltest.h")
addAsn1PluginTest(tst_generalmetadataparser)
addAsn1PluginTest(tst_indenter)
addAsn1PluginTest(tst_linkcreator)
addAsn1PluginTest(tst_metadatacheckstatehandler "common/modeltest.cpp;common/modeltest.h")
addAsn1PluginTest(tst_metadatamodel "common/modeltest.cpp;common/modeltest.h")
addAsn1PluginTest(tst_modelvalidityguard)
addAsn1PluginTest(tst_modulemetadataparser)
addAsn1PluginTest(tst_outlineindexupdater)
addAsn1PluginTest(tst_outlinemodel "common/modeltest.cpp;common/modeltest.h")
addAsn1PluginTest(tst_parseddatastorage)
addAsn1PluginTest(tst_projectcontenthandler "documentprocessorstub.cpp;documentprocessorstub.h;sourcereadermock.cpp;sourcereadermock.h")
addAsn1PluginTest(tst_selectionpositionresolver)
addAsn1PluginTest(tst_typestreemodel "common/modeltest.cpp;common/modeltest.h")
