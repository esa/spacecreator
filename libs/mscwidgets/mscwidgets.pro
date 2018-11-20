TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    baseitems/common/abstractinteractiveobject.h \
    baseitems/common/arrowsign.h \
    baseitems/common/drawrectinfo.h \
    baseitems/common/highlightrectitem.h \
    baseitems/common/objectanchor.h \
    baseitems/common/objectslink.h \
    baseitems/common/utils.h \
    baseitems/arrowitem.h \
    baseitems/grippoint.h \
    baseitems/grippointshandler.h \
    baseitems/interactiveobject.h \
    baseitems/labeledarrowitem.h \
    baseitems/objectslinkitem.h \
    baseitems/textitem.h \
    commands/common/commandids.h \
    commands/common/commandsfactory.h \
    commands/common/commandsstack.h \
    commands/basecommand.h \
    commands/cmdinstanceitemmove.h \
    commands/cmdinstanceitemresize.h \
    commands/cmdmessageitemmove.h \
    commands/cmdmessageitemresize.h \
    chartviewmodel.h \
    graphicsview.h \
    instanceitem.h \
    messageitem.h \
    commands/cmdmessageitemcreate.h \
    commands/cmdinstanceitemcreate.h

SOURCES += \
    baseitems/common/arrowsign.cpp \
    baseitems/common/drawrectinfo.cpp \
    baseitems/common/highlightrectitem.cpp \
    baseitems/common/objectanchor.cpp \
    baseitems/common/objectslink.cpp \
    baseitems/common/utils.cpp \
    baseitems/arrowitem.cpp \
    baseitems/grippoint.cpp \
    baseitems/grippointshandler.cpp \
    baseitems/interactiveobject.cpp \
    baseitems/labeledarrowitem.cpp \
    baseitems/objectslinkitem.cpp \
    baseitems/textitem.cpp \
    commands/common/commandsfactory.cpp \
    commands/common/commandsstack.cpp \
    commands/basecommand.cpp \
    commands/cmdinstanceitemmove.cpp \
    commands/cmdinstanceitemresize.cpp \
    commands/cmdmessageitemmove.cpp \
    commands/cmdmessageitemresize.cpp \
    chartviewmodel.cpp \
    graphicsview.cpp \
    instanceitem.cpp \
    messageitem.cpp \
    commands/cmdmessageitemcreate.cpp \
    commands/cmdinstanceitemcreate.cpp

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
