option(OPTIONS_WARNINGS_AS_ERRORS
       "Treat compiler warnings as errors"
       TRUE)
option(OPTIONS_NO_DEPRECATED
       "Disable deprecation warnings"
       TRUE)

set(COMMON_WARNINGS -Wall
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
                   -Wformat=2
)

if(OPTIONS_WARNINGS_AS_ERRORS)
    set(COMMON_WARNINGS ${COMMON_WARNINGS} -Werror)
endif()

if(OPTIONS_NO_DEPRECATED)
    set(COMMON_WARNINGS ${COMMON_WARNINGS} -Wno-deprecated -Wno-deprecated-declarations)
endif()


set(CLANG_WARNINGS ${COMMON_WARNINGS}
                   -Wno-c99-extensions
                   -Wno-deprecated-volatile
)

set(GCC_WARNINGS ${COMMON_WARNINGS}
                 -Wmisleading-indentation
                 -Wduplicated-cond
                 -Wduplicated-branches
                 -Wlogical-op
                 -Wuseless-cast
                 -Wno-unused-result
                 -Wno-pessimizing-move
             )

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
endif()

add_library(CommonProjectWarnings INTERFACE)
target_compile_options(CommonProjectWarnings INTERFACE ${PROJECT_WARNINGS})
