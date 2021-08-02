project(SedsTestBase VERSION 1.0.0 LANGUAGES CXX)

add_library(SedsTestBase STATIC)
target_sources(SedsTestBase
    PRIVATE     sedstestbase.h
                sedstestbase.cpp)
target_include_directories(SedsTestBase
    PUBLIC      .
                ${CMAKE_SOURCE_DIR}/src/libs/conversion)
target_link_libraries(SedsTestBase
    PUBLIC      Conversion::Converter
                Qt5::Core
                Qt5::Test)

add_format_target(SedsTestBase)

add_library(SEDS::TestBase ALIAS SedsTestBase)
