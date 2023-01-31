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
#include "chartlayoutmanager.h"
#include "commands/cmddeleteentity.h"
#include "commands/cmdentitycommentchange.h"
#include "commentitem.h"
#include "coregionitem.h"
#include "documentitem.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "msccomment.h"
#include "msccoregion.h"
#include "mscdocument.h"
#include "mscentity.h"

#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVariant>
#include <QVector>

namespace msc {

EntityDeleteTool::EntityDeleteTool(ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Delete");
    m_description = tr("Delete an item");
    m_icon = QIcon(":/icons/toolbar/delete.svg");

    setAction(new QAction(m_icon, m_title, this));
    m_action->setToolTip(m_description);
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
    QVector<msc::MscEntity *> comments;
    msc::MscDocument *parentDocument = nullptr;

    for (auto item : m_view->scene()->selectedItems()) {
        auto obj = dynamic_cast<msc::InteractiveObject *>(item);
        if (obj) {
            if (auto coregionItem = qobject_cast<CoregionItem *>(obj)) {
                items << coregionItem->begin() << coregionItem->end();
            } else {
                if (obj->modelEntity()) {
                    if (obj->modelEntity()->entityType() == MscEntity::EntityType::Comment) {
                        if (MscComment *comment = qobject_cast<MscComment *>(obj->modelEntity()))
                            comments.append(comment->attachedEntity());
                    } else {
                        items.append(obj->modelEntity());
                    }
                }
            }
        }

        auto documentItem = dynamic_cast<msc::DocumentItem *>(item);
        if (documentItem) {
            parentDocument = documentItem->document()->parentDocument();

            if (parentDocument)
                items.append(documentItem->document());
            else
                return;
        }
    }

    MscCommandsStack *undoStack = m_model->undoStack();
    undoStack->beginMacro(tr("Removing Entities"));
    for (msc::MscEntity *entity : comments) {
        m_model->undoStack()->push(new cmd::CmdEntityCommentChange(entity, QString(), m_model->currentChart()));
    }
    if (parentDocument) {
        m_model->undoStack()->push(new cmd::CmdDeleteEntity(items, parentDocument));
    } else {
        m_model->undoStack()->push(new cmd::CmdDeleteEntity(items, m_model->currentChart()));
    }
    undoStack->endMacro();
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
