include(../../../../esa.pri)

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_mscfile

SOURCES +=  tst_mscfile.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../libs/msclibrary/release/ -lmsclibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../libs/msclibrary/debug/ -lmsclibrary
else:unix: LIBS += -L$$OUT_PWD/../../../../libs/msclibrary/ -lmsclibrary

INCLUDEPATH += $$PWD/../../../../libs/msclibrary
DEPENDPATH += $$PWD/../../../../libs/msclibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/release/libmsclibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/debug/libmsclibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/release/msclibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/debug/msclibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/libmsclibrary.a
