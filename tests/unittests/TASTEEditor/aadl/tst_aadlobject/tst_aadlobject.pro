
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
        $$TASTE_PRO_ROOT/tab_aadl/aadlobject.h

SOURCES += \
        $$TASTE_PRO_ROOT/app/common.cpp \
        $$TASTE_PRO_ROOT/tab_aadl/aadlobject.cpp \
        tst_aadlobject.cpp
