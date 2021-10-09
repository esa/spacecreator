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
#include "itemeditor/graphicsview.h"
#include "ui_dvappwidget.h"

#include <QDebug>
#include <QSplitter>

namespace dve {

DVAppWidget::DVAppWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DVAppWidget)
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
    return ui->dvTreeWidget->selectionModel();
}

void DVAppWidget::setGraphicsScene(QGraphicsScene *scene)
{
    ui->view->setScene(scene);
}

void DVAppWidget::setDVCore(DVEditorCore *core)
{
    m_dvCore = core;

    ui->dvTreeWidget->setDVCore(core);

    ui->hwLibraryView->setModel(m_dvCore->hwItemModel());
}

void DVAppWidget::setActions(const QVector<QAction *> &actions)
{
    for (QAction *action : actions) {
        ui->toolBar->addAction(action);
    }
}

} // namespace dve
