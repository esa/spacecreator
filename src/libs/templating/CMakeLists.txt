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
    objectsexporter.cpp
    objectsexporter.h
    template_syntax_help.qrc
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
    Grantlee5::Templates
    shared
)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
