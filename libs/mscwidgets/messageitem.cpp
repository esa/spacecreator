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
#include "baseitems/arrowitem.h"
#include "baseitems/grippointshandler.h"

#include <mscmessage.h>

#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPainter>
#include <QGraphicsScene>

#include <QDebug>
#define LOG qDebug() << Q_FUNC_INFO << name()

namespace msc {

MessageItem::MessageItem(MscMessage *message, QGraphicsItem *parent)
    : InteractiveObject(parent)
    , m_message(message)
    , m_arrowItem(new ArrowItem(this))
{
    Q_ASSERT(m_message != nullptr);

    setName(m_message->name());
    connect(m_message, &msc::MscMessage::nameChanged, this, &msc::MessageItem::setName);

    connect(this, &msc::InteractiveObject::resized, this, &msc::MessageItem::updateLayout);
    connect(this, &msc::InteractiveObject::relocated, this, &msc::MessageItem::updateLayout);

    m_gripPoints->setUsedPoints(
            GripPoint::GPP_Center
            | GripPoint::GPP_Left
            | GripPoint::GPP_Right);

    for (GripPoint *gp : m_gripPoints->gripPoints())
        gp->setIsAnchor(true);

    updateLayout();

    m_arrowItem->setZValue(m_gripPoints->zValue() - 1);
}

void MessageItem::setSourceInstanceItem(InstanceItem *sourceInstance)
{
    if (sourceInstance == m_sourceInstance || sourceInstance == m_targetInstance) {
        return;
    }

    if (m_sourceInstance) {
        m_sourceInstance->unregisterOutgoing(this);
    }

    m_sourceInstance = sourceInstance;
    if (m_sourceInstance) {
        m_sourceInstance->registerOutgoing(this);
    }

    m_boundingRect = QRectF();
    updateLayout();

    if (m_sourceInstance) {
        m_message->setSourceInstance(m_sourceInstance->modelItem());
    } else {
        m_message->setSourceInstance(nullptr);
    }
}

void MessageItem::setTargetInstanceItem(InstanceItem *targetInstance)
{
    if (targetInstance == m_targetInstance || targetInstance == m_sourceInstance) {
        return;
    }

    if (m_targetInstance) {
        m_targetInstance->unregisterIncoming(this);
    }

    m_targetInstance = targetInstance;
    if (m_targetInstance) {
        m_targetInstance->registerIncoming(this);
    }

    m_boundingRect = QRectF();
    updateLayout();

    if (m_targetInstance) {
        m_message->setTargetInstance(m_targetInstance->modelItem());
    } else {
        m_message->setTargetInstance(nullptr);
    }
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
    QMetaObject::invokeMethod(this, "buildLayout", Qt::QueuedConnection);
}

void MessageItem::setName(const QString &name)
{
    m_arrowItem->setText(name);
    updateLayout();
    m_message->setName(name);
}

void MessageItem::buildLayout()
{
    if (m_boundingRect.isEmpty()) {
        auto itemCenterLocal = [this](InstanceItem *item) {
            return item
                    ? mapFromScene(item->mapToScene(item->boundingRect().center()))
                    : QPointF();
        };
        const QPointF fromC = itemCenterLocal(m_sourceInstance);
        const QPointF toC = itemCenterLocal(m_targetInstance);

        QPointF pntFrom, pntTo;
        if (m_sourceInstance && m_targetInstance) {
            const QPointF minShift(qMin(fromC.x(), toC.x()), 0.);
            pntFrom = fromC - minShift;
            pntTo = toC - minShift;
        } else if (m_sourceInstance) {
            pntFrom = fromC;
            pntTo = pntFrom - QPointF(ArrowItem::DEFAULT_WIDTH, 0.);
        } else if (m_targetInstance) {
            pntTo = toC;
            pntFrom = pntTo - QPointF(ArrowItem::DEFAULT_WIDTH, 0.);
        }

        m_arrowItem->setPointFrom(pntFrom);
        m_arrowItem->setPointTo(pntTo);

        prepareGeometryChange();
        m_boundingRect = m_arrowItem->boundingRect();
        setPos({ qMin(fromC.x(), toC.x()), y() - m_boundingRect.topLeft().y() });
    }

    m_arrowItem->setFromArrowVisible(m_sourceInstance && m_targetInstance);

    m_layoutDirty = false;
    QMetaObject::invokeMethod(this, "updateGripPoints", Qt::QueuedConnection);
}

void MessageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath MessageItem::shape() const
{
    QPainterPath res;
    res.addPath(m_arrowItem->shape());
    return res;
}

void MessageItem::updateAnchorSource(const QPointF &delta)
{
    if (m_sourceInstance) {
        m_arrowItem->setPointFrom(m_arrowItem->pointFrom() + delta);

        if (!m_targetInstance)
            m_arrowItem->setPointTo(m_arrowItem->pointTo() + delta);

        prepareGeometryChange();
        m_boundingRect = m_arrowItem->boundingRect();
        m_gripPoints->updateLayout();
    }
}

void MessageItem::updateAnchorTarget(const QPointF &delta)
{
    if (m_targetInstance) {
        m_arrowItem->setPointTo(m_arrowItem->pointTo() + delta);

        if (!m_sourceInstance)
            m_arrowItem->setPointFrom(m_arrowItem->pointFrom() + delta);

        prepareGeometryChange();
        m_boundingRect = m_arrowItem->boundingRect();
        m_gripPoints->updateLayout();
    }
}

void MessageItem::handleGripPointMovement(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (grip->location() == GripPoint::GPP_Center) {
        InteractiveObject::handleGripPointMovement(grip, from, to);
        return;
    }

    const QPointF fromLocal(m_arrowItem->mapFromScene(from));
    const QPointF toLocal(m_arrowItem->mapFromScene(to));

    const QLineF sourceLine(fromLocal, m_arrowItem->pointFrom());
    const QLineF targetLine(fromLocal, m_arrowItem->pointTo());

    const QPointF delta(toLocal - fromLocal);

    InstanceItem *overInstance(nullptr);
    QList<QGraphicsItem *> hovered(scene() ? scene()->items(to) : QList<QGraphicsItem *>());
    for (QGraphicsItem *gi : hovered)
        if (InstanceItem *instance = dynamic_cast<InstanceItem *>(gi))
            if (instance != m_sourceInstance && instance != m_targetInstance) {
                overInstance = instance;
                break;
            }

    switch (grip->location()) {
    case GripPoint::GPP_Left: {
        m_arrowItem->setPointFrom(m_arrowItem->pointFrom() + delta);
        setSourceInstanceItem(overInstance);
        break;
    }
    case GripPoint::GPP_Right: {
        m_arrowItem->setPointTo(m_arrowItem->pointTo() + delta);
        setTargetInstanceItem(overInstance);
        break;
    }
    default:
        return;
    }

    m_arrowItem->setFromArrowVisible(m_sourceInstance && m_targetInstance);
    m_arrowItem->setOrphanFrom(!m_sourceInstance);
    m_arrowItem->setOrphanTo(!m_targetInstance);

    prepareGeometryChange();
    m_boundingRect = m_arrowItem->boundingRect();
    m_gripPoints->updateLayout();
    updateGripPoints();
}

void MessageItem::updateGripPoints()
{
    if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::GPP_Left)) {
        gp->setPos(mapFromItem(m_arrowItem, m_arrowItem->pointFrom()));
    }

    if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::GPP_Right)) {
        gp->setPos(mapFromItem(m_arrowItem, m_arrowItem->pointTo()));
    }

    m_gripPoints->updateLayout();
}

} // namespace msc
