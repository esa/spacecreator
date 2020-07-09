set(APP_NAME mscconverter)
project(${APP_NAME})

add_executable(${APP_NAME})

target_sources(${APP_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${APP_NAME} PUBLIC
    msclibrary
    shared
    Qt5::Core
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

install(TARGETS ${APP_NAME} DESTINATION bin)
