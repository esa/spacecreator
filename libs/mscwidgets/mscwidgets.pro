TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    graphicsview.h

SOURCES += \
    graphicsview.cpp
