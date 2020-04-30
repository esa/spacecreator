set(LIB_NAME shared)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    document/abstracttabdocument.cpp
    document/abstracttabdocument.h
    document/documentsmanager.cpp
    document/documentsmanager.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
