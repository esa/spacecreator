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

static const qreal kCoregionWidth = 50;
static const qreal kDefaultHeight = 40;

CoregionItem::CoregionItem(ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent)
    : EventItem(nullptr, chartLayoutManager, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(ItemClipsToShape, false);

    setBoundingRect(QRectF(QPointF(0, 0), QSizeF(kCoregionWidth, kDefaultHeight)));
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
    if (m_instance) {
        m_instance->stackBefore(this);
    }

    //    prepareGeometryChange();
    if (!m_begin || !m_end || !m_instance || m_begin->instance() != m_instance->modelItem()
            || m_end->instance() != m_instance->modelItem()) {
        // Not fully defined. Set the default size
        setBoundingRect(QRectF(QPointF(0, 0), QSizeF(kCoregionWidth, kDefaultHeight)));
        return;
    }

    if (m_topMove.isNull() && m_bottomMove.isNull()) {
        return;
    }

    QRectF rect = boundingRect();
    const qreal top = m_topMove.isNull() ? 0. : m_topMove.y();
    const qreal bottom = m_bottomMove.isNull() ? rect.bottom() : m_bottomMove.y();
    const qreal height = std::max(bottom - top, kDefaultHeight / 2);
    rect.setHeight(height);
    setBoundingRect({ QPointF(0, top), rect.size() });
}

void CoregionItem::onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from)
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

void CoregionItem::setInstance(InstanceItem *instance)
{
    if (instance == m_instance) {
        return;
    }

    m_instance = instance;
}

} // namespace msc
