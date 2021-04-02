set(APP_NAME mscstreaming)
project(${APP_NAME})

add_executable(${APP_NAME})

target_sources(${APP_NAME} PRIVATE
    main.cpp
    streamingwindow.cpp
    streamingwindow.h
    streamingwindow.ui
)

target_link_libraries(${APP_NAME} PUBLIC
    libmsceditor
    libiveditor
    asn1editor
    msclibrary
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)

target_include_directories(${APP_NAME} PRIVATE
    .
)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")

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
