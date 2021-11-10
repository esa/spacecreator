option(ENABLE_FORMAT_CHECK
       "Enable automatic code formatting check with clang-format"
       TRUE)

option(ENABLE_FORMAT_ON_BUILD
       "Enable automatic code formatting on all files"
       FALSE)

find_program(CLANG-FORMAT clang-format)

find_package(Git)
if(GIT_FOUND AND CLANG-FORMAT)
    add_custom_target(GitDiffFormat
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND bash -c "${CLANG-FORMAT} -i -style=file $(${GIT_EXECUTABLE} diff --name-only --no-color ${SC_GIT_MAIN_BRANCH_NAME})"
        VERBATIM)
endif()

function(add_format_target TARGET)
    if(CLANG-FORMAT)
        get_target_property(TARGET_DIR ${TARGET} SOURCE_DIR)
        get_target_property(TARGET_SOURCES ${TARGET} SOURCES)

        set(FORMAT_TARGET_NAME ${TARGET}_format_check)
        add_custom_target(${FORMAT_TARGET_NAME})

        foreach(FILE ${TARGET_SOURCES})
            get_filename_component(FILE ${FILE} REALPATH BASE_DIR ${TARGET_DIR})
            if(ENABLE_FORMAT_ON_BUILD)
                list(APPEND FORMAT_SOURCES ${FILE})
            endif()
            if(ENABLE_FORMAT_CHECK)
                add_custom_command(TARGET ${FORMAT_TARGET_NAME}
                                   COMMAND bash -c "diff ${FILE} <(${CLANG-FORMAT} -style=file ${FILE})"
                                   VERBATIM)
            endif()
        endforeach()

        if(ENABLE_FORMAT_ON_BUILD)
            add_custom_command(TARGET ${FORMAT_TARGET_NAME}
                               COMMAND ${CLANG-FORMAT} -i -style=file ${FORMAT_SOURCES})
        endif()

        add_dependencies(${TARGET} ${FORMAT_TARGET_NAME})
    endif()
endfunction()
