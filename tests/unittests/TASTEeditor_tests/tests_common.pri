QT += testlib core gui widgets
CONFIG += qt console warn_on depend_includepath testcase c++11
mac:CONFIG -= app_bundle

TEMPLATE = app

TASTE_PRO_ROOT=$$PWD/../../../TASTEeditor

INCLUDEPATH += $$TASTE_PRO_ROOT

GRANTLEE_LIB_DIR = $$[QT_INSTALL_LIBS]

DEFINES += GRANTLEE_LIB_DIR='\\"$$GRANTLEE_LIB_DIR\\"'
GRANTLEE_LIB=Grantlee_Templates
win32: CONFIG(debug, debug|release):GRANTLEE_LIB = $$join(GRANTLEE_LIB,,,d)

LIBS += -L$$GRANTLEE_LIB_DIR -l$$GRANTLEE_LIB

DISTFILES += \
    $$PWD/TASTEeditor_sources.pri
