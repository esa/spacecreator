set(APP_NAME e2eimagesaver)
project(${APP_NAME} VERSION 0.0.1 LANGUAGES CXX)

add_executable (${APP_NAME})

target_sources(${APP_NAME} PRIVATE
    main.cpp
)

target_include_directories(${APP_NAME} PRIVATE
    .
)

target_link_libraries(${APP_NAME} PRIVATE
    libiveditor
    shared
    templating
    ${QT_CORE}
    ${QT_WIDGETS}
)

set_target_properties(${APP_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (WIN32)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # hide the console window for release builds
    set_property(TARGET ${APP_NAME} PROPERTY WIN32_EXECUTABLE true)
  endif()
endif()

install(TARGETS ${APP_NAME} DESTINATION bin)
