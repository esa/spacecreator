include(../msceditortests.pri)

QT += testlib gui
CONFIG += qt warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_remotecontrol.cpp

win32: QMAKE_LFLAGS_RELEASE += /LTCG
