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

#include "dvappwidget.h"

#include "dveditorcore.h"
#include "dvtreesortproxymodel.h"
#include "implementationdelegate.h"
#include "itemeditor/graphicsview.h"
#include "ui_dvappwidget.h"

#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QSplitter>
#include <QTreeView>

namespace dve {

DVAppWidget::DVAppWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DVAppWidget)
    , m_selectionModel(new QItemSelectionModel(nullptr, this))
    , m_dvTreeSortModel(new DVTreeSortProxyModel(this))
{
    ui->setupUi(this);
    ui->mainSplitter->setStretchFactor(1, 1);
    ui->leftSplitter->setStretchFactor(0, 1);
}

DVAppWidget::~DVAppWidget() { }

GraphicsView *DVAppWidget::graphicsView() const
{
    return ui->view;
}

QItemSelectionModel *DVAppWidget::selectionModel() const
{
    return m_selectionModel;
}

void DVAppWidget::setGraphicsScene(QGraphicsScene *scene)
{
    ui->view->setScene(scene);
}

void DVAppWidget::setDVCore(DVEditorCore *core)
{
    m_dvCore = core;

    m_dvTreeSortModel->setSourceModel(m_dvCore->itemTreeModel());
    m_selectionModel->setModel(m_dvTreeSortModel);
    ui->treeView->setModel(m_dvTreeSortModel);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setSelectionModel(m_selectionModel);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    auto delegate = new ImplementationDelegate(m_dvCore->commandsStack(), ui->treeView);
    ui->treeView->setItemDelegateForColumn(1, delegate);

    ui->hwLibraryView->setModel(m_dvCore->hwItemModel());
}

void DVAppWidget::setActions(const QVector<QAction *> &actions)
{
    for (QAction *action : actions) {
        ui->toolBar->addAction(action);
    }
}

} // namespace dve
