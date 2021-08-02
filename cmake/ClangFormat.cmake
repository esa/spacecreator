option(ENABLE_FORMATTING
       "Enable automatic code formatting with clang-format"
       TRUE)

if(ENABLE_FORMATTING)
    find_program(CLANG-FORMAT clang-format)
endif()

function(add_format_target TARGET)
    if(ENABLE_FORMATTING)
        get_target_property(TARGET_DIR ${TARGET} SOURCE_DIR)
        get_target_property(TARGET_SOURCES ${TARGET} SOURCES)

        foreach(FILE ${TARGET_SOURCES})
            get_filename_component(FILE ${FILE} REALPATH BASE_DIR ${TARGET_DIR})
            list(APPEND FORMAT_SOURCES ${FILE})
        endforeach()

        set(FORMAT_TARGET_NAME ${TARGET}_format)

        add_custom_target(${FORMAT_TARGET_NAME}
                          COMMAND ${CLANG-FORMAT} -i -style=file ${FORMAT_SOURCES}
                          COMMENT "Formatting ${TARGET}...")

        add_dependencies(${TARGET} ${FORMAT_TARGET_NAME})
    endif()
endfunction()
