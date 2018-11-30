include(../../../../esa.pri)
include(../mscwidgetstests.pri)

QT += widgets testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_100messages

SOURCES +=  tst_100messages.cpp

# Common tools for UI testing
include("$$PWD/../testui.pri")

