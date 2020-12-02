# Usage:
# cmake -G Ninja -DCOVERAGE_ENABLED=ON ../SpaceCreator/ ; ninja ; ctest ; ninja coverage ; ninja coverage_html
#
# Target "coverage" will create the file "coverage_gcov.html" in the build directory. Created by gcovr
# Target "coverage_html" will create the directory "lcov-html " with "/index.html" in the build directory. Created by lcov
#

if(COVERAGE_ENABLED)
find_program( GCOV_PATH gcovr )

if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        message(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
    endif()
elseif(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
endif()

set(COVERAGE_COMPILER_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage")

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COVERAGE_LINKER_FLAGS "")
    set(COVERAGE_LIBRARIES "gcov")
else()
    set(COVERAGE_LINKER_FLAGS "--coverage")
    set(COVERAGE_LIBRARIES "")
endif()

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${COVERAGE_LINKER_FLAGS} -lgcov" )

mark_as_advanced(
    COVERAGE_COMPILER_FLAGS
    COVERAGE_LINKER_FLAGS
    COVERAGE_LIBRARIES
)

add_custom_command(OUTPUT _run_gcovr_parser
    POST_BUILD
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/src --object-dir=${CMAKE_BINARY_DIR}/src -e ".*/3rdparty.*" --html > ${CMAKE_BINARY_DIR}/coverage_gcov.html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target (coverage DEPENDS _run_gcovr_parser)

add_custom_command(OUTPUT _run_lcov_parser
    POST_BUILD
    COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file=coverage.info
    COMMAND lcov --remove coverage.info "*opt/*" "*usr/include/*" "*3rdparty/*" "*MscBaseVisitor*" "*MscLexer*" "*MscParser*" "*/tests/*" "*_autogen*" -o coverage-cleared.info
    COMMAND genhtml coverage-cleared.info --output lcov-html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_custom_target (coverage_html DEPENDS _run_lcov_parser)

message(STATUS "Code coverage enabled")
endif()
