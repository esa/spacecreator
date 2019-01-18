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

#include "messagecreatortool.h"

#include <baseitems/arrowitem.h>
#include <commands/common/commandsstack.h>
#include <mscchart.h>
#include <mscmessage.h>

#include <QDebug>

namespace msc {

MessageCreatorTool::MessageCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Message");
    m_description = tr("Create new Message item");
    m_icon = QPixmap(":/icn/toolbar/icons/toolbar/message.png");
}

ToolType MessageCreatorTool::toolType() const
{
    return msc::ToolType::MessageCreator;
}

void MessageCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active)
        return;

    MscMessage *orphanMessage = new MscMessage(tr("Message"));
    MessageItem *messageItem = m_model->createDefaultMessageItem(orphanMessage, scenePos());

    if (!messageItem) {
        delete orphanMessage;
        return;
    }

    m_previewItem = messageItem;
    m_previewEntity = messageItem->modelItem();

    messageItem->setAutoResizable(false);
    messageItem->performSnap();

    m_scene->addItem(m_previewItem);
    m_previewItem->setOpacity(0.5);
}

void MessageCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity || !m_activeChart)
        return;

    auto message = qobject_cast<msc::MscMessage *>(m_previewEntity);
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscMessage *>(message),
                                      QVariant::fromValue<msc::MscChart *>(m_activeChart) };

    removePreviewItem();
    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateMessage, cmdParams);

    Q_EMIT created();
}

void MessageCreatorTool::removePreviewItem()
{
    if (!m_previewItem)
        return;

    m_model->removeMessageItem(dynamic_cast<MessageItem *>(m_previewItem.data()));
}

void MessageCreatorTool::onCurrentChartChagend(msc::MscChart *chart)
{
    if (m_previewEntity && m_activeChart)
        if (MscMessage *message = dynamic_cast<MscMessage *>(m_previewEntity.data())) {
            m_activeChart->removeInstanceEvent(message);
            delete m_previewEntity;
        }

    BaseCreatorTool::onCurrentChartChagend(chart);
}

} // ns msc
