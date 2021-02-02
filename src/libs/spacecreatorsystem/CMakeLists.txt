set(LIB_NAME spacecreatorsystem)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    editorcorequery.cpp
    editorcorequery.h
    ivsystemchecks.cpp
    ivsystemchecks.h
    mscsystemchecks.cpp
    mscsystemchecks.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME}
    libmsceditor
    libiveditor
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)
