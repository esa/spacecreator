/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "entitydeletetool.h"

#include "baseitems/interactiveobject.h"
#include "commands/common/commandsstack.h"
#include "documentitem.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscentity.h"

#include <QAction>
#include <QVariant>
#include <QVector>

namespace msc {

EntityDeleteTool::EntityDeleteTool(QGraphicsView *view, QObject *parent)
    : BaseTool(view, parent)
{
    m_title = tr("Delete");
    m_description = tr("Delete an item");
    m_icon = QPixmap(":/icons/toolbar/delete.svg");

    m_action = new QAction(m_icon, m_title, this);
    m_action->setToolTip(m_description);
    m_action->setShortcut(QKeySequence::Delete);
    connect(m_action, &QAction::triggered, this, &EntityDeleteTool::deleteSelectedItems);

    if (m_view) {
        connect(m_view->scene(), &QGraphicsScene::selectionChanged, this, &EntityDeleteTool::updateEnabledState);
    }
    updateEnabledState();
}

BaseTool::ToolType EntityDeleteTool::toolType() const
{
    return BaseTool::ToolType::EntityDeleter;
}

void EntityDeleteTool::setView(QGraphicsView *view)
{
    if (m_view) {
        disconnect(m_view->scene(), nullptr, this, nullptr);
    }

    BaseTool::setView(view);

    if (m_view) {
        connect(m_view->scene(), &QGraphicsScene::selectionChanged, this, &EntityDeleteTool::updateEnabledState);
    }
    updateEnabledState();
}

QAction *EntityDeleteTool::action() const
{
    return m_action;
}

void EntityDeleteTool::setCurrentChart(MscChart *chart)
{
    m_currentChart = chart;
}

bool EntityDeleteTool::onMousePress(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool EntityDeleteTool::onMouseRelease(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool EntityDeleteTool::onMouseMove(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void EntityDeleteTool::deleteSelectedItems()
{
    if (!m_view || !m_view->scene()) {
        return;
    }

    QVector<msc::MscEntity *> items;
    msc::MscDocument *parentDocument = nullptr;

    for (auto item : m_view->scene()->selectedItems()) {
        auto obj = dynamic_cast<msc::InteractiveObject *>(item);
        if (obj && obj->modelEntity()) {
            items.append(obj->modelEntity());
        }

        auto documentItem = dynamic_cast<msc::DocumentItem *>(item);
        if (documentItem) {
            parentDocument = documentItem->document()->parentDocument();

            if (parentDocument) {
                items.append(documentItem->document());
            } else {
                return;
            }
        }
    }

    msc::cmd::CommandsStack::push(msc::cmd::DeleteEntity,
                                  { QVariant::fromValue<QVector<msc::MscEntity *>>(items),
                                    QVariant::fromValue<msc::MscChart *>(m_currentChart),
                                    QVariant::fromValue<msc::MscDocument *>(parentDocument) });
}

void EntityDeleteTool::updateEnabledState()
{
    if (!m_view || !m_view->scene() || m_view->scene()->selectedItems().isEmpty()) {
        m_action->setEnabled(false);
        return;
    }

    auto documentItem = dynamic_cast<msc::DocumentItem *>(m_view->scene()->selectedItems()[0]);
    m_action->setEnabled(documentItem ? documentItem->document()->parentDocument() != nullptr : true);
}

} // namespace msc
