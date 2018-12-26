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

static const double CONDITION_HEIGHT = 20.0;
static const double CONDITION_MARGIN = 10.0;

ConditionItem::ConditionItem(MscCondition *condition, QGraphicsItem *parent)
    : InteractiveObject(parent)
    , m_condition(condition)
    , m_polygonItem(new QGraphicsPolygonItem(this))
    , m_nameItem(new TextItem(this))
{
    setName(m_condition->name());
    connect(m_condition, &msc::MscCondition::nameChanged, this, &msc::ConditionItem::setName);

    buildLayout();

    setFlags(QGraphicsItem::ItemSendsGeometryChanges);

    m_nameItem->setEditable(true);
    m_nameItem->setBackgroundColor(Qt::transparent);
    connect(m_nameItem, &TextItem::edited, this, &ConditionItem::onNameEdited, Qt::QueuedConnection);

    m_polygonItem->setBrush(Qt::white);
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

    m_boundingRect = QRectF();
    buildLayout();
}

void ConditionItem::buildLayout(qreal width)
{
    prepareGeometryChange();

    // set default size:
    QRectF nameRect({ 0., 0. }, m_nameItem->boundingRect().size());
    if (m_boundingRect.isEmpty()) {
        m_boundingRect.setTopLeft(nameRect.topLeft());
        m_boundingRect.setWidth(nameRect.width() + CONDITION_MARGIN);
        m_boundingRect.setHeight(qMax(nameRect.height(), CONDITION_HEIGHT));

        updateGripPoints();
    }

    if (!qFuzzyIsNull(width)) {
        m_boundingRect.setWidth(qMax(width, nameRect.width() + CONDITION_MARGIN));
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
    m_nameItem->setPos(-m_nameItem->pos() + nameDelta);
}

void ConditionItem::onNameEdited(const QString &name)
{
    if (name.isEmpty()) {
        m_nameItem->setPlainText(m_condition->name());
        return;
    }

    setName(name);
}

void ConditionItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center)
        msc::cmd::CommandsStack::push(cmd::Id::MoveCondition, { QVariant::fromValue<ConditionItem *>(this), pos() + (to - from) });
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
        if (gp->location() == GripPoint::Location::Left
            || gp->location() == GripPoint::Location::TopLeft
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
