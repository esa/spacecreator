option(ENABLE_CLANG_TIDY
       "Enable static analysis with clang-tidy"
       FALSE)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG-TIDY clang-tidy)
endif(ENABLE_CLANG_TIDY)

function(add_tidy_target TARGET)
    if(ENABLE_CLANG_TIDY)
        set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY ${CLANG-TIDY})
    endif()
endfunction()
