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

#include "conditionitem.h"

#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"
#include "msccondition.h"
#include "ui/grippointshandler.h"

#include <QDebug>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

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
    Q_ASSERT(m_condition != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_polygonItem->setBrush(Qt::white);
    m_polygonItem->setFlag(QGraphicsItem::ItemStacksBehindParent, true);

    m_nameItem->setMscValidationTest(QString("msc c1;instance i1;condition %1;endinstance;endmsc;"));
    m_nameItem->setEditable(true);
    setName(m_condition->name());
    m_nameItem->setBackgroundColor(Qt::transparent);
    m_nameItem->setSendClickEvent(true);

    connect(m_condition, &msc::MscCondition::nameChanged, this, &msc::ConditionItem::setName);

    connect(m_nameItem, &TextItem::edited, this, &ConditionItem::onNameEdited, Qt::QueuedConnection);
    connect(m_nameItem, &TextItem::textChanged, this, &ConditionItem::rebuildLayout);
    connect(m_nameItem, &TextItem::clicked, this, [&]() { this->setSelected(true); });

    scheduleLayoutUpdate();
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

void ConditionItem::connectObjects(InstanceItem *instance, qreal y, const QRectF &instancesRect)
{
    m_InstancesRect = instancesRect;

    setY(y);
    setInstance(instance);

    rebuildLayout();
}

void ConditionItem::setInstancesRect(const QRectF &instancesRect)
{
    if (!modelItem()->shared()) {
        return;
    }
    if (m_InstancesRect != instancesRect) {
        m_InstancesRect = instancesRect;
        instantLayoutUpdate();
    }
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
    if (m_instance)
        connect(m_instance, &InteractiveObject::relocated, this, &ConditionItem::onInstanceMoved, Qt::UniqueConnection);

    scheduleLayoutUpdate();
}

ConditionItem *ConditionItem::createDefaultItem(MscCondition *condition, const QPointF &pos)
{
    ConditionItem *item = new ConditionItem(condition);
    item->setPos(pos);

    return item;
}

void ConditionItem::setName(const QString &name)
{
    m_nameItem->setPlainText(name);
    m_nameItem->adjustSize();

    scheduleLayoutUpdate();
}

void ConditionItem::buildLayout()
{

    m_nameItem->setTextWrapMode(QTextOption::ManualWrap);

    // set default size:
    QSizeF nameSize(m_nameItem->boundingRect().size());
    prepareGeometryChange();

    if (nameSize.width()
            > (modelItem()->shared() && m_InstancesRect.isValid() ? m_InstancesRect.width() : MAX_TEXT_WIDTH)) {
        m_nameItem->setTextWrapMode(QTextOption::WrapAnywhere);
        m_nameItem->setTextWidth(m_InstancesRect.width() - 2 * CONDITION_MARGIN);

        nameSize = m_nameItem->boundingRect().size();
    }

    QRectF br = boundingRect();
    if (br.isEmpty()) {
        br.setTopLeft({ 0.0, 0.0 });
        br.setWidth(qMax(nameSize.width() + CONDITION_MARGIN, CONDITION_WIDTH));
        br.setHeight(qMax(nameSize.height(), CONDITION_HEIGHT));
        setBoundingRect(br);
    }

    if (modelItem()->shared() && m_InstancesRect.isValid()) {
        br.setWidth(qMax(nameSize.width() + CONDITION_MARGIN, m_InstancesRect.width()));
        setBoundingRect(br);
    }

    QVector<QPointF> points;
    points.append(br.topLeft() + QPointF(CONDITION_MARGIN, 0));
    points.append(br.topRight() - QPointF(CONDITION_MARGIN, 0));
    points.append(br.topRight() + QPointF(0, (br.bottom() - br.top()) / 2));
    points.append(br.bottomRight() - QPointF(CONDITION_MARGIN, 0));
    points.append(br.bottomLeft() + QPointF(CONDITION_MARGIN, 0));
    points.append(br.topLeft() + QPointF(0, (br.bottom() - br.top()) / 2));
    m_polygonItem->setPolygon(points);

    // name in the middle of polygon
    const QPointF nameDelta = br.center() - m_nameItem->boundingRect().center();
    m_nameItem->setPos({ 0., 0. });
    m_nameItem->moveBy(nameDelta.x(), nameDelta.y());

    double x = 0;
    if (modelItem()->shared()) {
        if (m_InstancesRect.isValid()) {
            x = m_InstancesRect.x() - qAbs((m_InstancesRect.width() - br.width()) / 2);
        }
    } else if (m_instance) {
        x = m_instance->centerInScene().x() - br.width() / 2;
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
    setBoundingRect(QRectF());
    buildLayout();
}

void ConditionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mousePressEvent(event);
    setSelected(true);
    event->accept();
}

void ConditionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mouseDoubleClickEvent(event);
    m_nameItem->enableEditMode();
    event->accept();
}

void ConditionItem::onManualMoveProgress(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    moveBy(0., to.y() - from.y());
}

void ConditionItem::onManualMoveFinish(shared::ui::GripPoint *, const QPointF &, const QPointF &)
{
    Q_EMIT moved(this);
}

void ConditionItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Center });
}

void ConditionItem::onInstanceMoved(const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    scheduleLayoutUpdate();
}

} // namespace msc
