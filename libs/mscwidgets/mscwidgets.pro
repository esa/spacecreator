TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    graphicsview.h \
    instanceitem.h \
    messageitem.h \
    chartviewmodel.h \
    baseitems/arrowitem.h \
    baseitems/textitem.h \
    baseitems/grippointshandler.h \
    baseitems/grippoint.h \
    baseitems/interactiveobject.h \
    baseitems/objectslinkitem.h \
    baseitems/labeledarrowitem.h \
    baseitems/common/drawrectinfo.h \
    baseitems/common/abstractinteractiveobject.h \
    baseitems/common/arrowsign.h \
    baseitems/common/utils.h \
    baseitems/common/objectanchor.h \
    baseitems/common/objectslink.h \
    baseitems/common/highlightrectitem.h

SOURCES += \
    graphicsview.cpp \
    instanceitem.cpp \
    messageitem.cpp \
    chartviewmodel.cpp \
    baseitems/arrowitem.cpp \
    baseitems/textitem.cpp \
    baseitems/grippoint.cpp \
    baseitems/grippointshandler.cpp \
    baseitems/common/drawrectinfo.cpp \
    baseitems/objectslinkitem.cpp \
    baseitems/interactiveobject.cpp \
    baseitems/labeledarrowitem.cpp \
    baseitems/common/arrowsign.cpp \
    baseitems/common/utils.cpp \
    baseitems/common/objectanchor.cpp \
    baseitems/common/objectslink.cpp \
    baseitems/common/highlightrectitem.cpp

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

RESOURCES += \
    resources.qrc
