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

#include <QMainWindow>
#include <QFileInfo>
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

Q_SIGNALS:
    void visibleChanged(bool visible);

private Q_SLOTS:
    Qt::CheckState listProperties(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent, QStringList preSelection);
    QStringList getPropertiesSelection(QTreeWidgetItem *propertyWidgetItem, QStringList selections);
    Qt::CheckState getCheckState(QStringList selection, QString path);
    void listSubtypes(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent);
    QStringList getSubtypesSelection();
    void listModelFunctions(QTreeWidgetItem *parentWidgetItem);
    QStringList getFunctionsSelection();
    void listResults(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent);
    void on_treeWidget_properties_itemChanged(QTreeWidgetItem *item, int column);
    void updateParentItem(QTreeWidgetItem *item);
    void on_treeWidget_properties_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_subtyping_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_subtyping_itemChanged(QTreeWidgetItem *item, int column);
    void on_pushButton_callIF_clicked();
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

private:
    QString propertiesPath;
    QString subtypesPath;
    QString configurationsPath;
    QString outputPath;
    QString projectDir;

    QMenu* contextMenuPropertiesTop;
    QMenu* contextMenuProperties;
    QMenu* contextMenuPropertiesBSC;
    QMenu* contextMenuPropertiesMSC;
    QMenu* contextMenuPropertiesOBS;
    QMenu* contextMenuPropertiesFile;
    QMenu* contextMenuPropertiesMSCFile;
    QMenu* contextMenuSubtypes;
    QMenu* contextMenuSubtypesFile;

    struct ModelCheckingWindowPrivate;
    ModelCheckingWindowPrivate *d;

    QTreeWidgetItem *propertiesTopDirWidgetItem;
    QTreeWidgetItem *subtypesTopDirWidgetItem;
    QTreeWidgetItem *functionsTopNodeWidgetItem;
};

}
