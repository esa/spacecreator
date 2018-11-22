include(../../../../esa.pri)
include(../asn1librarytests.pri)

QT += xml testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_asn1xmlparser

SOURCES += tst_asn1xmlparser.cpp

DISTFILES += \
    boolenum_type.xml \
    choice_type.xml \
    empty.xml \
    invalid_format.xml \
    number_type.xml \
    sequence_type.xml
