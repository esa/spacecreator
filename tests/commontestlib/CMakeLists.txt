set(LIB_NAME commontestlib)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    syntheticinteraction.cpp
    syntheticinteraction.h
    graphicsitemcompare.cpp
    graphicsitemcompare.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    ${QT_CORE}
    ${QT_GUI}
    ${QT_TEST}
    ${QT_WIDGETS}
)
