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

GRANTLEE_LIB_DIR = $$[QT_INSTALL_LIBS]

DEFINES += GRANTLEE_LIB_DIR='\\"$$GRANTLEE_LIB_DIR\\"'
LIBS += -L/$$GRANTLEE_LIB_DIR -lGrantlee_Templates

SOURCES += \
        app/commandlineparser.cpp \
        app/commandsstack.cpp \
        app/common.cpp \
        app/context/action/action.cpp \
        app/context/action/actionsmanager.cpp \
        app/context/action/attrhandler.cpp \
        app/context/action/condition.cpp \
        app/context/action/editor/actionsmodel.cpp \
        app/context/action/editor/attributesmodel.cpp \
        app/context/action/editor/conditiondialog.cpp \
        app/context/action/editor/conditionsmodel.cpp \
        app/context/action/editor/dynactioneditor.cpp \
        app/context/action/extprocmonitor.cpp \
        app/datatypes/basicdatatype.cpp \
        app/datatypes/datatypesstorage.cpp \
        app/mainwindow.cpp \
        baseitems/clicknotifieritem.cpp \
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
        baseitems/skinnablegraphicsobject.cpp \
        baseitems/textgraphicsitem.cpp \
        document/abstracttabdocument.cpp \
        document/documentsmanager.cpp \
        document/tabdocumentfactory.cpp \
        dummy/dummytabdocument.cpp \
        logging/logcategory.cpp \
        logging/loghandler.cpp \
        main.cpp \
        templating/previewdialog.cpp \
        templating/stringtemplate.cpp \
        templating/templatehighlighter.cpp \
        templating/xmlhighlighter.cpp \
        reports/bugreportdialog.cpp \
        reports/bugreporthandler.cpp \
        settings/appoptions.cpp \
        settings/settingsmanager.cpp \
        tab_aadl/aadlcommonprops.cpp \
        tab_aadl/aadlobject.cpp \
        tab_aadl/aadlobjectcomment.cpp \
        tab_aadl/aadlobjectconnection.cpp \
        tab_aadl/aadlobjectfunction.cpp \
        tab_aadl/aadlobjectfunctiontype.cpp \
        tab_aadl/aadlobjectiface.cpp \
        tab_aadl/aadlobjectsmodel.cpp \
        tab_aadl/aadlparameter.cpp \
        tab_aadl/aadltabdocument.cpp \
        tab_aadl/aadlxmlreader.cpp \
        tab_concurrency/concurrencytabdocument.cpp \
        tab_data/datatabdocument.cpp \
        tab_deployment/deploymenttabdocument.cpp \
        tab_interface/aadlcommentgraphicsitem.cpp \
        tab_interface/aadlconnectiongraphicsitem.cpp \
        tab_interface/aadlfunctiongraphicsitem.cpp \
        tab_interface/aadlfunctionnamegraphicsitem.cpp \
        tab_interface/aadlfunctiontypegraphicsitem.cpp \
        tab_interface/aadlinterfacegraphicsitem.cpp \
        tab_interface/colors/colorhandlereditor.cpp \
        tab_interface/colors/colormanager.cpp \
        tab_interface/colors/colormanagerdialog.cpp \
        tab_interface/colors/colorselectorbutton.cpp \
        tab_interface/commands/cmdcommentitemcreate.cpp \
        tab_interface/commands/cmdcommenttextchange.cpp \
        tab_interface/commands/cmdconnectionitemcreate.cpp \
        tab_interface/commands/cmdcontextparameterchange.cpp \
        tab_interface/commands/cmdcontextparametercreate.cpp \
        tab_interface/commands/cmdcontextparameterremove.cpp \
        tab_interface/commands/cmdentityattributechange.cpp \
        tab_interface/commands/cmdentitygeometrychange.cpp \
        tab_interface/commands/cmdentitypropertychange.cpp \
        tab_interface/commands/cmdentitypropertycreate.cpp \
        tab_interface/commands/cmdentitypropertyremove.cpp \
        tab_interface/commands/cmdentitypropertyrename.cpp \
        tab_interface/commands/cmdentityremove.cpp \
        tab_interface/commands/cmdfunctionitemcreate.cpp \
        tab_interface/commands/cmdfunctiontypeitemcreate.cpp \
        tab_interface/commands/cmdifaceparamchange.cpp \
        tab_interface/commands/cmdifaceparamcreate.cpp \
        tab_interface/commands/cmdifaceparamremove.cpp \
        tab_interface/commands/cmdinterfaceitemcreate.cpp \
        tab_interface/commands/commandsfactory.cpp \
        tab_interface/creatortool.cpp \
        tab_interface/interfacetabdocument.cpp \
        tab_interface/interfacetabgraphicsscene.cpp \
        tab_interface/properties/adddynamicropertydialog.cpp \
        tab_interface/properties/contextparametersmodel.cpp \
        tab_interface/properties/delegates/comboboxdelegate.cpp \
        tab_interface/properties/delegates/functionattrdelegate.cpp \
        tab_interface/properties/delegates/propertytypedelegate.cpp \
        tab_interface/properties/dynamicproperty.cpp \
        tab_interface/properties/dynamicpropertyconfig.cpp \
        tab_interface/properties/dynamicpropertymanager.cpp \
        tab_interface/properties/ifaceparametersmodel.cpp \
        tab_interface/properties/propertiesdialog.cpp \
        tab_interface/properties/propertieslistmodel.cpp \
        tab_interface/properties/propertiesmodelbase.cpp \
        tab_interface/properties/propertiesviewbase.cpp \
        tab_msc/msctabdocument.cpp

HEADERS += \
        app/commandlineparser.h \
        app/commandsstack.h \
        app/common.h \
        app/context/action/action.h \
        app/context/action/actionsmanager.h \
        app/context/action/attrhandler.h \
        app/context/action/condition.h \
        app/context/action/editor/actionsmodel.h \
        app/context/action/editor/attributesmodel.h \
        app/context/action/editor/conditiondialog.h \
        app/context/action/editor/conditionsmodel.h \
        app/context/action/editor/dynactioneditor.h \
        app/context/action/extprocmonitor.h \
        app/datatypes/basicdatatype.h \
        app/datatypes/datatypesstorage.h \
        app/mainwindow.h \
        baseitems/clicknotifieritem.h \
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
        baseitems/skinnablegraphicsobject.h \
        baseitems/textgraphicsitem.h \
        document/abstracttabdocument.h \
        document/documentsmanager.h \
        document/tabdocumentfactory.h \
        dummy/dummytabdocument.h \
        logging/logcategory.h \
        logging/loghandler.h \
        templating/previewdialog.h \
        templating/stringtemplate.h \
        templating/templatehighlighter.h \
        templating/xmlhighlighter.h \
        reports/bugreportdialog.h \
        reports/bugreporthandler.h \
        settings/appoptions.h \
        settings/settingsmanager.h \
        tab_aadl/aadlcommonprops.h \
        tab_aadl/aadlobject.h \
        tab_aadl/aadlobjectcomment.h \
        tab_aadl/aadlobjectconnection.h \
        tab_aadl/aadlobjectfunction.h \
        tab_aadl/aadlobjectfunctiontype.h \
        tab_aadl/aadlobjectiface.h \
        tab_aadl/aadlobjectsmodel.h \
        tab_aadl/aadlparameter.h \
        tab_aadl/aadltabdocument.h \
        tab_aadl/aadlxmlreader.h \
        tab_concurrency/concurrencytabdocument.h \
        tab_data/datatabdocument.h \
        tab_deployment/deploymenttabdocument.h \
        tab_interface/aadlcommentgraphicsitem.h \
        tab_interface/aadlconnectiongraphicsitem.h \
        tab_interface/aadlfunctiongraphicsitem.h \
        tab_interface/aadlfunctionnamegraphicsitem.h \
        tab_interface/aadlfunctiontypegraphicsitem.h \
        tab_interface/aadlinterfacegraphicsitem.h \
        tab_interface/colors/colorhandlereditor.h \
        tab_interface/colors/colormanager.h \
        tab_interface/colors/colormanagerdialog.h \
        tab_interface/colors/colorselectorbutton.h \
        tab_interface/commands/cmdcommentitemcreate.h \
        tab_interface/commands/cmdcommenttextchange.h \
        tab_interface/commands/cmdconnectionitemcreate.h \
        tab_interface/commands/cmdcontextparameterchange.h \
        tab_interface/commands/cmdcontextparametercreate.h \
        tab_interface/commands/cmdcontextparameterremove.h \
        tab_interface/commands/cmdentityattributechange.h \
        tab_interface/commands/cmdentitygeometrychange.h \
        tab_interface/commands/cmdentitypropertychange.h \
        tab_interface/commands/cmdentitypropertycreate.h \
        tab_interface/commands/cmdentitypropertyremove.h \
        tab_interface/commands/cmdentitypropertyrename.h \
        tab_interface/commands/cmdentityremove.h \
        tab_interface/commands/cmdfunctionitemcreate.h \
        tab_interface/commands/cmdfunctiontypeitemcreate.h \
        tab_interface/commands/cmdifaceparamchange.h \
        tab_interface/commands/cmdifaceparamcreate.h \
        tab_interface/commands/cmdifaceparamremove.h \
        tab_interface/commands/cmdinterfaceitemcreate.h \
        tab_interface/commands/commandids.h \
        tab_interface/commands/commandsfactory.h \
        tab_interface/creatortool.h \
        tab_interface/interfacetabdocument.h \
        tab_interface/interfacetabgraphicsscene.h \
        tab_interface/properties/adddynamicpropertydialog.h \
        tab_interface/properties/contextparametersmodel.h \
        tab_interface/properties/delegates/comboboxdelegate.h \
        tab_interface/properties/delegates/functionattrdelegate.h \
        tab_interface/properties/delegates/propertytypedelegate.h \
        tab_interface/properties/dynamicproperty.h \
        tab_interface/properties/dynamicpropertyconfig.h \
        tab_interface/properties/dynamicpropertymanager.h \
        tab_interface/properties/ifaceparametersmodel.h \
        tab_interface/properties/propertiesdialog.h \
        tab_interface/properties/propertieslistmodel.h \
        tab_interface/properties/propertiesmodelbase.h \
        tab_interface/properties/propertiesviewbase.h \
        tab_msc/msctabdocument.h

FORMS += \
    app/context/action/editor/conditiondialog.ui \
    app/context/action/editor/dynactioneditor.ui \
    app/mainwindow.ui \
    reports/bugreportdialog.ui \
    tab_interface/colors/colorhandlereditor.ui \
    tab_interface/colors/colormanagerdialog.ui \
    tab_interface/properties/adddynamicpropertydialog.ui \
    tab_interface/properties/dynamicpropertymanager.ui \
    tab_interface/properties/propertiesdialog.ui \
    tab_interface/properties/propertiesviewbase.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    version.pri

RESOURCES += \
    resources.qrc \
    tab_interface/tab_interface_resources.qrc

OTHER_FILES += templating/xml_templates/*.tmplt

SOURCE_DIR = $$absolute_path(templating/xml_templates)
DEST_DIR = $$OUT_PWD

win32 {
    SOURCE_DIR ~= s,/,\\,g
    DEST_DIR ~= s,/,\\,g
}

QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$SOURCE_DIR) $$shell_quote($$DEST_DIR)
