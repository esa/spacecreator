include(../msclibrarytests.pri)

QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_cifparser

SOURCES +=  tst_cifparser.cpp
