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
        app/commandlineparser.cpp \
        app/commandsstack.cpp \
        app/common.cpp \
        app/mainwindow.cpp \
        baseitems/common/arrowsign.cpp \
        baseitems/common/drawrectinfo.cpp \
        baseitems/common/highlightrectitem.cpp \
        baseitems/common/objectanchor.cpp \
        baseitems/common/objectslink.cpp \
        baseitems/common/utils.cpp \
        baseitems/graphicsview.cpp \
        baseitems/grippoint.cpp \
        baseitems/grippointshandler.cpp \
        baseitems/interactiveobject.cpp \
        baseitems/textgraphicsitem.cpp \
        document/abstracttabdocument.cpp \
        document/documentsmanager.cpp \
        document/tabdocumentfactory.cpp \
        dummy/dummytabdocument.cpp \
        logging/loghandler.cpp \
        main.cpp \
        reports/bugreportdialog.cpp \
        reports/bugreporthandler.cpp \
        settings/appoptions.cpp \
        settings/settingsmanager.cpp \
        tab_aadl/aadlobject.cpp \
        tab_aadl/aadlobjectcomment.cpp \
        tab_aadl/aadlobjectconnection.cpp \
        tab_aadl/aadlobjectcontainer.cpp \
        tab_aadl/aadlobjectfunction.cpp \
        tab_aadl/aadlobjectiface.cpp \
        tab_aadl/aadlobjectsmodel.cpp \
        tab_aadl/aadltabdocument.cpp \
        tab_aadl/aadlxmlreader.cpp \
        tab_concurrency/concurrencytabdocument.cpp \
        tab_data/datatabdocument.cpp \
        tab_deployment/deploymenttabdocument.cpp \
        tab_interface/aadlcommentgraphicsitem.cpp \
        tab_interface/aadlconnectiongraphicsitem.cpp \
        tab_interface/aadlcontainergraphicsitem.cpp \
        tab_interface/aadlfunctiongraphicsitem.cpp \
        tab_interface/aadlinterfacegraphicsitem.cpp \
        tab_interface/commands/cmdcommentitemcreate.cpp \
        tab_interface/commands/cmdcommenttextchange.cpp \
        tab_interface/commands/cmdcontaineritemcreate.cpp \
        tab_interface/commands/cmddirectconnectionitemcreate.cpp \
        tab_interface/commands/cmdentityattributechange.cpp \
        tab_interface/commands/cmdentitygeometrychange.cpp \
        tab_interface/commands/cmdentitypropertychange.cpp \
        tab_interface/commands/cmdentityremove.cpp \
        tab_interface/commands/cmdfunctionitemcreate.cpp \
        tab_interface/commands/cmdmanualconnectionitemcreate.cpp \
        tab_interface/commands/cmdprovidedinterfaceitemcreate.cpp \
        tab_interface/commands/cmdrequiredinterfaceitemcreate.cpp \
        tab_interface/commands/commandsfactory.cpp \
        tab_interface/creatortool.cpp \
        tab_interface/interfacetabdocument.cpp \
        tab_interface/interfacetabgraphicsscene.cpp \
        tab_msc/msctabdocument.cpp

HEADERS += \
        app/commandlineparser.h \
        app/commandsstack.h \
        app/common.h \
        app/mainwindow.h \
        baseitems/common/abstractinteractiveobject.h \
        baseitems/common/arrowsign.h \
        baseitems/common/drawrectinfo.h \
        baseitems/common/highlightrectitem.h \
        baseitems/common/objectanchor.h \
        baseitems/common/objectslink.h \
        baseitems/common/utils.h \
        baseitems/graphicsview.h \
        baseitems/grippoint.h \
        baseitems/grippointshandler.h \
        baseitems/interactiveobject.h \
        baseitems/textgraphicsitem.h \
        document/abstracttabdocument.h \
        document/documentsmanager.h \
        document/tabdocumentfactory.h \
        dummy/dummytabdocument.h \
        logging/logcategory.h \
        logging/loghandler.h \
        reports/bugreportdialog.h \
        reports/bugreporthandler.h \
        settings/appoptions.h \
        settings/settingsmanager.h \
        tab_aadl/aadlcommonprops.h \
        tab_aadl/aadlobject.h \
        tab_aadl/aadlobjectcomment.h \
        tab_aadl/aadlobjectconnection.h \
        tab_aadl/aadlobjectcontainer.h \
        tab_aadl/aadlobjectfunction.h \
        tab_aadl/aadlobjectiface.h \
        tab_aadl/aadlobjectsmodel.h \
        tab_aadl/aadltabdocument.h \
        tab_aadl/aadlxmlreader.h \
        tab_concurrency/concurrencytabdocument.h \
        tab_data/datatabdocument.h \
        tab_deployment/deploymenttabdocument.h \
        tab_interface/aadlcommentgraphicsitem.h \
        tab_interface/aadlconnectiongraphicsitem.h \
        tab_interface/aadlcontainergraphicsitem.h \
        tab_interface/aadlfunctiongraphicsitem.h \
        tab_interface/aadlinterfacegraphicsitem.h \
        tab_interface/commands/cmdcommentitemcreate.h \
        tab_interface/commands/cmdcommenttextchange.h \
        tab_interface/commands/cmdcontaineritemcreate.h \
        tab_interface/commands/cmddirectconnectionitemcreate.h \
        tab_interface/commands/cmdentityattributechange.h \
        tab_interface/commands/cmdentitygeometrychange.h \
        tab_interface/commands/cmdentitypropertychange.h \
        tab_interface/commands/cmdentityremove.h \
        tab_interface/commands/cmdfunctionitemcreate.h \
        tab_interface/commands/cmdmanualconnectionitemcreate.h \
        tab_interface/commands/cmdprovidedinterfaceitemcreate.h \
        tab_interface/commands/cmdrequiredinterfaceitemcreate.h \
        tab_interface/commands/commandids.h \
        tab_interface/commands/commandsfactory.h \
        tab_interface/creatortool.h \
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
