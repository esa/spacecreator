set(APP_NAME dvconverter)
project(${APP_NAME})

add_executable(${APP_NAME})

target_sources(${APP_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${APP_NAME} PUBLIC
    dvcore
    libdveditor
    shared
    templating
    ${QT_WIDGETS}
)

target_compile_definitions(${APP_NAME} PUBLIC QT_MESSAGELOGCONTEXT)

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
