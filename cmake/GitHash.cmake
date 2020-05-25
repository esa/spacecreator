# Set the SC_GIT_HASH variable
find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    OUTPUT_VARIABLE SC_GIT_HASH
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(STATUS "GIT hash: ${SC_GIT_HASH}")
else()
  message(STATUS "GIT not found")
endif()

add_definitions (-DVS_BUILD_HASH="${SC_GIT_HASH}")
