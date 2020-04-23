include(../msclibrarytests.pri)
include(../msclibrarytests_ciflinepoints.pri)

QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_ciflineposition

SOURCES +=  tst_ciflineposition.cpp
