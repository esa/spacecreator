TEMPLATE = lib
TARGET = asn1editor

include(../../esa.pri)

CONFIG += static

QT += core gui widgets xml

HEADERS += \
    asn1editor.h \
    xmlparser.h \
    asn1itemdelegate.h \
    asn1treeview.h

SOURCES += \
    asn1editor.cpp \
    xmlparser.cpp \
    asn1itemdelegate.cpp \
    asn1treeview.cpp

BuildType =release
CONFIG(debug, debug|release) {
    BuildType =debug
} # Profiling, etc (if any) are defaulted to Release

FORMS += asn1editor.ui
