set(LIB_NAME utils)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    animation.cpp
    animation.h
    common.cpp
    common.h
    delayedsignal.cpp
    delayedsignal.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
