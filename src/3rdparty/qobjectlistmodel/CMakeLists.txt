set(LIB_NAME qobjectlistmodel)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    qobjectlistmodel.h
    qobjectlistmodelt.h
    qobjectlistmodel.cpp
)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME} Qt5::Core)
