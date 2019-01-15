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
#include "baseitems/labeledarrowitem.h"
#include "baseitems/arrowitem.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/common/utils.h"
#include "baseitems/common/objectslink.h"
#include "commands/common/commandsstack.h"

#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

namespace msc {

MessageItem::MessageItem(MscMessage *message, InstanceItem *source, InstanceItem *target, qreal y,
                         QGraphicsItem *parent)
    : InteractiveObject(parent)
    , m_message(message)
    , m_arrowItem(new LabeledArrowItem(this))
{
    Q_ASSERT(m_message != nullptr);
    connect(m_message, &msc::MscMessage::nameChanged, this, &msc::MessageItem::setName);
    setName(m_message->name());

    connect(m_arrowItem, &LabeledArrowItem::layoutChanged, this, &MessageItem::commitGeometryChange);
    connect(m_arrowItem, &LabeledArrowItem::textEdited, this, &MessageItem::onRenamed);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges);

    connectObjects(source, target, y);

    m_arrowItem->setColor(QColor("#3e47e6")); // see https://git.vikingsoftware.com/esa/msceditor/issues/30
}

MscMessage *MessageItem::modelItem() const
{
    return m_message;
}

void MessageItem::connectObjects(InstanceItem *source, InstanceItem *target, qreal y)
{
    setPositionChangeIgnored(true); // ignore position changes in MessageItem::itemChange

    setY(y);
    if (source || target)
        setInstances(source, target);

    setPositionChangeIgnored(false);
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
        connect(m_sourceInstance, &InteractiveObject::relocated, this, &MessageItem::onSourceInstanceMoved,
                Qt::DirectConnection);
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
        connect(m_targetInstance, &InteractiveObject::relocated, this, &MessageItem::onTargetInstanceMoved,
                Qt::DirectConnection);
        m_message->setTargetInstance(m_targetInstance->modelItem());
    } else
        m_message->setTargetInstance(nullptr);

    updateTooltip();
}

void MessageItem::updateTooltip()
{
    const QString env(tr("Env"));
    setToolTip(tr("%1: %2→%3")
                       .arg(name(), m_sourceInstance ? m_sourceInstance->name() : env,
                            m_targetInstance ? m_targetInstance->name() : env));
}

QString MessageItem::name() const
{
    return m_arrowItem->text();
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

    const QPointF &linkCenterInScene =
            m_arrowItem->arrow()->makeArrow(m_sourceInstance, pntFrom, m_targetInstance, pntTo);
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
    if (m_gripPoints) {
        m_gripPoints->updateLayout();

        const QPointF &start(m_arrowItem->arrow()->anchorPointSource());
        const QPointF &end(m_arrowItem->arrow()->anchorPointTarget());

        m_gripPoints->setGripPointPos(GripPoint::Left, start);
        m_gripPoints->setGripPointPos(GripPoint::Right, end);
        m_gripPoints->setGripPointPos(GripPoint::Center, utils::lineCenter(QLineF(start, end)));
    }
}

QVariant MessageItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged: {
        if (proceedPositionChange()) {
            updateSourceAndTarget(value.toPointF() - m_prevPos);
        }
        break;
    }
    default:
        break;
    }
    return InteractiveObject::itemChange(change, value);
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

    if (!isAutoResizable())
        return res;

    // snap to the instance's center:
    const bool sourceFound(!prevSource && m_sourceInstance);
    const bool targetFound(!prevTarget && m_targetInstance);

    if (sourceFound) {
        const QPointF &instanceCenter(m_sourceInstance->boundingRect().center());
        const QPointF &instanceCenterScene(m_sourceInstance->mapToScene(instanceCenter));
        const QPointF newAnchor(instanceCenterScene.x(), shiftedSource.y());
        const QPointF &delta(newAnchor - m_arrowItem->arrow()->link()->source()->point());

        const bool updated =
                updateSource(newAnchor, isAutoResizable() ? ObjectAnchor::Snap::SnapTo : ObjectAnchor::Snap::NoSnap);

        if (updated && !m_targetInstance && !delta.isNull())
            updateTarget(m_arrowItem->arrow()->link()->target()->point() + delta, ObjectAnchor::Snap::NoSnap);
    }
    if (targetFound) {
        const QPointF &instanceCenter(m_targetInstance->boundingRect().center());
        const QPointF &instanceCenterScene(m_targetInstance->mapToScene(instanceCenter));
        const QPointF newAnchor(instanceCenterScene.x(), shiftedTarget.y());
        const QPointF &delta(newAnchor - m_arrowItem->arrow()->link()->target()->point());

        const bool updated =
                updateTarget(newAnchor, isAutoResizable() ? ObjectAnchor::Snap::SnapTo : ObjectAnchor::Snap::NoSnap);

        if (updated && !m_sourceInstance && !delta.isNull())
            updateSource(m_arrowItem->arrow()->link()->source()->point() + delta, ObjectAnchor::Snap::NoSnap);
    }

    return res;
}

bool MessageItem::updateSource(const QPointF &to, ObjectAnchor::Snap snap)
{
    setSourceInstanceItem(hoveredItem(to));
    const bool res = m_arrowItem->updateSource(m_sourceInstance, to, snap);
    updateGripPoints();
    commitGeometryChange();
    return res;
}

bool MessageItem::updateTarget(const QPointF &to, ObjectAnchor::Snap snap)
{
    setTargetInstanceItem(hoveredItem(to));
    const bool res = m_arrowItem->updateTarget(m_targetInstance, to, snap);
    updateGripPoints();
    commitGeometryChange();
    return res;
}

InstanceItem *MessageItem::hoveredItem(const QPointF &hoverPoint) const
{
    const QVector<InstanceItem *> &others = utils::itemByPos<InstanceItem, QPointF>(scene(), hoverPoint);
    return others.isEmpty() ? nullptr : others.first();
}

void MessageItem::commitGeometryChange()
{
    prepareGeometryChange();
    m_boundingRect = m_arrowItem->boundingRect();
}

void MessageItem::onSourceInstanceMoved(const QPointF &from, const QPointF &to)
{
    setPositionChangeIgnored(true);
    const QPointF offset(to - from);

    const QPointF srcPoint(m_arrowItem->arrow()->anchorPointSource() + offset);
    QPointF dstPoint(m_arrowItem->arrow()->anchorPointTarget());

    if (!m_targetInstance) {
        moveBy(offset.x(), offset.y());
        dstPoint += offset;
    }

    m_arrowItem->updatePoints(srcPoint, dstPoint);
    setPositionChangeIgnored(false);
}

void MessageItem::onTargetInstanceMoved(const QPointF &from, const QPointF &to)
{
    setPositionChangeIgnored(true);
    const QPointF offset(to - from);

    QPointF srcPoint(m_arrowItem->arrow()->anchorPointSource());
    const QPointF dstPoint(m_arrowItem->arrow()->anchorPointTarget() + offset);

    if (!m_sourceInstance) {
        moveBy(offset.x(), offset.y());
        srcPoint += offset;
    }

    m_arrowItem->updatePoints(srcPoint, dstPoint);
    setPositionChangeIgnored(false);
}

QPointF MessageItem::head() const
{
    return m_arrowItem->mapToScene(m_arrowItem->endSignPos());
}

void MessageItem::setHead(const QPointF &head, ObjectAnchor::Snap snap)
{
    if (head == this->head() && snap == ObjectAnchor::Snap::NoSnap)
        return;

    updateTarget(head, snap);
}

QPointF MessageItem::tail() const
{
    return m_arrowItem->mapToScene(m_arrowItem->startSignPos());
}

void MessageItem::setTail(const QPointF &tail, ObjectAnchor::Snap snap)
{
    if (tail == this->tail() && snap == ObjectAnchor::Snap::NoSnap)
        return;

    updateSource(tail, snap);
}

bool MessageItem::isAutoResizable() const
{
    return m_autoResize;
}

void MessageItem::setAutoResizable(bool resizable)
{
    if (resizable == m_autoResize)
        return;

    m_autoResize = resizable;
}

void MessageItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center)
        msc::cmd::CommandsStack::push(msc::cmd::Id::MoveMessage,
                                      { QVariant::fromValue<MessageItem *>(this), pos() + (to - from) });
}

void MessageItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    const QPointF &shift(to - from);
    if (gp->location() == GripPoint::Left) {
        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
                                      { QVariant::fromValue<MessageItem *>(this), head(), tail() + shift });
    } else if (gp->location() == GripPoint::Right) {
        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
                                      { QVariant::fromValue<MessageItem *>(this), head() + shift, tail() });
    }
    updateGripPoints();
}

MessageItem *MessageItem::createDefaultItem(MscMessage *message, const QPointF &pos)
{
    MessageItem *messageItem = new MessageItem(message, nullptr, nullptr, pos.y());
    static constexpr qreal halthLength(ArrowItem::DEFAULT_WIDTH / 2.);
    const QPointF head(halthLength, pos.y());
    const QPointF tail(-halthLength, pos.y());
    messageItem->setHead(head, ObjectAnchor::Snap::NoSnap);
    messageItem->setTail(tail, ObjectAnchor::Snap::NoSnap);
    messageItem->setPos(pos);

    return messageItem;
}

void MessageItem::performSnap()
{
    setHead(head(), ObjectAnchor::Snap::SnapTo);
    setTail(tail(), ObjectAnchor::Snap::SnapTo);
}

bool MessageItem::ignorePositionChange() const
{
    return m_posChangeIgnored;
}

bool MessageItem::proceedPositionChange() const
{
    return !ignorePositionChange();
}

void MessageItem::setPositionChangeIgnored(bool ignored)
{
    m_posChangeIgnored = ignored;
}

void MessageItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center, GripPoint::Location::Left, GripPoint::Location::Right });

    m_arrowItem->setZValue(m_gripPoints->zValue() - 1);
}

void MessageItem::onRenamed(const QString &title)
{
    if (title.isEmpty()) {
        m_arrowItem->setText(m_message->name());
        return;
    }

    if (m_message->name() == title)
        return;

    m_message->setName(title);

    updateLayout();
}

} // namespace msc
