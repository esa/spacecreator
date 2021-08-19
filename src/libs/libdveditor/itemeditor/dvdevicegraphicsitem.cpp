/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvdevicegraphicsitem.h"

#include "dvconnectiongraphicsitem.h"
#include "dvnodegraphicsitem.h"
#include "graphicsviewutils.h"
#include "positionlookuphelper.h"
#include "ui/textitem.h"

#include <QPainter>

static const QRectF kBoundingRect = { -10, -10, 20, 20 };

namespace dve {

DVDeviceGraphicsItem::DVDeviceGraphicsItem(dvm::DVDevice *device, QGraphicsItem *parent)
    : shared::ui::VEConnectionEndPointGraphicsItem(device, parent)
    , m_textItem(new shared::ui::TextItem(this))
{
    m_adjustDirection = shared::graphicsviewutils::LookupDirection::CounterClockwise;

    setBoundingRect(kBoundingRect);
    m_textItem->setPos(QPointF(5, -5));
    m_textItem->setFramed(false);
    m_textItem->setBackground(Qt::transparent);
    m_textItem->setEditable(true);
}

DVDeviceGraphicsItem::~DVDeviceGraphicsItem() { }

void DVDeviceGraphicsItem::init()
{
    shared::ui::VEConnectionEndPointGraphicsItem::init();
    m_textItem->setPlainText(entity()->titleUI());
    connect(entity(), &dvm::DVObject::titleChanged, m_textItem, &shared::ui::TextItem::setPlainText);
}

dvm::DVDevice *DVDeviceGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVDevice *>();
}

int DVDeviceGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

QPointF DVDeviceGraphicsItem::connectionEndPoint(const bool nestedConnection) const
{
    const QRectF deviceRect = sceneBoundingRect();
    if (auto parentGraphicsItem = parentItem()) {
        const QRectF parentRect = parentGraphicsItem->boundingRect();
        const Qt::Alignment alignment = shared::graphicsviewutils::getNearestSide(parentRect, pos());
        switch (alignment) {
        case Qt::AlignLeft:
            if (nestedConnection) {
                return { deviceRect.right(), QLineF(deviceRect.topRight(), deviceRect.bottomRight()).center().y() };
            } else {
                return { deviceRect.left(), QLineF(deviceRect.topLeft(), deviceRect.bottomLeft()).center().y() };
            }
        case Qt::AlignTop:
            if (nestedConnection) {
                return { QLineF(deviceRect.bottomLeft(), deviceRect.bottomRight()).center().x(), deviceRect.bottom() };
            } else {
                return { QLineF(deviceRect.topLeft(), deviceRect.topRight()).center().x(), deviceRect.top() };
            }
        case Qt::AlignRight:
            if (nestedConnection) {
                return { deviceRect.left(), QLineF(deviceRect.topLeft(), deviceRect.bottomLeft()).center().y() };
            } else {
                return { deviceRect.right(), QLineF(deviceRect.topRight(), deviceRect.bottomRight()).center().y() };
            }
        case Qt::AlignBottom:
            if (nestedConnection) {
                return { QLineF(deviceRect.topLeft(), deviceRect.topRight()).center().x(), deviceRect.top() };
            } else {
                return { QLineF(deviceRect.bottomLeft(), deviceRect.bottomRight()).center().x(), deviceRect.bottom() };
            }
        default:
            break;
        }
    }
    return deviceRect.center();
}

QPointF DVDeviceGraphicsItem::connectionEndPoint(shared::ui::VEConnectionGraphicsItem *connection) const
{
    if (connection) {
        const bool innerConnection = connection->entity()->parentObject() == entity()->parentObject();
        return connectionEndPoint(innerConnection);
    }
    return sceneBoundingRect().center();
}

DVNodeGraphicsItem *DVDeviceGraphicsItem::nodeItem() const
{
    return qgraphicsitem_cast<DVNodeGraphicsItem *>(targetItem());
}

void DVDeviceGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void DVDeviceGraphicsItem::rebuildLayout()
{
    shared::ui::VEInteractiveObject::rebuildLayout();

    if (!targetItem()) {
        prepareGeometryChange();
        setBoundingRect(QRectF());
        return;
    }

    if (entity()) {
        if (shared::graphicsviewutils::pos(entity()->coordinates()).isNull()) {
            layout();
            mergeGeometry();
            return;
        }
    }
    const QRectF parentRect = targetItem()->boundingRect();
    const QPointF ifacePos = pos();
    const Qt::Alignment side = shared::graphicsviewutils::getNearestSide(parentRect, ifacePos);
    const QPointF stickyPos = shared::graphicsviewutils::getSidePosition(parentRect, ifacePos, side);
    setPos(stickyPos);
    updateInternalItems(side);
}

void DVDeviceGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!entity())
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    const QRectF br = boundingRect();
    painter->drawRect(br);
    painter->restore();
}

shared::ColorManager::HandledColors DVDeviceGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Device;
}

void DVDeviceGraphicsItem::updateInternalItems(Qt::Alignment alignment)
{
    QRectF textRect = m_textItem->boundingRect();
    switch (alignment) {
    case Qt::AlignLeft:
        textRect.moveBottomRight(kBoundingRect.topLeft());
        break;
    case Qt::AlignRight:
        textRect.moveBottomLeft(kBoundingRect.topRight());
        break;
    case Qt::AlignTop:
        textRect.moveBottomLeft(kBoundingRect.topRight());
        break;
    case Qt::AlignBottom:
        textRect.moveTopLeft(kBoundingRect.bottomRight());
        break;
    default:
        return;
    }
    m_textItem->setPos(textRect.topLeft());
}

QList<QPair<Qt::Alignment, QPainterPath> > DVDeviceGraphicsItem::sidePaths() const
{
    return {
        { Qt::AlignLeft, itemPath(Qt::AlignLeft) },
        { Qt::AlignRight, itemPath(Qt::AlignRight) },
        { Qt::AlignTop, itemPath(Qt::AlignTop) },
        { Qt::AlignBottom, itemPath(Qt::AlignBottom) },
    };
}

shared::graphicsviewutils::LookupDirection DVDeviceGraphicsItem::lookupType() const
{
    return shared::graphicsviewutils::LookupDirection::Mixed;
}

} // namespace dve
