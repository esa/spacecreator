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

#include "instancecreatortool.h"

#include <commands/common/commandsstack.h>
#include <instanceitem.h>
#include <mscchart.h>
#include <mscinstance.h>

#include <QDebug>

namespace msc {

InstanceCreatorTool::InstanceCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Instance");
    m_description = tr("Create new Instance item");
    m_icon = QPixmap(":/icons/toolbar/instance.png");
}

ToolType InstanceCreatorTool::toolType() const
{
    return msc::ToolType::InstanceCreator;
}

void InstanceCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active)
        return;

    MscInstance *orphaninstance = new MscInstance(tr("Instance"));
    InstanceItem *instanceItem = m_model->createDefaultInstanceItem(orphaninstance, scenePos());

    if (!instanceItem) {
        delete orphaninstance;
        return;
    }

    m_previewItem = instanceItem;
    m_previewEntity = instanceItem->modelItem();

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void InstanceCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart)
        return;

    auto instance = qobject_cast<msc::MscInstance *>(m_previewEntity);
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscInstance *>(instance),
                                      QVariant::fromValue<msc::MscChart *>(m_activeChart) };

    removePreviewItem(); // free the space to avoid overlapping

    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateInstance, cmdParams);

    Q_EMIT created();
}

void InstanceCreatorTool::removePreviewItem()
{
    if (!m_previewItem)
        return;

    m_model->removeInstanceItem(dynamic_cast<InstanceItem *>(m_previewItem.data()));
}

void InstanceCreatorTool::onCurrentChartChagend(msc::MscChart *chart)
{
    if (m_previewEntity && m_activeChart)
        if (MscInstance *instance = dynamic_cast<MscInstance *>(m_previewEntity.data())) {
            m_activeChart->removeInstance(instance);
            delete m_previewEntity;
        }

    BaseCreatorTool::onCurrentChartChagend(chart);
}

} // ns msc
