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

#include "messageitem.h"
#include "instanceitem.h"
#include "baseitems/labeledarrowitem.h"
#include "baseitems/arrowitem.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/common/utils.h"
#include "baseitems/common/objectslink.h"

#include <mscmessage.h>

#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPainter>
#include <QGraphicsScene>

namespace msc {

MessageItem::MessageItem(MscMessage *message, InstanceItem *source, InstanceItem *target, qreal y, QGraphicsItem *parent)
    : InteractiveObject(parent)
    , m_message(message)
    , m_arrowItem(new LabeledArrowItem(this))
{
    Q_ASSERT(m_message != nullptr);
    connect(m_message, &msc::MscMessage::nameChanged, this, &msc::MessageItem::setName);
    setName(m_message->name());

    connect(m_arrowItem, &LabeledArrowItem::layoutChanged, this, &MessageItem::commitGeometryChange);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges);

    m_gripPoints->setUsedPoints( {GripPoint::Location::Center,
                                  GripPoint::Location::Left,
                                  GripPoint::Location::Right } );

    for (GripPoint *gp : m_gripPoints->gripPoints())
        gp->setGripType(GripPoint::GripType::Mover);

    m_arrowItem->setZValue(m_gripPoints->zValue() - 1);

    connectObjects(source, target, y);
}

void MessageItem::connectObjects(InstanceItem *source, InstanceItem *target, qreal y)
{
    m_connectingObjects = true; // ignore position changes in MessageItem::itemChange

    setY(y);
    if (source || target)
        setInstances(source, target);

    m_connectingObjects = false;
}

void MessageItem::setInstances(InstanceItem *sourceInstance, InstanceItem *targetInstance)
{
    m_layoutDirty = true;
    setSourceInstanceItem(sourceInstance);
    setTargetInstanceItem(targetInstance);
    m_layoutDirty = false;
    rebuildLayout();
}

void MessageItem::setSourceInstanceItem(InstanceItem *sourceInstance)
{
    if (sourceInstance == m_sourceInstance) {
        return;
    }

    if (m_sourceInstance) {
        disconnect(m_sourceInstance, nullptr, this, nullptr);
    }

    m_sourceInstance = sourceInstance;
    if (m_sourceInstance) {
        connect(m_sourceInstance, &InteractiveObject::relocated, this, &MessageItem::onSourceMoved);
        m_message->setSourceInstance(m_sourceInstance->modelItem());
    } else
        m_message->setSourceInstance(nullptr);

    updateTooltip();
}

void MessageItem::setTargetInstanceItem(InstanceItem *targetInstance)
{
    if (targetInstance == m_targetInstance) {
        return;
    }

    if (m_targetInstance) {
        disconnect(m_targetInstance, nullptr, this, nullptr);
    }

    m_targetInstance = targetInstance;
    if (m_targetInstance) {
        connect(m_targetInstance, &InteractiveObject::relocated, this, &MessageItem::onTargetMoved);
        m_message->setTargetInstance(m_targetInstance->modelItem());
    } else
        m_message->setTargetInstance(nullptr);

    updateTooltip();
}

void MessageItem::updateTooltip()
{
    const QString env(tr("Env"));
    setToolTip(tr("%1: %2→%3")
                       .arg(name(),
                            m_sourceInstance ? m_sourceInstance->name() : env,
                            m_targetInstance ? m_targetInstance->name() : env));
}

QString MessageItem::name() const
{
    return m_arrowItem->text();
}

void MessageItem::updateLayout()
{
    if (m_layoutDirty) {
        return;
    }

    m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "rebuildLayout", Qt::QueuedConnection);
}

void MessageItem::setName(const QString &name)
{
    m_message->setName(name);
    m_arrowItem->setText(m_message->name());
}

QRectF MessageItem::boundingRect() const
{
    return m_arrowItem->boundingRect();
}

void MessageItem::rebuildLayout()
{
    auto itemCenterScene = [](InstanceItem *item) {
        QPointF res;
        if (item) {
            const QPointF b = item->boundingRect().center();
            res = item->mapToScene(b);
        }

        return res;
    };
    const QPointF fromC(itemCenterScene(m_sourceInstance).x(), y());
    const QPointF toC(itemCenterScene(m_targetInstance).x(), y());

    QPointF pntFrom, pntTo;
    if (m_sourceInstance && m_targetInstance) {
        pntFrom = fromC;
        pntTo = toC;
    } else if (m_sourceInstance) {
        pntFrom = fromC;
        pntTo = pntFrom - QPointF(ArrowItem::DEFAULT_WIDTH, 0.);
    } else if (m_targetInstance) {
        pntTo = toC;
        pntFrom = pntTo - QPointF(ArrowItem::DEFAULT_WIDTH, 0.);
    }

    const QPointF &linkCenterInScene = m_arrowItem->arrow()->makeArrow(
            m_sourceInstance, pntFrom, m_targetInstance, pntTo);
    setPos(linkCenterInScene);

    m_layoutDirty = false;
    commitGeometryChange();
}

void MessageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath MessageItem::shape() const
{
    QPainterPath result;
    result.addPath(m_arrowItem->shape());
    return result;
}

void MessageItem::updateGripPoints()
{
    m_gripPoints->updateLayout();

    auto updateEdge = [this](GripPoint *gp, const QPointF &arrowPos) {
        const QRectF &currentGripPointRect(gp->boundingRect());
        const QPointF &posInMe(mapFromItem(m_arrowItem, arrowPos));
        gp->setPos(posInMe - currentGripPointRect.center());
    };

    if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Left)) {
        updateEdge(gp, m_arrowItem->arrowHead());
    }

    if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Right)) {
        updateEdge(gp, m_arrowItem->arrowTail());
    }

    if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Center)) {
        const QLineF arrowAxis(m_arrowItem->arrowHead(), m_arrowItem->arrowTail());
        updateEdge(gp, arrowAxis.center());
    }
}

QVariant MessageItem::itemChange(GraphicsItemChange change,
                                 const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged: {
        if (!m_connectingObjects) {
            const QPointF shift(value.toPointF() - m_prevPos);
            updateSourceAndTarget(shift);
        }
        break;
    }
    default:
        break;
    }
    return InteractiveObject::itemChange(change, value);
}

void MessageItem::handleGripPointMovement(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    switch (grip->location()) {
    case GripPoint::Center: {
        const QPointF shift(to - from);
        moveBy(shift.x(), shift.y());
        return;
    }
    case GripPoint::Left: {
        updateSource(to, ObjectAnchor::Snap::SnapTo);
        break;
    }
    case GripPoint::Right: {
        updateTarget(to, ObjectAnchor::Snap::SnapTo);
        break;
    }
    default:
        return;
    }
}

bool MessageItem::updateSourceAndTarget(const QPointF &shift)
{
    bool res(false);
    const InstanceItem *prevSource(m_sourceInstance);
    const InstanceItem *prevTarget(m_targetInstance);

    const QPointF shiftedSource(m_arrowItem->arrow()->anchorPointSource() + shift);
    res |= updateSource(shiftedSource, ObjectAnchor::Snap::NoSnap);
    const QPointF shiftedTarget(m_arrowItem->arrow()->anchorPointTarget() + shift);
    res |= updateTarget(shiftedTarget, ObjectAnchor::Snap::NoSnap);

    // snap to the instance's center:
    const bool sourceFound(!prevSource && m_sourceInstance);
    const bool targetFound(!prevTarget && m_targetInstance);

    if (sourceFound) {
        const QPointF instanceCenter(m_sourceInstance->boundingRect().center());
        const QPointF instanceCenterScene(m_sourceInstance->mapToScene(instanceCenter));
        const QPointF newAnchor(instanceCenterScene.x(), shiftedSource.y());
        updateSource(newAnchor, ObjectAnchor::Snap::SnapTo);
    }
    if (targetFound) {
        const QPointF instanceCenter(m_targetInstance->boundingRect().center());
        const QPointF instanceCenterScene(m_targetInstance->mapToScene(instanceCenter));
        const QPointF newAnchor(instanceCenterScene.x(), shiftedTarget.y());

        updateTarget(newAnchor, ObjectAnchor::Snap::SnapTo);
    }

    return res;
}

bool MessageItem::updateSource(const QPointF &to, ObjectAnchor::Snap snap)
{
    InstanceItem *hoveredInstance = utils::instanceByPos<InstanceItem>(scene(), to);

    setSourceInstanceItem(hoveredInstance);
    const bool res = m_arrowItem->replaceSource(m_sourceInstance, to, snap);

    commitGeometryChange();
    return res;
}

bool MessageItem::updateTarget(const QPointF &to, ObjectAnchor::Snap snap)
{
    InstanceItem *hoveredInstance = utils::instanceByPos<InstanceItem>(scene(), to);
    setTargetInstanceItem(hoveredInstance);
    const bool res = m_arrowItem->replaceTarget(m_targetInstance, to, snap);

    commitGeometryChange();
    return res;
}

void MessageItem::commitGeometryChange()
{
    prepareGeometryChange();
    const QRectF prevRect = m_boundingRect;
    m_boundingRect = m_arrowItem->boundingRect();
    m_gripPoints->updateLayout();
    updateGripPoints();

    if (prevRect != m_boundingRect)
        Q_EMIT resized(prevRect, m_boundingRect);
}

void MessageItem::onSourceMoved(const QPointF &from, const QPointF &to)
{
    const QPointF offset(to - from);
    if (!m_targetInstance) {
        moveBy(offset.x(), offset.y());
        return;
    }

    updateSource(m_arrowItem->arrow()->anchorPointSource() + offset, ObjectAnchor::Snap::NoSnap);
}

void MessageItem::onTargetMoved(const QPointF &from, const QPointF &to)
{
    const QPointF offset(to - from);
    if (!m_sourceInstance) {
        moveBy(offset.x(), offset.y());
        return;
    }

    updateTarget(m_arrowItem->arrow()->anchorPointTarget() + offset, ObjectAnchor::Snap::NoSnap);
}

} // namespace msc
