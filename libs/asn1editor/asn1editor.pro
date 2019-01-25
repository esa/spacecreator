TEMPLATE = lib
TARGET = asn1editor

include(../../esa.pri)

CONFIG += static

QT += core gui widgets xml

HEADERS += \
    asn1editor.h \
    asn1itemdelegate.h \
    asn1treeview.h \
    asn1xmlparser.h \
    asn1valueparser.h \
    asn1itemmodel.h \
    asn1const.h

SOURCES += \
    asn1editor.cpp \
    asn1itemdelegate.cpp \
    asn1treeview.cpp \
    asn1xmlparser.cpp \
    asn1valueparser.cpp \
    asn1itemmodel.cpp

FORMS += asn1editor.ui
