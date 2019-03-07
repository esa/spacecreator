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

#include "conditioncreatortool.h"

#include "baseitems/common/utils.h"
#include "commands/common/commandsstack.h"
#include "conditionitem.h"
#include "mscchart.h"
#include "msccondition.h"
#include "mscinstance.h"

namespace msc {

ConditionCreatorTool::ConditionCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Condition");
    m_description = tr("Create new Condition item");
    m_icon = QPixmap(":/icons/toolbar/condition.svg");
}
BaseTool::ToolType ConditionCreatorTool::toolType() const
{
    return BaseTool::ToolType::ConditionCreator;
}

void ConditionCreatorTool::onCurrentChartChagend(MscChart *chart)
{
    if (m_previewEntity && m_activeChart)
        if (MscMessage *message = dynamic_cast<MscMessage *>(m_previewEntity.data())) {
            m_activeChart->removeInstanceEvent(message);
            delete m_previewEntity;
        }

    BaseCreatorTool::onCurrentChartChagend(chart);
}

void ConditionCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active) {
        return;
    }

    auto orphanCondition = new MscCondition(this);
    orphanCondition->setName(tr("Condition_%1").arg(m_model->currentChart()->instanceEvents().size()));
    auto item = new ConditionItem(orphanCondition);

    m_previewItem = item;
    m_previewEntity = item->modelItem();

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void ConditionCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart) {
        return;
    }

    auto condition = qobject_cast<msc::MscCondition *>(m_previewEntity);
    auto instance = m_model->nearestInstance(m_previewItem->sceneBoundingRect().center());
    const int eventIndex = m_model->eventIndex(m_previewItem->y());
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscCondition *>(condition),
                                      QVariant::fromValue<msc::MscChart *>(m_activeChart),
                                      QVariant::fromValue<msc::MscInstance *>(instance), eventIndex };

    utils::removeSceneItem(m_previewItem);
    delete m_previewItem.data();

    startWaitForModelLayoutComplete(condition);
    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateCondition, cmdParams);

    Q_EMIT created();
}

void ConditionCreatorTool::removePreviewItem()
{
    if (!m_previewItem) {
        return;
    }

    utils::removeSceneItem(m_previewItem);
    delete m_previewItem.data();
    delete m_previewEntity.data();
}

} // namespace msc
