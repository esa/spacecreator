include(../../../esa.pri)
include(../../../depend_asn1library.pri)
include(../../../depend_mscwidgets.pri)
include(../../../depend_msclibrary.pri)

QT += core gui widgets xml network websockets

SOURCES += \
    ../../../../msceditor/remotecontrolwebserver.cpp \
    ../../../../msceditor/mainwindow.cpp \
    ../../../../msceditor/mainmodel.cpp \
    ../../../../msceditor/commandlineparser.cpp \
    ../../../../msceditor/settings/settingsmanager.cpp \
    ../../../../msceditor/settings/appoptions.cpp \

HEADERS += \
    ../../../../msceditor/remotecontrolwebserver.h \
    ../../../../msceditor/mainwindow.h \
    ../../../../msceditor/mainmodel.h \
    ../../../../msceditor/commandlineparser.h \
    ../../../../msceditor/settings/settingsmanager.h \
    ../../../../msceditor/settings/appoptions.h \

FORMS += \
    ../../../../msceditor/mainwindow.ui

INCLUDEPATH += ../../../../msceditor

win32 {
    DEFINES += EXAMPLES_STORAGE_PATH=\\\"$$_PRO_FILE_PWD_/../../../../examples/\\\"
} else:unix {
    DEFINES += EXAMPLES_STORAGE_PATH=\\\"$$_PRO_FILE_PWD_/../../../../examples/\\\"
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
