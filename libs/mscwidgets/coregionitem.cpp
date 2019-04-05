/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "coregionitem.h"

#include "baseitems/common/utils.h"
#include "chartviewmodel.h"
#include "instanceitem.h"
#include "mscchart.h"
#include "msccoregion.h"

#include <QPainter>

namespace msc {

CoregionItem::CoregionItem(ChartViewModel *model, QGraphicsItem *parent)
    : InteractiveObject(nullptr, parent)
    , m_model(model)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void CoregionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const QRectF rect = boundingRect();
    painter->fillRect(rect, Qt::white);
    const QLineF topLine = { rect.topLeft(), rect.topRight() };
    const QLineF bottomLine = { rect.bottomLeft(), rect.bottomRight() };
    painter->drawLine(topLine);
    painter->drawLine(bottomLine);
    painter->setPen(QPen(Qt::darkGray, 3., Qt::DashLine));
    if (m_unorderedEntities) {
        painter->drawLine(utils::lineCenter(topLine), utils::lineCenter(bottomLine));
    } else {
        painter->drawLine(rect.topLeft(), rect.bottomLeft());
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
    InteractiveObject::paint(painter, option, widget);
}

void CoregionItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints(GripPoint::Locations {});
}

void CoregionItem::rebuildLayout()
{
    static const qreal kCoregionWidth = 50;
    static const qreal kOffset = 20;

    prepareGeometryChange();
    m_boundingRect = QRectF();
    if (!m_begin || !m_end || !m_instance || m_begin->instance() != m_instance->modelItem()
        || m_end->instance() != m_instance->modelItem()) {
        m_boundingRect = QRectF(QPointF(0, 0), QSizeF(kCoregionWidth, 2 * kOffset));
        return;
    }

    QRectF rect;
    const QVector<MscInstanceEvent *> &events = m_model->currentChart()->instanceEvents();
    auto it = std::next(std::find(events.constBegin(), events.constEnd(), m_begin));
    while (it != events.end()) {
        Q_ASSERT(*it);

        if (InteractiveObject *iObj = m_model->itemForEntity(*it)) {
            rect |= iObj->sceneBoundingRect();
            stackBefore(iObj);
        }

        if (*it == m_end)
            break;

        ++it;
    }
    const QRectF instanceRect = m_instance->sceneBoundingRect();
    if (!rect.isValid()) {
        m_boundingRect = QRectF(QPointF(0, 0), QSizeF(kCoregionWidth, 2 * kOffset));
        setX(instanceRect.x() + (instanceRect.width() - kCoregionWidth) / 2);
        return;
    }

    rect.setWidth(kCoregionWidth);
    rect.adjust(0, -kOffset, 0, kOffset);
    rect.translate(instanceRect.center().x() - rect.center().x(), 0);
    m_boundingRect = { QPointF(0, 0), rect.size() };
    setPos(rect.topLeft());
}

void CoregionItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void CoregionItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void CoregionItem::connectObjects(InstanceItem *instance, qreal y)
{
    setY(y);
    setInstance(instance);
}

void CoregionItem::setInstance(InstanceItem *instance)
{
    if (instance == m_instance)
        return;

    if (m_instance)
        disconnect(m_instance, nullptr, this, nullptr);

    m_instance = instance;
    instance->stackBefore(this);

    if (m_instance) {
        connect(m_instance, &InteractiveObject::relocated, this, &CoregionItem::updateLayout, Qt::UniqueConnection);
        connect(m_instance, &InteractiveObject::moved, this, &CoregionItem::updateLayout, Qt::UniqueConnection);
    }

    MscInstance *instanceEntity = m_instance ? m_instance->modelItem() : nullptr;
    if (m_begin)
        m_begin->setInstance(instanceEntity);
    if (m_end)
        m_end->setInstance(instanceEntity);
}

} // namespace msc
