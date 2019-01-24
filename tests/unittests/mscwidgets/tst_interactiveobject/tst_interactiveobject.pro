include(../mscwidgetstests.pri)

QT += widgets testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_interactiveobject

SOURCES += \
    tst_interactiveobject.cpp

# Common tools for UI testing
include("$$PWD/../testui.pri")
