TEMPLATE = lib
TARGET = application

include(../../esa.pri)
include(../../depend_asn1library.pri)
include(../../depend_msclibrary.pri)
include(../../depend_mscwidgets.pri)

CONFIG += static

QT += core gui widgets network websockets

HEADERS += \
    settings/appoptions.h \
    settings/settingsmanager.h \
    asn1fileview.h \
    documenttreeview.h \
    documentitemmodel.h \
    mainmodel.h \
    remotecontrolwebserver.h \

SOURCES += \
    settings/appoptions.cpp \
    settings/settingsmanager.cpp \
    asn1fileview.cpp \
    documenttreeview.cpp \
    documentitemmodel.cpp \
    mainmodel.cpp \
    remotecontrolwebserver.cpp \

FORMS += \
    asn1fileview.ui \
