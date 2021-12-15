/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QWidget>

class QItemSelectionModel;
class QModelIndex;
class QTreeView;

namespace dve {
class DVTreeSortProxyModel;
class DVEditorCore;

/*!
   Class to represent theDV data in a tree view
 */
class DVTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DVTreeWidget(QWidget *parent = nullptr);

    void setDVCore(DVEditorCore *core);
    QItemSelectionModel *selectionModel() const;

private Q_SLOTS:
    void dvObjectInserted(const QModelIndex &parent, int first, int last);
    void enableAllImplementationEdits();
    void updateImplementation(
            const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

private:
    QTreeView *m_treeView = nullptr;
    DVTreeSortProxyModel *m_dvTreeSortModel = nullptr;
    QItemSelectionModel *m_selectionModel = nullptr;
    DVEditorCore *m_dvCore = nullptr;
};

} // namespace dve
