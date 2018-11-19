TEMPLATE = app
TARGET = msceditor

include(../esa.pri)

QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmodel.cpp \
    tools/basetool.cpp \
    tools/pointertool.cpp \
    tools/instancecreatortool.cpp \
    tools/messagecreatortool.cpp \
    tools/basecreatortool.cpp

HEADERS += \
    mainwindow.h \
    mainmodel.h \
    tools/basetool.h \
    tools/tooltypes.h \
    tools/pointertool.h \
    tools/instancecreatortool.h \
    tools/messagecreatortool.h \
    tools/basecreatortool.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    $$PWD/../libs/mscwidgets \
    $$PWD/../libs/msclibrary

DEPENDPATH += \
    $$PWD/../libs/mscwidgets \
    $$PWD/../libs/msclibrary


BuildType = Release
CONFIG(debug, debug|release) {
    BuildType = Debug
} # Profiling, etc (if any) are defaulted to Release
BuildTypeLower = $$lower($$BuildType)

LibSuffix = a
LibPrefix = lib

win32:!win32-g++ {
    LibSuffix = lib
    LibPrefix = ""
}

win32 {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += \
        -L$$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/ -lmscwidgets \
        -L$$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/ -lmsclibrary
} else:unix {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../libs/msclibrary/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += \
        -L$$OUT_PWD/../libs/msclibrary/ -lmsclibrary \
        -L$$OUT_PWD/../libs/mscwidgets/ -lmscwidgets
}

# Add ANTLR runtime
include("../link_antlr_rt.pri")

CONFIG(debug, debug|release) {
    DEFINES += DEVELOPER_AUTO_OPEN_MSC=\\\"$${_PRO_FILE_PWD_}/../examples/\\\"
}
