include(../../../../esa.pri)
include(../../../../depend_asn1library.pri)

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_asn1file

SOURCES += tst_asn1file.cpp
