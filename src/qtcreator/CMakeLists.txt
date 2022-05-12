add_subdirectory(common)

if (QTC_VERSION EQUAL 408)
    add_subdirectory(asn1plugin)
    add_subdirectory(spacecreatorplugin)
    add_subdirectory(sedsplugin)
endif()
