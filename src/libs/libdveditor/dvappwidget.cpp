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

#include "baseitems/graphicsview.h"

#include <QBoxLayout>
#include <QSplitter>
#include <QTreeView>

namespace dve {

struct DVAppWidget::DVAppWidgetPrivate {
    DVAppWidgetPrivate(const DVAppWidgetPrivate &) = delete;
    DVAppWidgetPrivate &operator=(const DVAppWidgetPrivate &) = delete;
    DVAppWidgetPrivate() { }

    dve::GraphicsView *m_view = nullptr;
    QTreeView *m_treeView = nullptr;
};

DVAppWidget::DVAppWidget(QWidget *parent)
    : QWidget(parent)
    , d(new DVAppWidgetPrivate)
{
    setupUi();
}

DVAppWidget::~DVAppWidget() { }

GraphicsView *DVAppWidget::graphicsView() const
{
    return d->m_view;
}

void DVAppWidget::setGraphicsScene(QGraphicsScene *scene)
{
    d->m_view->setScene(scene);
}

void DVAppWidget::setTreeViewModel(QAbstractItemModel *model)
{
    d->m_treeView->setModel(model);
}

void DVAppWidget::setupUi()
{
    auto rootLayout = new QVBoxLayout(this);
    rootLayout->setMargin(0);
    setLayout(rootLayout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    rootLayout->addWidget(splitter);

    d->m_treeView = new QTreeView(this);
    d->m_treeView->setHeaderHidden(true);
    splitter->addWidget(d->m_treeView);
    splitter->setStretchFactor(0, 0);

    d->m_view = new dve::GraphicsView(this);
    splitter->addWidget(d->m_view);
    splitter->setStretchFactor(1, 1);
}

} // namespace dve
