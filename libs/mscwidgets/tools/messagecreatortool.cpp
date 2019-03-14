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

#include "messagecreatortool.h"

#include "baseitems/arrowitem.h"
#include "baseitems/common/objectanchor.h"
#include "baseitems/common/utils.h"
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
    m_cursor = QCursor(Qt::CrossCursor);
}

BaseTool::ToolType MessageCreatorTool::toolType() const
{
    return BaseTool::ToolType::MessageCreator;
}

void MessageCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active)
        return;

    MscMessage *orphanMessage = new MscMessage(tr("Message"));
    m_messageItem = m_model->createDefaultMessageItem(orphanMessage, cursorInScene());

    movePreviewItemTo(cursorInScene());

    if (!m_messageItem) {
        delete orphanMessage;
        return;
    }

    m_previewItem = m_messageItem;
    m_previewEntity = m_messageItem->modelItem();

    m_messageItem->setAutoResizable(false);

    m_scene->addItem(m_previewItem);

    m_currMode = InteractionMode::None;
}

void MessageCreatorTool::commitPreviewItem()
{
    m_currMode = InteractionMode::None;
    m_mouseDown = QPointF();

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
    switch (m_currMode) {
    case InteractionMode::None: {
        m_mouseDown = cursorInScene();
        break;
    }
    case InteractionMode::Drag: {
        processMousePressDrag(e);
        break;
    }
    case InteractionMode::Click: {
        processMousePressClick(e);
        break;
    }
    }

    return true;
}

bool MessageCreatorTool::onMouseRelease(QMouseEvent *e)
{
    switch (m_currMode) {
    case InteractionMode::None: {
        if (e->button() == Qt::LeftButton && m_mouseDown == cursorInScene(e->globalPos())) {
            m_currMode = InteractionMode::Click;
            onMouseRelease(e);
        }
        break;
    }
    case InteractionMode::Drag: {
        processMouseReleaseDrag(e);
        break;
    }
    case InteractionMode::Click: {
        processMouseReleaseClick(e);
        break;
    }
    }

    return true;
}

bool MessageCreatorTool::onMouseMove(QMouseEvent *e)
{
    switch (m_currMode) {
    case InteractionMode::None: {
        if (e->buttons() == Qt::LeftButton) {
            m_currMode = InteractionMode::Drag;
            processMousePressDrag(e);
        } else {
            const QPointF &scenePos = cursorInScene(e->globalPos());
            movePreviewItemTo(scenePos);
        }
        break;
    }
    case InteractionMode::Drag: {
        processMouseMoveDrag(e);
        break;
    }
    case InteractionMode::Click: {
        processMouseMoveClick(e);
        break;
    }
    }

    return true;
}

void MessageCreatorTool::processMousePressDrag(QMouseEvent *e)
{
    if (m_messageItem && m_currStep == Step::ChooseSource) {
        m_messageItem->setTail(cursorInScene(e->globalPos()), ObjectAnchor::Snap::NoSnap);
        m_currStep = Step::ChooseTarget;
    }
}

void MessageCreatorTool::processMouseReleaseDrag(QMouseEvent *e)
{
    if (m_currStep == Step::ChooseTarget) {
        m_currStep = Step::ChooseSource;
        commitPreviewItem();
        createPreviewItem();

        if (m_messageItem) {
            const QPointF &scenePos = this->cursorInScene(e->globalPos());
            movePreviewItemTo(scenePos);
        }
    }
}

void MessageCreatorTool::processMouseMoveDrag(QMouseEvent *e)
{
    if (m_messageItem) {
        const QPointF &scenePos = this->cursorInScene(e->globalPos());
        switch (m_currStep) {
        case Step::ChooseSource: {
            movePreviewItemTo(scenePos);
            break;
        }
        case Step::ChooseTarget: {
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            break;
        }
        }
    }
}

void MessageCreatorTool::processMousePressClick(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void MessageCreatorTool::processMouseReleaseClick(QMouseEvent *e)
{
    switch (m_currStep) {
    case Step::ChooseSource: {
        m_messageItem->setTail(cursorInScene(e->globalPos()), ObjectAnchor::Snap::NoSnap);
        m_currStep = Step::ChooseTarget;
        break;
    }
    case Step::ChooseTarget: {
        const QPointF &scenePos = this->cursorInScene(e->globalPos());
        const bool isClick = m_currMode == InteractionMode::Click;
        const bool needCommit = m_currMode == InteractionMode::Drag || (isClick && e->modifiers() != Qt::NoModifier);
        if (needCommit) {
            commitPreviewItem();
            m_currStep = Step::ChooseSource;
            createPreviewItem();

            if (m_messageItem) {
                movePreviewItemTo(scenePos);
            }
        } else if (isClick) {
            m_messageItem->addMessagePoint(scenePos);
        }

        break;
    }
    }
}

void MessageCreatorTool::processMouseMoveClick(QMouseEvent *e)
{
    if (m_messageItem) {
        const QPointF &scenePos = cursorInScene(e->globalPos());
        switch (m_currStep) {
        case Step::ChooseSource: {
            movePreviewItemTo(scenePos);
            break;
        }
        case Step::ChooseTarget: {
            m_messageItem->setHead(scenePos, ObjectAnchor::Snap::NoSnap);
            break;
        }
        }
    }
}

QVariantList MessageCreatorTool::prepareMessage()
{
    // Depending on the pointing device, sometimes it's possible to get such event flow:
    //
    // onMousePress:   current(210,76) m_mouseDown(0,0)
    // onMouseMove:    current(210,75) m_mouseDown(210,76)
    // onMouseMove:    current(210,75) m_mouseDown(210,76)
    // onMouseRelease: current(210,75) m_mouseDown(210,76)
    //
    // While technicaly it's a drag, mostprobably it was not the user's intent to
    // create such a short arrow.
    // That's a reason for arrow geometry validation - it should intersect the geometry of
    // source or target instance, otherwise message would be discarded silently.
    //
    // Same for "async" messages (which currently are not supported) detection -
    // message discarded in case an arrow's delta y > 15 pixels.

    auto arrowLine = [&]() {
        if (m_view && m_messageItem)
            return QLineF(m_messageItem->tail(), m_messageItem->head());
        return QLineF();
    };

    auto instanceItemRect = [&](MscInstance *instance) {
        if (instance)
            if (InstanceItem *item = m_model->itemForInstance(instance)) {
                return item->sceneBoundingRect();
            }
        return QRectF();
    };

    auto ensureIntersected = [&](const QLineF &arrow, MscInstance *instance) {
        const QRectF r = instanceItemRect(instance);
        if (!r.isNull() && !utils::intersects(r, arrow))
            return false;
        return true;
    };

    auto validateArrow = [&](MscMessage *message) {
        const QLineF &arrow = arrowLine();

        const bool exceedsInstanceWidth = ensureIntersected(arrow, message->sourceInstance())
                && ensureIntersected(arrow, message->targetInstance());
        if (!exceedsInstanceWidth) {
            qWarning() << "Message intersects neither source nor target instance bounds, discarded.";
            return false;
        }

        return true;
    };

    auto getCif = [&](const QVector<QPointF> &sceneCoords) {
        if (sceneCoords.size() > 2)
            return utils::sceneToCif(sceneCoords, m_scene);

        const QLineF &arrow = arrowLine();
        static constexpr qreal horizontalityTolerancePixels = 15.;
        const bool isHorizontal = qAbs(arrow.dy()) <= horizontalityTolerancePixels;

        return isHorizontal ? QVector<QPoint>() : utils::sceneToCif(sceneCoords, m_scene);
    };
    QVariantList args;

    auto message = qobject_cast<msc::MscMessage *>(m_previewEntity);
    if (validateArrow(message)) {
        if (!message->isOrphan()) {
            if (message->sourceInstance() == message->targetInstance())
                message->setTargetInstance(nullptr);

            const int eventIndex = m_model->eventIndex(m_previewItem->y());
            args = { QVariant::fromValue<msc::MscMessage *>(message),
                     QVariant::fromValue<msc::MscChart *>(m_activeChart), eventIndex,
                     QVariant::fromValue<QVector<QPoint>>(getCif(m_messageItem->messagePoints())) };
        }
    }

    return args;
}

void MessageCreatorTool::movePreviewItemTo(const QPointF &newScenePos)
{
    m_messageItem->setHead(newScenePos, ObjectAnchor::Snap::NoSnap);
    m_messageItem->setTail(newScenePos, ObjectAnchor::Snap::NoSnap);
    m_messageItem->setPos(newScenePos);
}

void MessageCreatorTool::activate()
{
    m_currMode = InteractionMode::None;
    m_mouseDown = cursorInScene();
    setActive(true);
}

} // ns msc
