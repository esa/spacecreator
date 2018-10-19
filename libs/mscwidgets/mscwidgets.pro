TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    graphicsview.h \
    instanceitem.h \
    messageitem.h

SOURCES += \
    graphicsview.cpp \
    instanceitem.cpp \
    messageitem.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../msclibrary/release/ -lmsclibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../msclibrary/debug/ -lmsclibrary
else:unix: LIBS += -L$$OUT_PWD/../msclibrary/ -lmsclibrary

INCLUDEPATH += $$PWD/../msclibrary
DEPENDPATH += $$PWD/../msclibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/release/libmsclibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/debug/libmsclibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/release/msclibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/debug/msclibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/libmsclibrary.a
