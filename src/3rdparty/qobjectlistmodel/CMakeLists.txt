set(LIB_NAME qobjectlistmodel)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    qobjectlistmodel.h
    qobjectlistmodelt.h
    qobjectlistmodel.cpp
)

set_property(SOURCE qobjectlistmodelt.h PROPERTY SKIP_AUTOMOC ON)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME} ${QT_CORE})
