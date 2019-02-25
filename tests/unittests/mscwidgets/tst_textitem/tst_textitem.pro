include(../mscwidgetstests.pri)

QT += testlib widgets gui
TARGET = tst_textitem

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_textitem.cpp \
    textitembasetest.cpp

HEADERS += \
    textitembasetest.h
