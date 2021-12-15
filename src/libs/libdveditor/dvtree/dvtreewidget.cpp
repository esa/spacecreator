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

#include "dvtreewidget.h"

#include "dveditorcore.h"
#include "dvfunction.h"
#include "dvtreesortproxymodel.h"
#include "dvtreeviewmodel.h"
#include "implementationdelegate.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QTreeView>
#include <QVBoxLayout>

namespace dve {

DVTreeWidget::DVTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_treeView(new QTreeView(this))
    , m_dvTreeSortModel(new DVTreeSortProxyModel(this))
    , m_selectionModel(new SelectionOverProxyModel(m_dvTreeSortModel, this))
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);
    setLayout(layout);
    layout->addWidget(m_treeView);

    connect(m_dvTreeSortModel, &DVTreeSortProxyModel::rowsInserted, this, &DVTreeWidget::dvObjectInserted);
    connect(m_dvTreeSortModel, &DVTreeSortProxyModel::modelReset, this, &DVTreeWidget::enableAllImplementationEdits);
    connect(m_dvTreeSortModel, &DVTreeSortProxyModel::dataChanged, this, &DVTreeWidget::updateImplementation);
}

void DVTreeWidget::setDVCore(DVEditorCore *core)
{
    m_dvCore = core;

    auto delegate = new ImplementationDelegate(m_dvCore->commandsStack(), m_treeView);
    m_treeView->setItemDelegateForColumn(1, delegate);

    m_dvTreeSortModel->setSourceModel(m_dvCore->itemTreeModel());
    m_selectionModel->setModel(m_dvTreeSortModel);
    m_treeView->setModel(m_dvTreeSortModel);
    m_treeView->setSortingEnabled(true);
    m_treeView->setSelectionModel(m_selectionModel);
    m_treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    enableAllImplementationEdits();
}

QItemSelectionModel *DVTreeWidget::selectionModel() const
{
    return m_selectionModel;
}

void DVTreeWidget::dvObjectInserted(const QModelIndex &parent, int first, int last)
{
    if (!m_dvTreeSortModel) {
        return;
    }

    for (int idx = first; idx <= last; ++idx) {
        const QModelIndex index = m_dvTreeSortModel->index(idx, DVTreeViewModel::Columns::Implementation, parent);
        dvm::DVFunction *fn = index.data(dve::DVTreeViewModel::DVObjectRole).value<dvm::DVFunction *>();
        if (fn) {
            m_treeView->openPersistentEditor(index);
        }
        QModelIndex index1 = m_dvTreeSortModel->index(idx, 0, parent);
        if (index1.isValid() && m_dvTreeSortModel->rowCount(index1) > 0) {
            dvObjectInserted(index1, 0, m_dvTreeSortModel->rowCount(index1));
        }
    }
}

void DVTreeWidget::enableAllImplementationEdits()
{
    if (!m_dvTreeSortModel) {
        return;
    }
    dvObjectInserted(QModelIndex(), 0, m_dvTreeSortModel->rowCount() - 1);
}

void DVTreeWidget::updateImplementation(
        const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles);
    if (!m_dvTreeSortModel) {
        return;
    }
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        QModelIndex index = m_dvTreeSortModel->index(row, topLeft.column(), topLeft.parent());
        dvm::DVFunction *fn = index.data(dve::DVTreeViewModel::DVObjectRole).value<dvm::DVFunction *>();
        if (fn) {
            m_treeView->closePersistentEditor(index);
            m_treeView->openPersistentEditor(index);
        }
    }
}

} // namespace dve
