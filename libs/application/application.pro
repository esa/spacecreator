TEMPLATE = lib
TARGET = application

include(../../esa.pri)
include(../../depend_asn1library.pri)
include(../../depend_msclibrary.pri)
include(../../depend_mscwidgets.pri)

CONFIG += static

QT += core gui widgets network websockets

HEADERS += \
    settings/settingsmanager.h \
    settings/appoptions.h \
    asn1fileview.h \
    documenttreeview.h \
    documentitemmodel.h \
    mainmodel.h \

SOURCES += \
    settings/settingsmanager.cpp \
    settings/appoptions.cpp \
    asn1fileview.cpp \
    documenttreeview.cpp \
    documentitemmodel.cpp \
    mainmodel.cpp \

FORMS += \
    asn1fileview.ui \
