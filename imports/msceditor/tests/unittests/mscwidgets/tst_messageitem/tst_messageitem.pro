include(../mscwidgetstests.pri)

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_messageitem

SOURCES +=  tst_messageitem.cpp
