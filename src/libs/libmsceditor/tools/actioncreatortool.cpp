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

#include "actioncreatortool.h"

#include "actionitem.h"
#include "baseitems/common/mscutils.h"
#include "commands/cmdactionitemcreate.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscinstance.h"

namespace msc {

ActionCreatorTool::ActionCreatorTool(ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Action");
    m_description = tr("Create new Action item");
    m_icon = QIcon(":/icons/toolbar/action.svg");
}

BaseTool::ToolType ActionCreatorTool::toolType() const
{
    return BaseTool::ToolType::ActionCreator;
}

void ActionCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active) {
        return;
    }

    auto orphanAction = new MscAction(this);
    orphanAction->setInformalAction(tr("Action_%1").arg(m_model->currentChart()->instanceEvents().size()));
    auto actionItem = new ActionItem(orphanAction, m_model);

    m_previewItem = actionItem;
    m_previewEntity.reset(actionItem->modelItem());

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void ActionCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart) {
        return;
    }

    auto action = qobject_cast<msc::MscAction *>(m_previewEntity.take());
    startWaitForModelLayoutComplete(action);
    auto instance = m_model->nearestInstance(m_previewItem->sceneBoundingRect().center());
    const int eventIndex = m_model->eventIndex(m_previewItem->pos());
    m_model->undoStack()->push(new cmd::CmdActionItemCreate(action, instance, eventIndex, m_model));

    removePreviewItem();

    Q_EMIT created();
}
} // namespace msc
