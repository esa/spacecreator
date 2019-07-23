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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "aadlinterfacegraphicsitem.h"

#include <QPainter>
#include <baseitems/common/utils.h>
#include <baseitems/grippointshandler.h>
#include <tab_aadl/aadlobjectiface.h>

static qreal kBase = 15;
static qreal kHeight = 12;

namespace taste3 {
namespace aadl {

AADLInterfaceGraphicsItem::AADLInterfaceGraphicsItem(AADLObjectIface *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_iface(new QGraphicsPathItem(this))
    , m_text(new QGraphicsTextItem(this))
{
    QPainterPath pp;
    pp.addPolygon(QVector<QPointF> { QPointF(-kHeight / 3, -kBase / 2), QPointF(-kHeight / 3, kBase / 2),
                                     QPointF(2 * kHeight / 3, 0) });
    pp.closeSubpath();
    m_iface->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    m_iface->setBrush(QColor(Qt::blue));
    m_iface->setPath(pp);

    m_text->setPlainText(tr("Interface Name"));

    setFlag(QGraphicsItem::ItemIsMovable, false);
}

AADLObjectIface *AADLInterfaceGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectIface *>(m_entity);
}

void AADLInterfaceGraphicsItem::setTargetItem(QGraphicsItem *item, const QPointF &pos)
{
    if (!item || item == m_item)
        return;

    setParentItem(item);
    setPos(mapFromScene(pos));
    m_item = item;
    instantLayoutUpdate();
}

void AADLInterfaceGraphicsItem::rebuildLayout()
{
    prepareGeometryChange();
    const Qt::Alignment alignment = utils::getNearestSide(m_item->boundingRect(), pos());
    const QPointF stickyPos = utils::getSidePosition(m_item->boundingRect(), pos(), alignment);
    setPos(stickyPos);
    const bool insideOut = entity()->direction() == AADLObjectIface::IfaceType::Provided;
    switch (alignment) {
    case Qt::AlignLeft:
        m_iface->setRotation(insideOut ? 180 : 0);
        break;
    case Qt::AlignRight:
        m_iface->setRotation(insideOut ? 0 : 180);
        break;
    case Qt::AlignTop:
        m_iface->setRotation(insideOut ? 270 : 90);
        break;
    case Qt::AlignBottom:
        m_iface->setRotation(insideOut ? 90 : 270);
        break;
    }
    m_boundingRect = m_iface->boundingRect();
}

void AADLInterfaceGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center });
}

} // namespace aadl
} // namespace taste3
