TEMPLATE = app
TARGET = msceditor

include(../esa.pri)
include(../depend_application.pri)
include(../depend_mscwidgets.pri)
include(../depend_msclibrary.pri)
include(../depend_asn1library.pri)

QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    commandlineparser.cpp \

HEADERS += \
    mainwindow.h \
    commandlineparser.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
