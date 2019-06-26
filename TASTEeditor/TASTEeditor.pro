#-------------------------------------------------
#
# Project created by QtCreator 2019-06-18T13:46:51
#
#-------------------------------------------------

QT       += core gui network concurrent

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
        baseitems/common/arrowsign.cpp \
        baseitems/common/drawrectinfo.cpp \
        baseitems/common/highlightrectitem.cpp \
        baseitems/common/objectanchor.cpp \
        baseitems/common/objectslink.cpp \
        baseitems/common/utils.cpp \
        baseitems/grippoint.cpp \
        baseitems/grippointshandler.cpp \
        baseitems/interactiveobject.cpp \
        main.cpp \
        app/commandlineparser.cpp \
        app/common.cpp \
        app/commandsstack.cpp \
        app/mainwindow.cpp \
        document/abstracttabdocument.cpp \
        document/documentsmanager.cpp \
        dummy/dummytabdocument.cpp \
        logging/loghandler.cpp \
        reports/bugreportdialog.cpp \
        reports/bugreporthandler.cpp \
        settings/appoptions.cpp \
        settings/settingsmanager.cpp \
        tab_aadl/aadlobject.cpp \
        tab_aadl/aadlobjectcontainer.cpp \
        tab_aadl/aadlobjectfunction.cpp \
        tab_aadl/aadlobjectiface.cpp \
        tab_aadl/aadlobjectsmodel.cpp \
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
        baseitems/common/abstractinteractiveobject.h \
        baseitems/common/arrowsign.h \
        baseitems/common/drawrectinfo.h \
        baseitems/common/highlightrectitem.h \
        baseitems/common/objectanchor.h \
        baseitems/common/objectslink.h \
        baseitems/common/utils.h \
        baseitems/grippoint.h \
        baseitems/grippointshandler.h \
        baseitems/interactiveobject.h \
        app/commandsstack.h \
        app/mainwindow.h \
        document/abstracttabdocument.h \
        document/documentsmanager.h \
        dummy/dummytabdocument.h \
        logging/logcategory.h \
        logging/loghandler.h \
        reports/bugreportdialog.h \
        reports/bugreporthandler.h \
        settings/appoptions.h \
        settings/settingsmanager.h \
        tab_aadl/aadlobject.h \
        tab_aadl/aadlobjectcontainer.h \
        tab_aadl/aadlobjectfunction.h \
        tab_aadl/aadlobjectiface.h \
        tab_aadl/aadlobjectsmodel.h \
        tab_aadl/aadltabdocument.h \
        tab_concurrency/concurrencytabdocument.h \
        tab_data/datatabdocument.h \
        tab_deployment/deploymenttabdocument.h \
        tab_interface/interfacetabdocument.h \
        tab_interface/interfacetabgraphicsscene.h \
        tab_msc/msctabdocument.h

FORMS += \
    app/mainwindow.ui \
    reports/bugreportdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    version.pri

RESOURCES += \
    resources.qrc \
    tab_interface/tab_interface_resources.qrc
