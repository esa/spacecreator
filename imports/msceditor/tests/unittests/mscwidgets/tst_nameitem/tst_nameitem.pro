include(../mscwidgetstests.pri)

QT += testlib widgets gui
TARGET = tst_nameitem

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_nameitem.cpp \
    ../tst_textitem/textitembasetest.cpp

HEADERS += \
    ../tst_textitem/textitembasetest.h
