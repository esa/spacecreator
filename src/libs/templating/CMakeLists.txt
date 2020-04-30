set(LIB_NAME templating)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    abstractexportableobject.h
    exportableproperty.h
    stringtemplate.cpp
    stringtemplate.h
    templateeditor.cpp
    templateeditor.h
    templatehighlighter.cpp
    templatehighlighter.h
    templatesyntaxhelpdialog.cpp
    templatesyntaxhelpdialog.h
    xmlhighlighter.cpp
    xmlhighlighter.h
    template_syntax_help.qrc
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Grantlee5::Templates
)
