if (NOT WIN32)
    # The unit tests do not compile on Windows right now
    add_subdirectory(unittests)
endif()
