if (${QTC_FOUND})
    add_subdirectory(qtcreator)
endif()

add_definitions(-DQT_NO_KEYWORDS)
add_subdirectory(3rdparty)
add_subdirectory(libs)
add_subdirectory(applications)
