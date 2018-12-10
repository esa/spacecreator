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

#include <QDebug>

namespace msc {

InstanceCreatorTool::InstanceCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Instance");
    m_description = tr("Create new Instance item");
    m_icon = QPixmap(":/icn/toolbar/icons/toolbar/instance.png");
}

ToolType InstanceCreatorTool::toolType() const
{
    return msc::ToolType::InstanceCreator;
}

void InstanceCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem)
        return;

    m_previewItem = m_model->createDefaultInstanceItem(nullptr, scenePos());
    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void InstanceCreatorTool::commitPreviewItem()
{
    if (!m_previewItem || !m_scene)
        return;

    const QVariantList &cmdParams = { QVariant::fromValue<QGraphicsScene *>(m_scene),
                                      QVariant::fromValue<ChartViewModel *>(m_model),
                                      m_previewItem->pos() };

    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateInstance, cmdParams);

    InstanceItem *instanceItem = static_cast<InstanceItem *>(m_previewItem);
    instanceItem->setName(tr("Instance_%1").arg(m_model->currentChart()->instances().size()));
}

void InstanceCreatorTool::removePreviewItem()
{
    if (!m_previewItem)
        return;

    if (m_model->removeInstanceItem(static_cast<InstanceItem *>(m_previewItem)))
        m_previewItem = nullptr;
}

} // ns msc
