cmake_minimum_required(VERSION 3.18)

project(e2eimagesaver VERSION 0.0.1 LANGUAGES CXX)

add_executable (e2eimagesaver
    main.cpp
)

target_include_directories(e2eimagesaver PRIVATE
)

target_link_libraries(e2eimagesaver PRIVATE
    libiveditor
    ${QT_WIDGETS}
)

