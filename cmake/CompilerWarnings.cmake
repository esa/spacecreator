option(OPTIONS_WARNINGS_AS_ERRORS
       "Treat compiler wargnings as errors"
       TRUE)
option(OPTIONS_NO_DEPRECATED
       "Disable deprecation warnings"
       TRUE)

set(CLANG_WARNINGS -Wall
                   -Wextra
                   -Wpedantic
                   -Wshadow
                   -Wnon-virtual-dtor
                   -Wold-style-cast
                   -Wcast-align
                   -Wunused
                   -Woverloaded-virtual
                   -Wsign-conversion
                   -Wnull-dereference
                   -Wdouble-promotion
                   -Wpessimizing-move
                   -Wformat=2
                   -Wno-c99-extensions
                   -Wno-deprecated-volatile)

if(OPTIONS_WARNINGS_AS_ERRORS)
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
endif()

if(OPTIONS_NO_DEPRECATED)
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Wno-deprecated -Wno-deprecated-declarations)
endif()

set(GCC_WARNINGS ${CLANG_WARNINGS}
                 -Wmisleading-indentation
                 -Wduplicated-cond
                 -Wduplicated-branches
                 -Wlogical-op
                 -Wuseless-cast)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
endif()

add_library(CommonProjectWarnings INTERFACE)
target_compile_options(CommonProjectWarnings INTERFACE ${PROJECT_WARNINGS})
