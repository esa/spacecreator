TEMPLATE = app
TARGET = msceditor

include(../esa.pri)

QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmodel.cpp

HEADERS += \
    mainwindow.h \
    mainmodel.h

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

win32 {
    WinLibSuffix = lib
    win32-g++: WinLibSuffix = a
    #TODO: check if the win32-g++ needs the lib prefix

    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/mscwidgets.$$WinLibSuffix \
        $$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/msclibrary.$$WinLibSuffix

    LIBS += -L$$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/ -lmscwidgets
    LIBS += -L$$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/ -lmsclibrary
} else:unix {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/libmscwidgets.a \
        $$OUT_PWD/../libs/msclibrary/libmsclibrary.a

    LIBS += -L$$OUT_PWD/../libs/mscwidgets/ -lmscwidgets
    LIBS += -L$$OUT_PWD/../libs/msclibrary/ -lmsclibrary
}

# Add ANTLR runtime
include("../link_antlr_rt.pri")
