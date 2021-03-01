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

#include "baseitems/common/mscutils.h"
#include "chartlayoutmanager.h"
#include "colors/colormanager.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "ui/grippoint.h"
#include "ui/grippointshandler.h"

#include <QDebug>
#include <QPainter>
#include <cmath>

namespace msc {

CoregionItem::CoregionItem(ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent)
    : InteractiveObject(nullptr, chartLayoutManager, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(ItemClipsToShape, false);
}

void CoregionItem::setBegin(MscCoregion *begin)
{
    m_begin = begin;
    m_entity = begin;
}

void CoregionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    shared::ColorHandler color = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::CoRegion);
    QColor penColor = color.pen().color();
    qreal penWidth = color.pen().widthF();

    painter->save();
    const QRectF rect = boundingRect();
    const QLineF topLine = { rect.topLeft(), rect.topRight() };
    const QLineF bottomLine = { rect.bottomLeft(), rect.bottomRight() };
    painter->setPen(QPen(penColor, penWidth, Qt::SolidLine));
    painter->drawLine(topLine);
    painter->drawLine(bottomLine);
    penWidth *= 1.5;
    painter->setPen(QPen(Qt::white, penWidth, Qt::SolidLine));
    painter->drawLine(topLine.center(), bottomLine.center());
    painter->setPen(QPen(penColor, penWidth, Qt::DashLine));
    if (m_unorderedEntities) {
        painter->drawLine(topLine.center(), bottomLine.center());
    } else {
        painter->drawLine(rect.topLeft(), rect.bottomLeft());
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

void CoregionItem::initGripPoints()
{
    InteractiveObjectBase::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Top, shared::ui::GripPoint::Bottom });
}

void CoregionItem::rebuildLayout()
{
    static const qreal kCoregionWidth = 50;
    static const qreal kOffset = 20;
    static const qreal kDefaultHeight = 40;

    if (m_instance) {
        m_instance->stackBefore(this);
    }

    prepareGeometryChange();
    if (!m_begin || !m_end || !m_instance || m_begin->instance() != m_instance->modelItem()
            || m_end->instance() != m_instance->modelItem()) {
        // Not fully defined. Set the default size
        setBoundingRect(QRectF(QPointF(0, 0), QSizeF(kCoregionWidth, kDefaultHeight)));
        return;
    }

    QRectF rect;
    const QVector<MscInstanceEvent *> &events =
            m_chartLayoutManager->currentChart()->eventsForInstance(m_instance->modelItem());
    auto it = std::next(std::find(events.constBegin(), events.constEnd(), m_begin));
    auto itEnd = std::find(events.constBegin(), events.constEnd(), m_end);
    while (it != events.end() && it != itEnd) {
        Q_ASSERT(*it);

        if (InteractiveObject *iObj = m_chartLayoutManager->itemForEntity(*it)) {
            auto messageItem = qobject_cast<MessageItem *>(iObj);
            if (messageItem != nullptr) {
                // insert the messages source or target - depending which is in the co-region
                QPointF pos;
                msc::InstanceItem *sourceItem = messageItem->sourceInstanceItem();
                if (sourceItem && (m_instance->modelItem() == sourceItem->modelItem())) {
                    pos = messageItem->messagePoints().first();
                }
                msc::InstanceItem *targetItem = messageItem->targetInstanceItem();
                if (targetItem && (m_instance.data()->modelItem() == targetItem->modelItem())) {
                    pos = messageItem->messagePoints().last();
                }
                rect |= QRectF(pos.x() - 1, pos.y() - 1, 2, 2);
            } else {
                rect |= iObj->sceneBoundingRect();
            }
            stackBefore(iObj);
        }
        ++it;
    }

    const QRectF instanceRect = m_instance->sceneBoundingRect();
    if (!rect.isValid()) {
        // No events inside this co-region. Set default size
        const qreal top = std::min(m_topMove.y(), kDefaultHeight - 1.);
        const qreal bottom = std::max(m_bottomMove.y(), 1.);
        const qreal height = m_bottomMove.isNull() ? kDefaultHeight - top : (bottom - top);
        setBoundingRect(QRectF(QPointF(0, top), QSizeF(kCoregionWidth, height)));
        setX(instanceRect.x() + (instanceRect.width() - kCoregionWidth) / 2);
        return;
    }

    // Set size from the events
    rect.setWidth(kCoregionWidth);
    rect.adjust(0, -kOffset, 0, kOffset);
    rect.translate(instanceRect.center().x() - rect.center().x(), 0);
    const QLineF axis = m_instance->axis();
    if (rect.top() < axis.y1())
        rect.setTop(axis.y1());
    if (rect.bottom() > axis.y2())
        rect.setBottom(axis.y2());

    setPos(rect.topLeft());

    // adapt height and top, when being resized
    const qreal top = std::min(m_topMove.y(), rect.height());
    const qreal bottom = std::max(m_bottomMove.y(), -rect.height());
    const qreal height = m_bottomMove.isNull() ? rect.height() - top : (bottom - top);
    rect.setHeight(height);

    setBoundingRect({ QPointF(0, top), rect.size() });
}

void CoregionItem::onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    if (gp->location() == shared::ui::GripPoint::Top) {
        m_topMove = mapFromScene(to);
    }
    if (gp->location() == shared::ui::GripPoint::Bottom) {
        m_bottomMove = mapFromScene(to);
    }
    instantLayoutUpdate();
}

void CoregionItem::onManualResizeFinish(shared::ui::GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(gp)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)

    Q_ASSERT(!m_chartLayoutManager.isNull());

    m_topMove = QPointF();
    m_bottomMove = QPointF();

    Q_EMIT moved(this);
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

    if (m_instance) {
        connect(m_instance, &InteractiveObject::relocated, this, &CoregionItem::scheduleLayoutUpdate,
                Qt::UniqueConnection);
        connect(m_instance, &InteractiveObject::moved, this, &CoregionItem::scheduleLayoutUpdate, Qt::UniqueConnection);
    }

    MscInstance *instanceEntity = m_instance ? m_instance->modelItem() : nullptr;
    if (m_begin)
        m_begin->setInstance(instanceEntity);
    if (m_end)
        m_end->setInstance(instanceEntity);
    instantLayoutUpdate();
}

} // namespace msc
