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

#include "baseitems/common/mscutils.h"
#include "commands/common/commandsstack.h"
#include "conditionitem.h"
#include "mscchart.h"
#include "msccondition.h"
#include "mscinstance.h"

namespace msc {

ConditionCreatorTool::ConditionCreatorTool(bool shared, ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
    , m_shared(shared)
{
    if (shared) {
        m_title = tr("Shared Condition");
        m_description = tr("Create new shared Condition item");
        m_icon = QPixmap(":/icons/toolbar/condition_shared.svg");
    } else {
        m_title = tr("Condition");
        m_description = tr("Create new Condition item");
        m_icon = QPixmap(":/icons/toolbar/condition.svg");
    }
}
BaseTool::ToolType ConditionCreatorTool::toolType() const
{
    return BaseTool::ToolType::ConditionCreator;
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
    m_previewEntity.reset(item->modelItem());

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void ConditionCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart) {
        return;
    }

    auto condition = qobject_cast<msc::MscCondition *>(m_previewEntity.take());
    condition->setShared(m_shared);
    auto instance = m_model->nearestInstance(m_previewItem->sceneBoundingRect().center());
    const int eventIndex = m_model->eventIndex(m_previewItem->y());
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscCondition *>(condition),
        QVariant::fromValue<msc::MscInstance *>(instance), eventIndex };
    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateCondition, cmdParams);

    startWaitForModelLayoutComplete(condition);
    removePreviewItem();

    Q_EMIT created();
}

} // namespace msc
