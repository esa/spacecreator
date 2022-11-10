#if (QTC_VERSION EQUAL 408)
option(BUILD_PLUGIN_ASN1 "Build the ANS1Plugin" ON)
if (BUILD_PLUGIN_ASN1)
    add_subdirectory(asn1plugin)
    add_subdirectory(simulinkimporterplugin)
endif()

add_subdirectory(spacecreatorplugin)
add_subdirectory(sedsplugin)

add_subdirectory(functiontesterplugin)
add_subdirectory(devicegeneratorplugin)
