set(APP_NAME msceditor)

add_executable(${APP_NAME})

target_sources(${APP_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${APP_NAME} PUBLIC
    application
    asn1editor
    msclibrary
    mscwidgets
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

target_include_directories(${APP_NAME} PRIVATE
    .
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
