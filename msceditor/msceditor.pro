TEMPLATE = app
TARGET = msceditor

include(../esa.pri)

QT += core gui widgets xml

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmodel.cpp \
    asn1editor.cpp \
    asn1editor/xmlparser.cpp

HEADERS += \
    mainwindow.h \
    mainmodel.h \
    asn1editor.h \
    asn1editor/xmlparser.h

FORMS += \
    mainwindow.ui \
    asn1editor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    $$PWD/asn1editor \
    $$PWD/../libs/mscwidgets \
    $$PWD/../libs/msclibrary

VPATH += $$PWD/asn1editor

DEPENDPATH += \
    $$PWD/asn1editor \
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

    LIBS += -L$$OUT_PWD/../libs/mscwidgets/$$BuildTypeLower/ -lmscwidgets
    LIBS += -L$$OUT_PWD/../libs/msclibrary/$$BuildTypeLower/ -lmsclibrary
} else:unix {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../libs/mscwidgets/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../libs/msclibrary/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += -L$$OUT_PWD/../libs/mscwidgets/ -lmscwidgets
    LIBS += -L$$OUT_PWD/../libs/msclibrary/ -lmsclibrary
}

# Add ANTLR runtime
include("../link_antlr_rt.pri")

CONFIG(debug, debug|release) {
    DEFINES += DEVELOPER_AUTO_OPEN_MSC=\\\"$${_PRO_FILE_PWD_}/../examples/\\\"
}
