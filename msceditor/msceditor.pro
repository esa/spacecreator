TEMPLATE = app
TARGET = msceditor

include(../esa.pri)

QT += core gui widgets xml

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmodel.cpp \
    tools/basetool.cpp \
    tools/pointertool.cpp \
    tools/instancecreatortool.cpp \
    tools/messagecreatortool.cpp \
    tools/basecreatortool.cpp \
    commandlineparser.cpp

HEADERS += \
    mainwindow.h \
    mainmodel.h \
    tools/basetool.h \
    tools/tooltypes.h \
    tools/pointertool.h \
    tools/instancecreatortool.h \
    tools/messagecreatortool.h \
    tools/basecreatortool.h \
    commandlineparser.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    $$PWD/../libs/asn1editor \
    $$PWD/../libs/mscwidgets \
    $$PWD/../libs/msclibrary


DEPENDPATH += \
    $$PWD/../libs/asn1editor \
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
        $$OUT_PWD/../libs/asn1editor/$$BuildTypeLower/$${LibPrefix}asn1editor.$$LibSuffix \
        $$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += \
        -L$$OUT_PWD/../libs/asn1editor/$$BuildTypeLower/ -lasn1editor \
        -L$$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/ -lmscwidgets \
        -L$$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/ -lmsclibrary
} else:unix {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/asn1editor/$${LibPrefix}asn1editor.$$LibSuffix \
        $$OUT_PWD/../libs/mscwidgets/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../libs/msclibrary/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += \
        -L$$OUT_PWD/../libs/asn1editor/ -lasn1editor \
        -L$$OUT_PWD/../libs/msclibrary/ -lmsclibrary \
        -L$$OUT_PWD/../libs/mscwidgets/ -lmscwidgets
}

# Add ANTLR runtime
include("../link_antlr_rt.pri")
win32: QMAKE_LFLAGS += /LTCG

RESOURCES += \
    resources/rsc.qrc
