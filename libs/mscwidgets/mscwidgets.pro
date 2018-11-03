TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    graphicsview.h \
    instanceitem.h \
    messageitem.h \
    graphicsview.h \
    instanceitem.h \
    messageitem.h \
    baseitems/arrowitem.h \
    baseitems/textitem.h \
    baseitems/grippointshandler.h \
    baseitems/grippoint.h \
    baseitems/interactiveobject.h \
    baseitems/common/drawrectinfo.h \
    baseitems/common/abstractinteractiveobject.h

SOURCES += \
    graphicsview.cpp \
    instanceitem.cpp \
    messageitem.cpp \
    graphicsview.cpp \
    instanceitem.cpp \
    messageitem.cpp \
    baseitems/arrowitem.cpp \
    baseitems/textitem.cpp \
    baseitems/grippoint.cpp \
    baseitems/grippointshandler.cpp \
    baseitems/interactiveobject.cpp \
    baseitems/common/drawrectinfo.cpp

BuildType =release
CONFIG(debug, debug|release) {
    BuildType =debug
} # Profiling, etc (if any) are defaulted to Release

win32 {
    LIBS += -L$$OUT_PWD/../msclibrary/$$BuildType/ -lmsclibrary
    MSC_LIB_NAME=msclibrary.lib
    win32-g++: MSC_LIB_NAME=libmsclibrary.a
    PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/$$BuildType/$$MSC_LIB_NAME
} else:unix {
    LIBS += -L$$OUT_PWD/../msclibrary/ -lmsclibrary
    PRE_TARGETDEPS += $$OUT_PWD/../msclibrary/libmsclibrary.a
}

INCLUDEPATH += $$PWD/../msclibrary
DEPENDPATH += $$PWD/../msclibrary
