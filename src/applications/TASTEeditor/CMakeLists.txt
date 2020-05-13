set(APP_NAME IVeditor)

add_executable(${APP_NAME})

target_sources(${APP_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${APP_NAME} PUBLIC
    aadl
    shared
    libiveditor
    templating
    utils
    Qt5::Concurrent
    Qt5::Network
    Qt5::Svg
    Qt5::Widgets
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
