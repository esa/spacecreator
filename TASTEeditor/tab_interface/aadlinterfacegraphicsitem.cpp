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

#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "colors/colormanager.h"

#include <QPainter>
#include <QtDebug>
#include <baseitems/common/utils.h>
#include <baseitems/grippointshandler.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectiface.h>

static const qreal kBase = 15;
static const qreal kHeight = kBase * 4 / 5;
static const QColor kSelectedBackgroundColor = QColor(Qt::magenta);
static const QColor kDefaultBackgroundColor = QColor(Qt::blue);
static const QList<Qt::Alignment> sides = { Qt::AlignLeft, Qt::AlignTop, Qt::AlignRight, Qt::AlignBottom };

namespace taste3 {
namespace aadl {

AADLInterfaceGraphicsItem::AADLInterfaceGraphicsItem(AADLObjectIface *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_type(new QGraphicsPathItem(this))
    , m_iface(new QGraphicsPathItem(this))
    , m_text(new QGraphicsTextItem(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(QGraphicsItem::ItemIsSelectable);

    updateKind();

    QPainterPath pp;
    pp.addPolygon(QVector<QPointF> { QPointF(-kHeight / 3, -kBase / 2), QPointF(-kHeight / 3, kBase / 2),
                                     QPointF(2 * kHeight / 3, 0) });
    pp.closeSubpath();
    m_iface->setPath(pp);
    m_text->setPlainText(ifaceLabel());

    connect(entity, &AADLObject::attributeChanged, [this](taste3::aadl::meta::Props::Token attr) {
        switch (attr) {
        case taste3::aadl::meta::Props::Token::name:
        case taste3::aadl::meta::Props::Token::labelInheritance: {
            updateLabel();
            break;
        }
        case taste3::aadl::meta::Props::Token::kind:
            updateKind();
            break;
        default:
            break;
        }
    });
    QObject::connect(entity, &AADLObjectIface::titleChanged, this, &AADLInterfaceGraphicsItem::updateLabel);
    if (AADLObjectIfaceRequired *ri = qobject_cast<AADLObjectIfaceRequired *>(entity))
        QObject::connect(ri, &AADLObjectIfaceRequired::inheritedLabelsChanged, this,
                         &AADLInterfaceGraphicsItem::updateLabel);

    colorSchemeUpdated();
}

AADLObjectIface *AADLInterfaceGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectIface *>(dataObject());
}

void AADLInterfaceGraphicsItem::addConnection(AADLConnectionGraphicsItem *item)
{
    if (!item || m_connections.contains(item))
        return;

    m_connections.append(item);
}

void AADLInterfaceGraphicsItem::removeConnection(AADLConnectionGraphicsItem *item)
{
    if (!item)
        return;

    if (m_connections.removeAll(item))
        setFlag(QGraphicsItem::ItemIsSelectable, m_connections.isEmpty());
}

QList<QPointer<AADLConnectionGraphicsItem>> AADLInterfaceGraphicsItem::connectionItems() const
{
    return m_connections;
}

AADLFunctionGraphicsItem *AADLInterfaceGraphicsItem::targetItem() const
{
    return qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem());
}

void AADLInterfaceGraphicsItem::setTargetItem(QGraphicsItem *item, const QPointF &scenePos)
{
    if (!item)
        return;

    setParentItem(item);
    setPos(parentItem()->mapFromScene(scenePos));
    instantLayoutUpdate();
}

void AADLInterfaceGraphicsItem::setInterfaceName(const QString &name)
{
    m_text->setPlainText(name);
}

static inline void moveClockwise(const QRectF &intersectedItemRect, QRectF &rect, int &idx,
                                 const QRectF &parentBoundingRect)
{
    if (!intersectedItemRect.isValid())
        return;

    QRectF clockwiseRect { rect };
    const int index = idx < 0 ? sides.size() - qAbs(idx) % sides.size() : idx % sides.size();
    switch (sides.value(index % sides.size())) {
    case Qt::AlignLeft:
        clockwiseRect.moveBottom(intersectedItemRect.top() - 1);
        break;
    case Qt::AlignRight:
        clockwiseRect.moveTop(intersectedItemRect.bottom() + 1);
        break;
    case Qt::AlignTop:
        clockwiseRect.moveLeft(intersectedItemRect.right() + 1);
        break;
    case Qt::AlignBottom:
        clockwiseRect.moveRight(intersectedItemRect.left() - 1);
        break;
    default:
        return;
    }
    if (!parentBoundingRect.intersects(clockwiseRect))
        moveClockwise(intersectedItemRect, rect, ++idx, parentBoundingRect);
    else
        rect = clockwiseRect;
};

static inline void moveCounterClockwise(const QRectF &intersectedItemRect, QRectF &rect, int &idx,
                                        const QRectF &parentBoundingRect)
{
    if (!intersectedItemRect.isValid())
        return;

    QRectF counterClockwiseRect { rect };
    const int index = idx < 0 ? sides.size() - (qAbs(idx) % sides.size()) : idx % sides.size();
    switch (sides.value(index)) {
    case Qt::AlignLeft:
        counterClockwiseRect.moveTop(intersectedItemRect.bottom() + 1);
        break;
    case Qt::AlignRight:
        counterClockwiseRect.moveBottom(intersectedItemRect.top() - 1);
        break;
    case Qt::AlignTop:
        counterClockwiseRect.moveRight(intersectedItemRect.left() - 1);
        break;
    case Qt::AlignBottom:
        counterClockwiseRect.moveLeft(intersectedItemRect.right() + 1);
        break;
    default:
        return;
    }
    if (!parentBoundingRect.intersects(counterClockwiseRect))
        moveCounterClockwise(intersectedItemRect, rect, --idx, parentBoundingRect);
    else
        rect = counterClockwiseRect;
};

void AADLInterfaceGraphicsItem::rebuildLayout()
{
    prepareGeometryChange();
    if (!parentItem()) {
        m_boundingRect = QRectF();
        return;
    }

    auto updateInternalItems = [this](Qt::Alignment alignment) {
        const bool insideOut = entity()->direction() == AADLObjectIface::IfaceType::Required;
        const qreal offset = kBase + 2;
        switch (alignment) {
        case Qt::AlignLeft:
            m_iface->setRotation(insideOut ? 180 : 0);
            m_type->setPos(QPointF(-offset, 0));
            break;
        case Qt::AlignRight:
            m_iface->setRotation(insideOut ? 0 : 180);
            m_type->setPos(QPointF(offset, 0));
            break;
        case Qt::AlignTop:
            m_iface->setRotation(insideOut ? 270 : 90);
            m_type->setPos(QPointF(0, -offset));
            break;
        case Qt::AlignBottom:
            m_iface->setRotation(insideOut ? 90 : 270);
            m_type->setPos(QPointF(0, offset));
            break;
        }
        return mapRectFromItem(m_iface, m_iface->boundingRect());
    };

    const QPointF ifacePos = pos();
    if (utils::pos(entity()->coordinates()).isNull()) {
        if (auto connection = m_connections.value(0)) {
            auto theirIface = connection->startItem() == this ? connection->endItem() : connection->startItem();
            if (auto theirItem = theirIface->targetItem()) {
                const QRectF theirRect = theirItem->sceneBoundingRect();
                const QRectF ourRect = targetItem()->sceneBoundingRect();
                const QPointF theirPos = utils::pos(theirIface->entity()->coordinates());
                const QPointF ourPos = ourRect.center();
                Qt::Alignment side = Qt::AlignAbsolute;
                QPointF sidePos;
                if (theirItem->isRootItem()) {
                    if (theirPos.isNull()) {
                        side = utils::getNearestSide(theirRect, ourPos);
                        sidePos = utils::getSidePosition(theirRect, ourPos, side);
                    } else {
                        side = utils::getNearestSide(ourRect, theirPos);
                        sidePos = utils::getSidePosition(ourRect, ourPos, side);
                    }
                } else if (targetItem()->isRootItem()) {
                    side = utils::getNearestSide(ourRect, theirRect.center());
                    sidePos = utils::getSidePosition(ourRect, theirRect.center(), side);
                } else if (theirRect.isValid()) {
                    if (theirPos.isNull())
                        side = utils::getNearestSide(ourRect, theirRect.center());
                    else
                        side = utils::getNearestSide(ourRect, theirPos);
                    sidePos = utils::getSidePosition(ourRect, ourRect.center(), side);
                }
                if (side != Qt::AlignAbsolute) {
                    updateInternalItems(side);
                    entity()->setCoordinates(utils::coordinates(sidePos));
                }
            }
            return;
        } else {
            /// NOTE: iface items without connections are put close to top left corner
            /// because of null pos
        }
    }

    const QRectF parentRect = parentItem()->boundingRect();

    Qt::Alignment alignment = utils::getNearestSide(parentRect, ifacePos);
    const int initialAlignment = sides.indexOf(alignment);

    QPointF stickyPos = utils::getSidePosition(parentRect, ifacePos, alignment);

    QRectF br = updateInternalItems(alignment);
    br.moveTo(stickyPos + br.topLeft());

    auto checkCollision = [this](const QRectF &parentRect, const QRectF &rect, QRectF &intersectedRect,
                                 Qt::Alignment &alignment, QPointF &stickyPos) {
        auto isInterfaceItemLambda = [this](const QGraphicsItem *item) {
            return item->type() == aadl::AADLInterfaceGraphicsItem::Type && parentItem() == item->parentItem()
                    && item != this;
        };

        const QList<QGraphicsItem *> collidedItems = scene()->items(parentItem()->mapRectToScene(rect));
        auto collidedIt = std::find_if(collidedItems.constBegin(), collidedItems.constEnd(), isInterfaceItemLambda);
        if (collidedIt == collidedItems.constEnd()) {
            alignment = utils::getNearestSide(parentRect, rect.center());
            stickyPos = utils::getSidePosition(parentRect, rect.center(), alignment);
            return false;
        }
        intersectedRect = parentItem()->mapRectFromScene((*collidedIt)->sceneBoundingRect());
        return true;
    };

    QRectF intersectedRect;
    if (checkCollision(parentRect, br, intersectedRect, alignment, stickyPos)) {
        QRectF cIntersectedRect { intersectedRect };
        QRectF ccIntersectedRect { intersectedRect };
        QRectF cbr { br };
        QRectF ccbr { br };
        int cAlignmentIdx { initialAlignment };
        int ccAlignmentIdx { initialAlignment };

        while ((ccAlignmentIdx / sides.size() == 0 || ccAlignmentIdx % sides.size() != initialAlignment)
               && (cAlignmentIdx / sides.size() == 0 || cAlignmentIdx % sides.size() != initialAlignment)) {
            moveClockwise(cIntersectedRect, cbr, cAlignmentIdx, parentRect);
            if (!checkCollision(parentRect, cbr, cIntersectedRect, alignment, stickyPos))
                break;

            moveCounterClockwise(ccIntersectedRect, ccbr, ccAlignmentIdx, parentRect);
            if (!checkCollision(parentRect, ccbr, ccIntersectedRect, alignment, stickyPos))
                break;
        }
    }

    m_boundingRect = updateInternalItems(alignment);

    m_shape = QPainterPath();
    m_shape.addPath(m_type->shape());
    m_shape.addPath(m_iface->shape());
    m_shape.addPath(m_text->shape());

    /// TODO:
    if (flags() & QGraphicsItem::ItemIsSelectable)
        m_boundingRect = childrenBoundingRect();

    setPos(stickyPos);
}

QPainterPath AADLInterfaceGraphicsItem::shape() const
{
    if (m_shape.isEmpty())
        return InteractiveObject::shape();

    return m_shape;
}

void AADLInterfaceGraphicsItem::updateFromEntity()
{
    aadl::AADLObjectIface *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    setInterfaceName(ifaceLabel());

    const QPointF coordinates = utils::pos(obj->coordinates());
    if (coordinates.isNull())
        instantLayoutUpdate();
    else
        setTargetItem(parentItem(), coordinates);
}

void AADLInterfaceGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    m_gripPoints->setUsedPoints({});
}

void AADLInterfaceGraphicsItem::onSelectionChanged(bool isSelected)
{
    const ColorHandler &h = colorHandler();
    m_iface->setBrush(isSelected ? kSelectedBackgroundColor : h.brush());
}

void AADLInterfaceGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void AADLInterfaceGraphicsItem::onManualMoveProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from)

    if (!scene() || grip != GripPoint::Location::Center || m_clickPos.isNull() || !m_connections.isEmpty())
        return;

    QPointF newPos = mapToParent(mapFromScene(to) - m_clickPos);
    if (parentItem()) {
        const QRectF contentRect = parentItem()->boundingRect();

        if (newPos.x() < contentRect.left())
            newPos.setX(contentRect.left());
        else if ((newPos.x()) > contentRect.right())
            newPos.setX(contentRect.right());

        if (newPos.y() < contentRect.top())
            newPos.setY(contentRect.top());
        else if ((newPos.y()) > contentRect.bottom())
            newPos.setY(contentRect.bottom());
    }
    setPos(newPos);

    rebuildLayout();
    updateGripPoints();

    Q_EMIT needUpdateLayout();
}

ColorManager::HandledColors AADLInterfaceGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Iface;
}

AADLObject *AADLInterfaceGraphicsItem::aadlObject() const
{
    return entity();
}

void AADLInterfaceGraphicsItem::colorSchemeUpdated()
{
    const ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    m_type->setPen(pen);
    pen.setWidthF(pen.widthF() / 2.);
    m_iface->setPen(pen);
    m_iface->setBrush(h.brush());
    update();
}

void AADLInterfaceGraphicsItem::updateLabel()
{
    const QString &label = ifaceLabel();
    if (label != m_text->toPlainText()) {
        m_text->setPlainText(label);
        instantLayoutUpdate();
    }
}

void AADLInterfaceGraphicsItem::updateKind()
{
    AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(aadlObject());
    if (!iface)
        return;

    QPainterPath kindPath;
    switch (iface->kind()) {
    case AADLObjectIface::OperationKind::Cyclic: {
        const qreal kindBaseValue = kHeight;
        kindPath.arcTo({ kindPath.currentPosition().x() - kindBaseValue / 2,
                         kindPath.currentPosition().y() - kindBaseValue, kindBaseValue, kindBaseValue },
                       -90, -270);
        kindPath.lineTo(kindPath.currentPosition() + QPointF(0, kindBaseValue / 3));
        kindPath.addPolygon(
                QVector<QPointF> { kindPath.currentPosition() + QPointF(-kindBaseValue / 3, -kindBaseValue / 3),
                                   kindPath.currentPosition(),
                                   kindPath.currentPosition() + QPointF(kindBaseValue / 3, -kindBaseValue / 3) });
        kindPath.translate(0, kindBaseValue / 2);
        break;
    }
    case AADLObjectIface::OperationKind::Sporadic: {
        const qreal kindBaseValue = kHeight;
        kindPath.moveTo(-kindBaseValue / 2, 0);
        kindPath.lineTo(0, -kindBaseValue / 4);
        kindPath.lineTo(0, kindBaseValue / 4);
        kindPath.lineTo(kindBaseValue / 2, 0);
        break;
    }
    case AADLObjectIface::OperationKind::Protetcted: {
        const qreal kindBaseValue = kHeight;
        const QRectF rect { -kindBaseValue / 2, -kindBaseValue / 2, kindBaseValue, kindBaseValue * 2 / 3 };
        kindPath.addRoundedRect(rect, 2, 2);
        QRectF arcRect(rect.adjusted(rect.width() / 5, 0, -rect.width() / 5, 0));
        arcRect.moveCenter(QPointF(rect.center().x(), rect.top()));
        kindPath.moveTo(arcRect.center());
        kindPath.arcTo(arcRect, 0, 180);
        kindPath.translate(0, rect.height() / 3);
    }
    default:
        break;
    }
    m_type->setPath(kindPath);
}

QString AADLInterfaceGraphicsItem::ifaceLabel() const
{
    if (AADLObjectIfaceRequired *ri = qobject_cast<AADLObjectIfaceRequired *>(entity())) {
        if (ri->inheritPi()) {
            const QStringList &labels = ri->inheritedLables();
            if (labels.size())
                return labels.join(", ");
        }
    }
    return entity()->title();
}

} // namespace aadl
} // namespace taste3
