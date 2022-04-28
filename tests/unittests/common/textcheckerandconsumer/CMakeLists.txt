project(TextCheckerAndConsumer VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES textcheckerandconsumer.cpp
            )
set(INCLUDES textcheckerandconsumer.h
             )

add_library(TextCheckerAndConsumer STATIC)

target_sources(TextCheckerAndConsumer
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")

target_include_directories(TextCheckerAndConsumer
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(TextCheckerAndConsumer
    PUBLIC      SdlModel
    PRIVATE     CommonProjectWarnings)

add_format_target(TextCheckerAndConsumer)
add_tidy_target(TextCheckerAndConsumer)

add_library(Common::TextCheckerAndConsumer ALIAS TextCheckerAndConsumer)
