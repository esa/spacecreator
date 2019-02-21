/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "baseitems/common/utils.h"
#include "commands/common/commandsstack.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "msctimer.h"
#include "timeritem.h"

namespace msc {

TimerCreatorTool::TimerCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Timer");
    m_description = tr("Create new Timer item");
    m_icon = QPixmap(":/icons/toolbar/starttimer.svg");
}

BaseTool::ToolType TimerCreatorTool::toolType() const
{
    return BaseTool::ToolType::TimerCreator;
}

void TimerCreatorTool::setTimerType(MscTimer::TimerType type)
{
    Q_ASSERT(type != MscTimer::TimerType::Unknown);

    m_timerType = type;
    switch (m_timerType) {
    case MscTimer::TimerType::Start:
        m_icon = QPixmap(":/icons/toolbar/starttimer.svg");
        break;
    case MscTimer::TimerType::Stop:
        m_icon = QPixmap(":/icons/toolbar/stoptimer.svg");
        break;
    case MscTimer::TimerType::Timeout:
        m_icon = QPixmap(":/icons/toolbar/timeouttimer.svg");
        break;
    default:
        break;
    }
}

void TimerCreatorTool::onCurrentChartChagend(MscChart *chart)
{
    if (m_previewEntity && m_activeChart)
        if (MscMessage *message = dynamic_cast<MscMessage *>(m_previewEntity.data())) {
            m_activeChart->removeInstanceEvent(message);
            delete m_previewEntity;
        }

    BaseCreatorTool::onCurrentChartChagend(chart);
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
    m_previewEntity = timerItem->modelItem();

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void TimerCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart) {
        return;
    }

    auto timer = qobject_cast<msc::MscTimer *>(m_previewEntity);
    auto instance = m_model->nearestInstance(m_previewItem->sceneBoundingRect().center());
    const int eventIndex = m_model->eventIndex(m_previewItem->y());
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscTimer *>(timer),
                                      QVariant::fromValue<msc::MscTimer::TimerType>(m_timerType),
                                      QVariant::fromValue<msc::MscChart *>(m_activeChart),
                                      QVariant::fromValue<msc::MscInstance *>(instance), eventIndex };

    utils::removeSceneItem(m_previewItem);
    delete m_previewItem.data();

    startWaitForModelLayoutComplete(timer);
    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateTimer, cmdParams);

    Q_EMIT created();
}

void TimerCreatorTool::removePreviewItem()
{
    if (!m_previewItem) {
        return;
    }

    utils::removeSceneItem(m_previewItem);
    delete m_previewItem.data();
    delete m_previewEntity.data();
}

} // namespace msc
