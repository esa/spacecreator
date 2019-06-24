#-------------------------------------------------
#
# Project created by QtCreator 2019-06-18T13:46:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TASTEeditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(version.pri)
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT
macx {
    CONFIG -= app_bundle
}

SOURCES += \
        app/commandlineparser.cpp \
        app/common.cpp \
        main.cpp \
        loghandler.cpp \
        app/commandsstack.cpp \
        app/mainwindow.cpp \
        document/abstracttabdocument.cpp \
        document/documentsmanager.cpp \
        dummy/dummytabdocument.cpp \
        settings/appoptions.cpp \
        settings/settingsmanager.cpp \
        tab_aadl/aadlobject.cpp \
        tab_aadl/aadltabdocument.cpp \
        tab_concurrency/concurrencytabdocument.cpp \
        tab_data/datatabdocument.cpp \
        tab_deployment/deploymenttabdocument.cpp \
        tab_interface/interfacetabdocument.cpp \
        tab_interface/interfacetabgraphicsscene.cpp \
        tab_msc/msctabdocument.cpp

HEADERS += \
        app/commandlineparser.h \
        app/common.h \
        logcategory.h \
        loghandler.h \
        app/commandsstack.h \
        app/mainwindow.h \
        document/abstracttabdocument.h \
        document/documentsmanager.h \
        dummy/dummytabdocument.h \
        settings/appoptions.h \
        settings/settingsmanager.h \
        tab_aadl/aadlobject.h \
        tab_aadl/aadltabdocument.h \
        tab_concurrency/concurrencytabdocument.h \
        tab_data/datatabdocument.h \
        tab_deployment/deploymenttabdocument.h \
        tab_interface/interfacetabdocument.h \
        tab_interface/interfacetabgraphicsscene.h \
        tab_msc/msctabdocument.h

FORMS += \
    app/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    version.pri

RESOURCES += \
    resources.qrc \
    tab_interface/tab_interface_resources.qrc
