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

#include "baseitems/arrowitem.h"
#include "baseitems/common/objectanchor.h"
#include "commands/common/commandsstack.h"
#include "mscchart.h"
#include "mscmessage.h"

#include <QDebug>
#include <QMouseEvent>

namespace msc {

MessageCreatorTool::MessageCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Message (Drag)");
    m_description = tr("Create new Message item");
    m_icon = QPixmap(":/icons/toolbar/message.png");
}

BaseTool::ToolType MessageCreatorTool::toolType() const
{
    return BaseTool::ToolType::MessageCreator;
}

void MessageCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active)
        return;

    MscMessage *orphanMessage = new MscMessage(tr("Drag to the target\n"));
    m_messageItem = m_model->createDefaultMessageItem(orphanMessage, scenePos());

    if (!m_messageItem) {
        delete orphanMessage;
        return;
    }

    m_previewItem = m_messageItem;
    m_previewEntity = m_messageItem->modelItem();

    m_messageItem->setAutoResizable(false);

    m_scene->addItem(m_previewItem);

    m_view->setCursor(Qt::CrossCursor);
}

void MessageCreatorTool::commitPreviewItem()
{
    // TODO: Currently the BaseCreatorTool::created signal is emitted only if
    // a message has been added successfuly - it allows to keep the toolbar's
    // item (this tool) active during a set of "create message" actions performed
    // in a row on an empty (without instances) scene.
    // While the result version is supposed to disalow adding messages to an empty
    // scene, the way how an orphan message is processed should be discussed.

    if (m_previewEntity && m_activeChart) {
        const QVariantList &cmdParams = prepareMessage();
        if (!cmdParams.isEmpty()) {
            startWaitForModelLayoutComplete(qobject_cast<msc::MscMessage *>(m_previewEntity));
            msc::cmd::CommandsStack::push(msc::cmd::Id::CreateMessage, cmdParams);

            Q_EMIT created(); // to deactivate toobar's item
            removePreviewItem();
            return;
        }
    }

    removePreviewItem();
    delete m_previewEntity.data();
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

bool MessageCreatorTool::onMousePress(QMouseEvent *e)
{
    if (m_messageItem && m_currStep == Step::ChooseSource) {
        m_messageItem->setTail(scenePos(e->globalPos()), ObjectAnchor::Snap::NoSnap);
        m_currStep = Step::ChooseTarget;
    }
    return true;
}

bool MessageCreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (m_currStep == Step::ChooseTarget) {
        commitPreviewItem();
        m_currStep = Step::ChooseSource;
        createPreviewItem();

        if (m_messageItem) {
            const QPointF &scenePos = this->scenePos(e->globalPos());
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setTail(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setPos(scenePos);
        }
    }

    return true;
}

bool MessageCreatorTool::onMouseMove(QMouseEvent *e)
{
    if (m_messageItem) {
        const QPointF &scenePos = this->scenePos(e->globalPos());
        switch (m_currStep) {
        case Step::ChooseSource: {
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setTail(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setPos(scenePos);
            break;
        }
        case Step::ChooseTarget: {
            QPointF head(scenePos);
            if (e->modifiers() == Qt::ControlModifier) {
                head.ry() = m_messageItem->tail().y();
            }
            m_messageItem->setHead(head, ObjectAnchor::Snap::NoSnap);
            break;
        }
        }
    }

    return true;
}

QVariantList MessageCreatorTool::prepareMessage()
{
    QVariantList args;

    auto message = qobject_cast<msc::MscMessage *>(m_previewEntity);

    if (message->isOrphan())
        if (m_activeChart && !m_activeChart->instances().isEmpty())
            message->setSourceInstance(m_activeChart->instances().first());

    if (!message->isOrphan()) {
        message->setName(tr("Message"));

        if (message->sourceInstance() == message->targetInstance())
            message->setTargetInstance(nullptr);
        const int eventIndex = m_model->eventIndex(m_previewItem->y());
        args = { QVariant::fromValue<msc::MscMessage *>(message), QVariant::fromValue<msc::MscChart *>(m_activeChart),
                 eventIndex };
    }

    return args;
}

MessageCreatorTool2::MessageCreatorTool2(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : MessageCreatorTool(model, view, parent)
{
    m_title = tr("Message (Click)");
}

void MessageCreatorTool2::createPreviewItem()
{
    MessageCreatorTool::createPreviewItem();
    if (m_messageItem)
        m_messageItem->setName(tr("Click to choose points"));
}

bool MessageCreatorTool2::onMousePress(QMouseEvent *e)
{
    Q_UNUSED(e);
    return true;
}

bool MessageCreatorTool2::onMouseRelease(QMouseEvent *e)
{
    switch (m_currStep) {
    case Step::ChooseSource: {
        m_messageItem->setTail(scenePos(e->globalPos()), ObjectAnchor::Snap::NoSnap);
        m_currStep = Step::ChooseTarget;
        break;
    }
    case Step::ChooseTarget: {
        commitPreviewItem();
        m_currStep = Step::ChooseSource;
        createPreviewItem();

        if (m_messageItem) {
            const QPointF &scenePos = this->scenePos(e->globalPos());
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setTail(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setPos(scenePos);
        }
        break;
    }
    }

    return true;
}

bool MessageCreatorTool2::onMouseMove(QMouseEvent *e)
{
    if (m_messageItem) {
        const QPointF &scenePos = this->scenePos(e->globalPos());
        switch (m_currStep) {
        case Step::ChooseSource: {
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setTail(scenePos, ObjectAnchor::Snap::NoSnap);
            m_messageItem->setPos(scenePos);
            break;
        }
        case Step::ChooseTarget: {
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            break;
        }
        }
    }

    return true;
}

} // ns msc
