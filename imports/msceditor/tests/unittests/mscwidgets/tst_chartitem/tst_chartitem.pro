include(../mscwidgetstests.pri)

QT += gui widgets testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_chartitem.cpp
