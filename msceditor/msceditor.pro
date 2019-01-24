TEMPLATE = app
TARGET = msceditor

include(../esa.pri)
include(../depend_asn1library.pri)
include(../depend_mscwidgets.pri)
include(../depend_msclibrary.pri)

QT += core gui widgets xml

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmodel.cpp \
    commandlineparser.cpp \
    settings/settingsmanager.cpp \
    settings/appoptions.cpp \

HEADERS += \
    mainwindow.h \
    mainmodel.h \
    commandlineparser.h \
    settings/settingsmanager.h \
    settings/appoptions.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
