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
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/common/objectanchor.h"
#include "chartlayoutmanager.h"
#include "commands/cmdmessageitemcreate.h"
#include "instanceitem.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "msccreate.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QDebug>
#include <QMouseEvent>

namespace msc {

MessageCreatorTool::MessageCreatorTool(
        MscMessage::MessageType msgType, ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
    , m_messageType(msgType)
{
    if (msgType == MscMessage::MessageType::Create) {
        m_title = tr("Create (Drag)");
        m_description = tr("Create new Create item");
        m_icon = QIcon(":/icons/toolbar/create.svg");
    } else {
        m_title = tr("Message (Drag)");
        m_description = tr("Create new Message item");
        m_icon = QIcon(":/icons/toolbar/message.png");
    }
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

    m_message = m_messageType == MscMessage::MessageType::Message ? new MscMessage(tr("Message"))
                                                                  : new MscCreate(tr("Create"));
    m_messageItem = m_model->createDefaultMessageItem(m_message, cursorInScene());

    movePreviewItemTo(cursorInScene());

    if (!m_messageItem) {
        delete m_message.data();
        return;
    }

    m_previewItem = m_messageItem;
    m_previewEntity.reset(m_messageItem->modelItem());

    m_messageItem->setAutoResizable(false);

    m_scene->addItem(m_previewItem);

    m_currMode = InteractionMode::None;
}

bool isCyclicCreation(MscMessage *message)
{
    if (message->sourceInstance()->explicitCreator() == message->targetInstance()) {
        return true;
    }

    MscInstance *excludedCreator = message->targetInstance();

    MscInstance *currentCreator = message->sourceInstance();
    while (currentCreator) {
        currentCreator = currentCreator->explicitCreator();
        if (currentCreator == excludedCreator) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the create, as defined in \p message, is allowed
 */
bool MessageCreatorTool::validateCreate(MscMessage *message) const
{
    if (message->sourceInstance() && message->targetInstance()) {
        if (message->sourceInstance() != message->targetInstance()) {
            if (message->targetInstance()->explicitCreator() == nullptr) {
                if (!isCyclicCreation(message)) {
                    return true;
                } else {
                    qWarning() << "Cyclic CREATE messages are not allowed";
                }
            } else {
                const QString &wrn = QString("The %1 instance already have a CREATE message attached")
                                             .arg(message->targetInstance()->name());
                qWarning() << wrn;
            }
        } else {
            qWarning() << "CREATE messages need to be connected to different existing instances";
        }
    } else {
        qWarning() << "CREATE message can't be global";
    }

    return false;
}

void MessageCreatorTool::commitPreviewItem()
{
    m_currMode = InteractionMode::None;
    m_mouseDown = QPointF();

    if (m_message && m_activeChart && m_messageItem) {
        m_message->setSourceInstance(
                m_messageItem->sourceInstanceItem() ? m_messageItem->sourceInstanceItem()->modelItem() : nullptr);
        m_message->setTargetInstance(
                m_messageItem->targetInstanceItem() ? m_messageItem->targetInstanceItem()->modelItem() : nullptr);
        bool isValid = true;
        if (m_messageType == MscMessage::MessageType::Create) {
            isValid = validateCreate(m_message);
        }

        if (isValid) {
            const QVariantList &cmdParams = prepareMessage();
            if (!cmdParams.isEmpty()) {
                startWaitForModelLayoutComplete(m_message);
                removePreviewItem(); // Remove the preview item before the real one is added

                const int eventIndex = cmdParams.at(1).toInt();
                const QVector<QPoint> &points =
                        cmdParams.size() == 3 ? cmdParams.at(2).value<QVector<QPoint>>() : QVector<QPoint>();
                m_model->undoStack()->push(new cmd::CmdMessageItemCreate(m_message, eventIndex, m_model, points));

                Q_EMIT created(); // to deactivate toobar's item
            }
        }
    }

    removePreviewItem();
    m_message.clear();
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
        m_messageItem->setTail(cursorInScene(e->globalPos()), ObjectAnchor::Snap::SnapTo);
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
            updateMessageHead(scenePos);
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
    const QPointF &scenePos = this->cursorInScene(e->globalPos());
    switch (m_currStep) {
    case Step::ChooseSource: {
        m_messageItem->setTail(scenePos, ObjectAnchor::Snap::SnapTo);
        m_currStep = Step::ChooseTarget;
        break;
    }
    case Step::ChooseTarget: {
        // disable message creation with multiple points for now
        // const bool isClick = m_currMode == InteractionMode::Click;
        // const bool needCommit = m_currMode == InteractionMode::Drag || (isClick && e->modifiers() ==
        // Qt::AltModifier);
        // if (needCommit) {
        //    finishArrowCreation(scenePos);
        // } else if (isClick) {
        //    m_messageItem->addMessagePoint(scenePos);
        // }

        finishArrowCreation(scenePos);

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
            updateMessageHead(scenePos);
            break;
        }
        }
    }
}

bool MessageCreatorTool::validateUserPoints(msc::MscMessage *message)
{
    // Depending on the pointing device, sometimes it's possible to get such events flow:
    //
    // onMousePress:   current(210,76) m_mouseDown(0,0)
    // onMouseMove:    current(210,75) m_mouseDown(210,76)
    // onMouseMove:    current(210,75) m_mouseDown(210,76)
    // onMouseRelease: current(210,75) m_mouseDown(210,76)
    //
    // While technicaly it's a drag, mostprobably it was not the user's intent to
    // create such a short arrow.
    // That's the first reason for validation - a message should intersect geometry
    // of source or target instances, otherwise it will be discarded silently.
    //
    // If user has added 2+ same points in a row, these should be filtered out:
    // (10,10),(20,20),(20,20),(30,30),(30,30),(30,30) -> (10,10),(20,20),(30,30)
    //
    // Next thing is the general direction of the arrow, it should be Top to Bottom
    // (start.y <= end.y)
    //
    // At the end the start/end points are snapped to an appropriate axis x.

    if (!m_view || !m_messageItem || !message)
        return false;

    QVector<QPointF> points = m_messageItem->messagePoints();
    if (points.size() < 2)
        return false;

    auto instanceItemRect = [&](MscInstance *instance) {
        if (instance)
            if (InstanceItem *item = m_model->itemForInstance(instance))
                return item->extendedSceneBoundingRect();

        return QRectF();
    };

    auto ensureIntersected = [&](const QLineF &arrow, MscInstance *instance) {
        const QRectF r = instanceItemRect(instance);
        if (!r.isNull() && !shared::graphicsviewutils::intersects(r, arrow))
            return false;
        return true;
    };

    auto snapPoint = [&instanceItemRect](MscInstance *instance, const QPointF &edge) {
        if (!instance)
            return edge;

        const QRectF &instanceRect = instanceItemRect(instance);
        if (instanceRect.isNull())
            return edge;

        return QPointF(instanceRect.center().x(), edge.y());
    };

    auto snapToInstance = [&](MscInstance *instance, QVector<QPointF> &points, int pointNumber) {
        const QPointF &selectedPoint = points.at(pointNumber);
        const QPointF &snapped = snapPoint(instance, selectedPoint);
        if (snapped != selectedPoint)
            points.replace(pointNumber, snapped);
    };

    const QLineF &arrow = { points.first(), points.last() };
    const bool exceedsInstanceWidth =
            ensureIntersected(arrow, message->sourceInstance()) && ensureIntersected(arrow, message->targetInstance());
    if (!exceedsInstanceWidth) {
        qWarning() << "Message intersects neither source nor target instance bounds, discarded.";
        return false;
    }

    // Remove one of two same points in a row:
    int pointId = points.size() - 1;
    while (pointId > 1) {
        const QPointF current = points.at(pointId);
        const QPointF preCurrent = points.at(pointId - 1);
        if (current == preCurrent)
            points.removeAt(pointId);
        --pointId;
    }

    // prevent points to have negative coordinates:
    for (int i = 0; i < points.size(); ++i) {
        QPointF point(points.at(i));
        if (point.x() < 0.)
            point.rx() = 0.;
        if (point.y() < 0.)
            point.ry() = 0.;

        if (point != points.at(i))
            points.replace(i, point);
    }

    // snap to instances, if any:
    if (MscInstance *sourceInstance = message->sourceInstance())
        snapToInstance(sourceInstance, points, 0);

    if (MscInstance *targetInstance = message->targetInstance())
        snapToInstance(targetInstance, points, points.size() - 1);

    m_messageItem->setMessagePoints(points);

    return points.size() > 1;
}

QVariantList MessageCreatorTool::prepareMessage()
{
    auto getCif = [&](const QVector<QPointF> &sceneCoords) {
        QVector<QPoint> retPoints;

        if (sceneCoords.size() < 2)
            return retPoints;

        if (sceneCoords.size() > 2) {
            retPoints = CoordinatesConverter::sceneToCif(sceneCoords);
        } else {
            retPoints = isHorizontal(sceneCoords) ? QVector<QPoint>() : CoordinatesConverter::sceneToCif(sceneCoords);
        }
        return retPoints;
    };

    QVariantList args;
    auto message = qobject_cast<msc::MscMessage *>(m_previewEntity.take());
    if (validateUserPoints(message)) {
        if (!message->isOrphan()) {
            if (message->sourceInstance() == message->targetInstance())
                message->setTargetInstance(nullptr);

            const int eventIndex = m_model->eventIndex(m_messageItem->tail().y());
            const QVector<QPoint> &arrowPoints = getCif(m_messageItem->messagePoints());
            args = { QVariant::fromValue<msc::MscMessage *>(message), eventIndex,
                QVariant::fromValue<QVector<QPoint>>(arrowPoints) };
        }
    } else {
        m_previewEntity.reset(message); // make it be clearable within removePreviewItem
    }

    return args;
}

void MessageCreatorTool::movePreviewItemTo(const QPointF &newScenePos)
{
    MessageItem::GeometryNotificationBlocker lock(m_messageItem);
    m_messageItem->setTail(newScenePos, ObjectAnchor::Snap::SnapTo);
    m_messageItem->setHead(newScenePos, ObjectAnchor::Snap::SnapTo);
    m_messageItem->setPos(newScenePos);
}

void MessageCreatorTool::activate()
{
    m_currMode = InteractionMode::None;
    m_mouseDown = cursorInScene();
    setActive(true);
}

bool MessageCreatorTool::processKeyPress(QKeyEvent *e)
{
    if (BaseTool::processKeyPress(e))
        return true;

    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return: {
        if (m_currMode == InteractionMode::Click && m_currStep == Step::ChooseTarget) {
            const QPointF &scenePos = cursorInScene();
            m_messageItem->addMessagePoint(scenePos);
            finishArrowCreation(scenePos);
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

void MessageCreatorTool::finishArrowCreation(const QPointF &scenePos)
{
    commitPreviewItem();
    m_currStep = Step::ChooseSource;
    createPreviewItem();

    if (m_messageItem) {
        movePreviewItemTo(scenePos);
    }
}

void MessageCreatorTool::updateMessageHead(const QPointF &to)
{
    QPointF head(to);
    const QPointF &tail = m_messageItem->tail();

    // prevent the arrow being upward:
    if (head.y() < tail.y())
        head.ry() = tail.y();

    m_messageItem->setHead(head, ObjectAnchor::Snap::SnapTo);
}

} // ns msc
