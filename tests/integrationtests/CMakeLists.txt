if (NOT DISABLE_SLOW_TESTS)
    add_subdirectory(applications)
    add_subdirectory(conversion)
endif()

add_subdirectory(libmsceditor)
add_subdirectory(testgenerator)
