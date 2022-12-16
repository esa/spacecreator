/*
   Copyright (C) 2021-2022 GMV - <tiago.jorge@gmv.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "spinconfigsaver.h"
#include "standardpaths.h"

#include <QCheckBox>
#include <QFileInfo>
#include <QInputDialog>
#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>
#include <functional>

namespace ive {

class InterfaceDocument;

/*!
 * \brief The ModelCheckingWindow class shows the Model Checking Window through the interface view
 */
class ModelCheckingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModelCheckingWindow(InterfaceDocument *document, const QString projectDir, QWidget *parent = nullptr);
    ~ModelCheckingWindow() override;
    void setVisible(bool visible) override;

public Q_SLOTS:
    void callTasteGens(bool toggled);

Q_SIGNALS:
    void visibleChanged(bool visible);

private Q_SLOTS:
    Qt::CheckState listProperties(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent, QStringList preSelection,
            QStringList expanded, int recLevel);
    QStringList getPropertiesSelection(QTreeWidgetItem *propertyWidgetItem, QStringList selections);
    QStringList getExpandedNodes(QTreeWidgetItem *propertyWidgetItem, QStringList expanded);
    bool isExpanded(QStringList expanded, QString dirName);
    Qt::CheckState getCheckState(QStringList selection, QString path);
    void listSubtypes(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent, QStringList preSelection);
    QStringList getSubtypesSelection();
    void listModelFunctions(QTreeWidgetItem *parentWidgetItem, QStringList preSelection);
    QStringList getFunctionsSelection();
    void listResults(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent);
    void listSpinResults(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent);
    void on_treeWidget_properties_itemChanged(QTreeWidgetItem *item, int column);
    void updateParentItem(QTreeWidgetItem *item);
    void on_treeWidget_properties_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_subtyping_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_subtyping_itemChanged(QTreeWidgetItem *item, int column);
    void on_pushButton_callIF_clicked();
    void on_pushButton_interactiveSim_clicked();
    void on_pushButton_exhaustiveSim_clicked();
    void on_treeWidget_results_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void convertToObs();
    void addProperty();
    void addSubtypes();
    void deleteSubtypes();
    void deleteProperty();
    void on_treeWidget_properties_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_subtyping_customContextMenuRequested(const QPoint &pos);
    void refreshView();
    void on_pushButton_saveConfiguration_clicked();
    bool saveConfiguration();
    void on_pushButton_loadConfiguration_clicked();
    void setPropertiesSelection(QStringList propertiesSelected);
    void setSubtypesSelection(QStringList subtypesSelected);
    void setFunctionsSelection(QStringList functionsSelected);
    void setSpinConfigParams(SpinConfigData spinConfig);
    void on_checkBox_errorScenarios_stateChanged(int arg1);
    void on_checkBox_successScenarios_stateChanged(int arg1);
    void addGenerationLimitsTableRow();
    void removeGenerationLimitsTableRow();
    void on_pushButton_callSpin_clicked();
    void processFinished(int, QProcess::ExitStatus);
    void processStderr();
    void processStdout();

private:
    QString propertiesPath;
    QString subtypesPath;
    QString configurationsPath;
    QString outputPath;
    QString spinOutputPath;
    QString projectDir;

    QMenu *contextMenuPropertiesTop;
    QMenu *contextMenuProperties;
    QMenu *contextMenuPropertiesBSC;
    QMenu *contextMenuPropertiesMSC;
    QMenu *contextMenuPropertiesOBS;
    QMenu *contextMenuPropertiesFile;
    QMenu *contextMenuPropertiesMSCFile;
    QMenu *contextMenuSubtypes;
    QMenu *contextMenuSubtypesFile;

    struct ModelCheckingWindowPrivate;
    ModelCheckingWindowPrivate *d;

    QTreeWidgetItem *propertiesTopDirWidgetItem;
    QTreeWidgetItem *subtypesTopDirWidgetItem;
    QTreeWidgetItem *functionsTopNodeWidgetItem;
    QTreeWidgetItem *resultsTopDirWidgetItem;
    QTreeWidgetItem *spinResultsTopDirWidgetItem;

    const QString booleanStopConditionLTL = "Boolean Stop Condition - LTL";
    const QString booleanStopConditionObserver = "Boolean Stop Condition - Observer";
    const QString messageSequenceChartSearchVerify = "Message Sequence Chart Search/Verify";
    const QString messageSequenceChartWhenThen = "Message Sequence Chart When/Then";
    const QString observer = "Observer";

    const QStringList availablePropertyTypes { booleanStopConditionLTL, booleanStopConditionObserver,
        messageSequenceChartSearchVerify, messageSequenceChartWhenThen, observer };

    const QStringList supportedPropertyTypes { booleanStopConditionLTL, booleanStopConditionObserver,
        messageSequenceChartSearchVerify, messageSequenceChartWhenThen, observer };

    const QString defaultMessageSequenceChartWhenThenMscTemplateName = "when-then-property-tmpl.msc";
    const QString defaultMessageSequenceChartWhenThenMscTemplatePath =
            shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/model-checker/"
            + defaultMessageSequenceChartWhenThenMscTemplateName;
    const QString sedCommand = "sed";
    const QString sedParameterForInplaceEdit = "-i";
    const QString sedParameterForWhenThenPropertyTemplate = "s/Untitled_Document/%1/";

    QString askAboutNewPropertyType();
    bool isPropertyTypeSupported(const QString &propertyType);
    QString askAboutNewPropertyName(const QString &propertyType);
    void escapeNewPropertyName(QString &propertyName) const;
    void checkNewPropertyNameAndAppendSuffixIfNeeded(QString &propertyName);
    QString getMakeRuleForPropertyType(const QString &propertyType) const;
    bool invokeMake(const QString &makeRule, const QString &propertyName);
    bool handleNoneMakePropertyTypes(const QString &propertyType, const QString &propertyName);
    bool handleBooleanStopConditionLTL(const QString newPropertyDirectoryPath, const QString &propertyName);
    bool handleMessageSequenceChartWhenThen(const QString newPropertyDirectoryPath, const QString &propertyName);
    QString getNewPropertyDirectoryPath(const QString &propertyName) const;
    bool createSubTypesDirectoryAndDirectoryForNewProperty(const QString &newPropertyDirectoryPath);
    void refreshPropertiesTreeViewWithPreselection();

    void setCheckBoxState(QCheckBox *checkBox, bool isChecked);
    SpinConfigData readSpinConfigFromUI();

    QProcess *observedProcess;
};

}
