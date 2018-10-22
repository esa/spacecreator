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
    WinLinbSuffix = lib
    win32-g++: WinLinbSuffix = a
    #TODO: check if the win32-g++ needs the lib prefix

    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/mscwidgets.$$WinLinbSuffix \
        $$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/msclibrary.$$WinLinbSuffix

    LIBS += -L$$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/ -lmscwidgets
    LIBS += -L$$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/ -lmsclibrary
    LIBS += -L$$clean_path("$$PWD/../3rdparty/antlr/cpp_runtime/runtime/bin/vs-2017/x64/$$BuildType Static") -lantlr4-runtime
} else:unix {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/libmscwidgets.a \
        $$OUT_PWD/../libs/msclibrary/libmsclibrary.a

    LIBS += -L$$OUT_PWD/../libs/mscwidgets/ -lmscwidgets
    LIBS += -L$$OUT_PWD/../libs/msclibrary/ -lmsclibrary
    osx: LIBS += -L$$clean_path($$PWD/../3rdparty/antlr/cpp_runtime/lib) -lantlr4-runtime_macos_x64
    else: LIBS += -L$$clean_path($$PWD/../3rdparty/antlr/cpp_runtime/dist) -lantlr4-runtime_linux_x64
}
