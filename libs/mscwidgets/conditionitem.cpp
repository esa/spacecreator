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

#include <QGraphicsPolygonItem>

#include "conditionitem.h"
#include "msccondition.h"

#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"

namespace msc {

static const qreal CONDITION_WIDTH = 50.0;
static const qreal CONDITION_HEIGHT = 20.0;
static const qreal CONDITION_MARGIN = 10.0;

ConditionItem::ConditionItem(MscCondition *condition, QGraphicsItem *parent)
    : InteractiveObject(parent)
    , m_condition(condition)
    , m_polygonItem(new QGraphicsPolygonItem(this))
    , m_nameItem(new TextItem(this))
{
    setName(m_condition->name());
    connect(m_condition, &msc::MscCondition::nameChanged, this, &msc::ConditionItem::setName);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges);

    m_nameItem->setEditable(true);
    m_nameItem->setBackgroundColor(Qt::transparent);
    connect(m_nameItem, &TextItem::edited, this, &ConditionItem::onNameEdited, Qt::QueuedConnection);

    m_polygonItem->setBrush(Qt::white);

    buildLayout();
}

MscCondition *ConditionItem::modelItem() const
{
    return m_condition;
}

QString ConditionItem::name() const
{
    return m_nameItem->toPlainText();
}

QPainterPath ConditionItem::shape() const
{
    QPainterPath result;
    result.addRect(m_polygonItem->boundingRect());
    result.addRect(m_nameItem->boundingRect());
    return result;
}

void ConditionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void ConditionItem::setBoundingRect(const QRectF &geometry)
{
    if (geometry == boundingRect())
        return;

    prepareGeometryChange();
    m_boundingRect = geometry;

    if (m_gripPoints)
        m_gripPoints->updateLayout();

    buildLayout();
}

void ConditionItem::connectObjects(InstanceItem *instance, qreal y, QRectF instancesRect)
{
    m_InstancesRect = instancesRect;

    setY(y);
    setInstance(instance);

    rebuildLayout();
}

void ConditionItem::setInstance(InstanceItem *instance)
{
    if (instance == m_instance) {
        return;
    }

    if (m_instance) {
        disconnect(m_instance, nullptr, this, nullptr);
    }

    m_instance = instance;
}

ConditionItem *ConditionItem::createDefaultItem(MscCondition *condition, const QPointF &pos)
{
    ConditionItem *item = new ConditionItem(condition);
    item->setPos(pos);

    return item;
}

void ConditionItem::setName(const QString &name)
{
    if (this->name() == name && m_condition->name() == name)
        return;

    m_condition->setName(name);

    m_nameItem->setPlainText(name);
    m_nameItem->adjustSize();

    rebuildLayout();

    Q_EMIT needRelayout();
}

void ConditionItem::buildLayout()
{
    prepareGeometryChange();

    // set default size:
    QSizeF nameSize(m_nameItem->boundingRect().size());

    if (m_boundingRect.isEmpty()) {
        m_boundingRect.setTopLeft({ 0.0, 0.0 });
        m_boundingRect.setWidth(qMax(nameSize.width() + CONDITION_MARGIN, CONDITION_WIDTH));
        m_boundingRect.setHeight(qMax(nameSize.height(), CONDITION_HEIGHT));

        updateGripPoints();
    }

    if (modelItem()->shared() && m_InstancesRect.isValid()) {
        m_boundingRect.setWidth(qMax(nameSize.width() + CONDITION_MARGIN, m_InstancesRect.width()));
        updateGripPoints();
    }

    QVector<QPointF> points;
    points.append(m_boundingRect.topLeft() + QPointF(CONDITION_MARGIN, 0));
    points.append(m_boundingRect.topRight() - QPointF(CONDITION_MARGIN, 0));
    points.append(m_boundingRect.topRight() + QPointF(0, (m_boundingRect.bottom() - m_boundingRect.top()) / 2));
    points.append(m_boundingRect.bottomRight() - QPointF(CONDITION_MARGIN, 0));
    points.append(m_boundingRect.bottomLeft() + QPointF(CONDITION_MARGIN, 0));
    points.append(m_boundingRect.topLeft() + QPointF(0, (m_boundingRect.bottom() - m_boundingRect.top()) / 2));
    m_polygonItem->setPolygon(points);

    // name in the middle of polygon
    const QPointF nameDelta = m_boundingRect.center() - m_nameItem->boundingRect().center();
    m_nameItem->setPos({ 0., 0. });
    m_nameItem->moveBy(nameDelta.x(), nameDelta.y());

    double x = 0;
    if (modelItem()->shared()) {
        if (m_InstancesRect.isValid()) {
            x = m_InstancesRect.x() - (m_InstancesRect.width() - m_boundingRect.width()) / 2;
        }
    } else if (m_instance) {
        x = m_instance->centerInScene().x() - m_boundingRect.width() / 2;
    }

    setX(x);
}

void ConditionItem::onNameEdited(const QString &name)
{
    if (name.isEmpty()) {
        m_nameItem->setPlainText(m_condition->name());
        return;
    }

    setName(name);
}

void ConditionItem::rebuildLayout()
{
    m_boundingRect = QRectF();
    buildLayout();
}

void ConditionItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center)
        msc::cmd::CommandsStack::push(cmd::Id::MoveCondition,
                                      { QVariant::fromValue<ConditionItem *>(this), pos() + (to - from) });
}

void ConditionItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    QPointF delta(to - from);
    QRectF newRect(m_boundingRect);

    switch (gp->location()) {
    case GripPoint::Location::Left:
    case GripPoint::Location::Right: {
        if (gp->location() == GripPoint::Location::Left)
            delta.rx() *= -1;
        newRect.adjust(-delta.x(), 0., delta.x(), 0.);
        break;
    }
    case GripPoint::Location::Top:
    case GripPoint::Location::Bottom: {
        newRect.adjust(0., -delta.y(), 0., delta.y());
        break;
    }
    case GripPoint::Location::TopLeft:
    case GripPoint::Location::TopRight:
    case GripPoint::Location::BottomLeft:
    case GripPoint::Location::BottomRight: {
        if (gp->location() == GripPoint::Location::Left || gp->location() == GripPoint::Location::TopLeft
            || gp->location() == GripPoint::Location::BottomLeft)
            delta.rx() *= -1;
        newRect.adjust(-delta.x(), -delta.y(), delta.x(), delta.y());
        break;
    }
    default:
        return;
    }

    msc::cmd::CommandsStack::push(cmd::Id::ResizeCondition, { QVariant::fromValue<ConditionItem *>(this), newRect });
}

} // namespace msc
