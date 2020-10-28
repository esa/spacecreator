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

#include "timercreatortool.h"

#include "baseitems/common/mscutils.h"
#include "chartlayoutmanager.h"
#include "commands/cmdtimeritemcreate.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscinstance.h"
#include "msctimer.h"
#include "timeritem.h"

namespace msc {

TimerCreatorTool::TimerCreatorTool(
        MscTimer::TimerType type, ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
    , m_timerType(type)
{
    Q_ASSERT(m_timerType != MscTimer::TimerType::Unknown);

    m_title = tr("Timer");

    switch (m_timerType) {
    case MscTimer::TimerType::Start:
        m_icon = QPixmap(":/icons/toolbar/starttimer.svg");
        m_description = tr("Create new start timer item");
        break;
    case MscTimer::TimerType::Stop:
        m_icon = QPixmap(":/icons/toolbar/stoptimer.svg");
        m_description = tr("Create new stop/reset timer item");
        break;
    case MscTimer::TimerType::Timeout:
        m_icon = QPixmap(":/icons/toolbar/timeouttimer.svg");
        m_description = tr("Create new timeout item");
        break;
    default:
        break;
    }
}

BaseTool::ToolType TimerCreatorTool::toolType() const
{
    return BaseTool::ToolType::TimerCreator;
}

void TimerCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active) {
        return;
    }

    auto orphantimer = new MscTimer(this);
    orphantimer->setName(tr("New_timer"));
    orphantimer->setTimerType(m_timerType);
    auto timerItem = new TimerItem(orphantimer, m_model);

    m_previewItem = timerItem;
    m_previewEntity.reset(timerItem->modelItem());

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void TimerCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart) {
        return;
    }

    auto timer = qobject_cast<msc::MscTimer *>(m_previewEntity.take());
    auto instance = m_model->nearestInstance(m_previewItem->sceneBoundingRect().center());
    const int eventIndex = m_model->eventIndex(m_previewItem->y());
    m_model->undoStack()->push(new cmd::CmdTimerItemCreate(timer, m_timerType, instance, eventIndex, m_model));

    startWaitForModelLayoutComplete(timer);
    removePreviewItem();

    Q_EMIT created();
}

} // namespace msc
