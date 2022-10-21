#if (QTC_VERSION EQUAL 408)
option(BUILD_ASN1PLUGIN "Build the ANS1Plugin" ON)
if (BUILD_ASN1PLUGIN)
    add_subdirectory(asn1plugin)
    add_subdirectory(simulinkimporterplugin)
endif()

add_subdirectory(spacecreatorplugin)
add_subdirectory(sedsplugin)

add_subdirectory(functiontesterplugin)
add_subdirectory(devicegeneratorplugin)
