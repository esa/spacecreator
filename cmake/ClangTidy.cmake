option(ENABLE_CLANG_TIDY
       "Enable static analysis with clang-tidy"
       FALSE)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG-TIDY clang-tidy)
    if(CLANG-TIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG-TIDY})
    endif(CLANG-TIDY)
endif(ENABLE_CLANG_TIDY)
