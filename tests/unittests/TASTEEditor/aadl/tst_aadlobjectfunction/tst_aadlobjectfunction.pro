QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

TASTE_PRO_ROOT=../../../../../TASTEEditor

INCLUDEPATH += $$TASTE_PRO_ROOT

HEADERS += \
        $$TASTE_PRO_ROOT/app/common.h \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobject.h \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobjectcontainer.h \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobjectfunction.h \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobjectiface.h

SOURCES += \
        $$TASTE_PRO_ROOT/app/common.cpp \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobject.cpp \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobjectcontainer.cpp \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobjectfunction.cpp \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobjectiface.cpp \
        tst_aadlobjectfunction.cpp
