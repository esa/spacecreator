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

#include "coregioncreatortool.h"

#include "baseitems/common/mscutils.h"
#include "chartlayoutmanager.h"
#include "commands/cmdcoregionitemcreate.h"
#include "coregionitem.h"
#include "msccommandsstack.h"
#include "msccoregion.h"
#include "mscinstance.h"

namespace msc {

CoregionCreatorTool::CoregionCreatorTool(ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Coregion");
    m_icon = QIcon(":/icons/toolbar/coregion.svg");
    m_description = tr("Create new Coregion");
}

BaseTool::ToolType CoregionCreatorTool::toolType() const
{
    return BaseTool::ToolType::CoregionCreator;
}

void CoregionCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active) {
        return;
    }

    auto coregionItem = new CoregionItem(m_model);
    auto orphanCoregionBegin = new MscCoregion(this);
    orphanCoregionBegin->setType(MscCoregion::Type::Begin);
    coregionItem->setBegin(orphanCoregionBegin);

    auto orphanCoregionEnd = new MscCoregion(this);
    orphanCoregionEnd->setType(MscCoregion::Type::End);
    coregionItem->setEnd(orphanCoregionEnd);
    coregionItem->scheduleLayoutUpdate();

    m_previewItem = coregionItem;

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void CoregionCreatorTool::commitPreviewItem()
{
    if (!m_previewItem || !m_activeChart)
        return;

    auto coregion = qobject_cast<msc::CoregionItem *>(m_previewItem);
    auto instance = m_model->nearestInstance(m_previewItem->sceneBoundingRect().center());
    const int eventIndex = m_model->eventInstanceIndex(m_previewItem->pos(), instance);
    m_model->undoStack()->push(
            new cmd::CmdCoregionItemCreate(coregion->begin(), coregion->end(), instance, eventIndex, m_model));

    removePreviewItem();

    Q_EMIT created();
}

} // namespace msc
