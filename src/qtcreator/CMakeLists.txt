if (QTC_VERSION EQUAL 408)
    add_subdirectory(asn1plugin)
endif()

add_subdirectory(common)
add_subdirectory(spacecreatorplugin)

if(${QTC_FOUND})
    add_subdirectory(sedsplugin)
endif()
