include(../mscwidgetstests.pri)

QT += gui widgets testlib

CONFIG += qt warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_chartviewmodel

SOURCES += \ 
    tst_chartviewmodel.cpp
