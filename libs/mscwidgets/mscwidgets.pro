TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    graphicsview.h \
    instanceitem.h \
    messageitem.h

SOURCES += \
    graphicsview.cpp \
    instanceitem.cpp \
    messageitem.cpp
