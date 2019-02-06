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

#include "conditionitem.h"

#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"
#include "msccondition.h"

#include <QGraphicsPolygonItem>

namespace msc {

static const qreal CONDITION_WIDTH = 50.0;
static const qreal CONDITION_HEIGHT = 25.0;
static const qreal CONDITION_MARGIN = 10.0;
static const qreal MAX_TEXT_WIDTH = 150.0;

ConditionItem::ConditionItem(MscCondition *condition, QGraphicsItem *parent)
    : InteractiveObject(condition, parent)
    , m_condition(condition)
    , m_polygonItem(new QGraphicsPolygonItem(this))
    , m_nameItem(new TextItem(this))
{
    setName(m_condition->name());
    connect(m_condition, &msc::MscCondition::nameChanged, this, &msc::ConditionItem::setName);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_nameItem->setEditable(true);
    m_nameItem->setBackgroundColor(Qt::transparent);

    connect(m_nameItem, &TextItem::edited, this, &ConditionItem::onNameEdited, Qt::QueuedConnection);
    connect(m_nameItem, &TextItem::keyPressed, this, &ConditionItem::rebuildLayout);

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
    m_condition->setName(name);

    m_nameItem->setPlainText(name);
    m_nameItem->adjustSize();

    updateLayout();
    Q_EMIT needRelayout();
}

void ConditionItem::buildLayout()
{
    prepareGeometryChange();

    m_nameItem->setTextWrapMode(QTextOption::NoWrap);

    // set default size:
    QSizeF nameSize(m_nameItem->boundingRect().size());

    if (nameSize.width() > MAX_TEXT_WIDTH) {
        m_nameItem->setTextWrapMode(QTextOption::WrapAnywhere);
        m_nameItem->adjustSize();

        nameSize = m_nameItem->boundingRect().size();
    }

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
            x = m_InstancesRect.x() - qAbs((m_InstancesRect.width() - m_boundingRect.width()) / 2);
        }
    } else if (m_instance) {
        x = m_instance->centerInScene().x() - m_boundingRect.width() / 2;
    }

    setX(x);
}

void ConditionItem::onNameEdited(const QString &name)
{
    if (name.isEmpty()) {
        return;
    }

    using namespace msc::cmd;
    CommandsStack::push(RenameEntity, { QVariant::fromValue<MscEntity *>(this->modelItem()), name });
}

void ConditionItem::rebuildLayout()
{
    m_boundingRect = QRectF();
    buildLayout();
}

void ConditionItem::onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &, const QPointF &)
{
    if (pos == GripPoint::Location::Center) {
        Q_EMIT moved(this);
    }
}

void ConditionItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center) {
        const QPointF &delta = to - from;
        setPos(pos() + delta);
    }
}

void ConditionItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void ConditionItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center });
    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &ConditionItem::onManualGeometryChangeFinished, Qt::UniqueConnection);
}

} // namespace msc
