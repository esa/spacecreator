include(../../../../esa.pri)
include(../../../../msceditor/msceditortests.pri)

QT += testlib
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_commandlineparser.cpp
