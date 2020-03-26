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
#include "commands/cmdentitygeometrychange.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QPainter>
#include <QtDebug>
#include <app/commandsstack.h>
#include <baseitems/common/utils.h>
#include <baseitems/grippointshandler.h>
#include <tab_aadl/aadlobjectconnection.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectiface.h>

static const qreal kBase = 15;
static const qreal kHeight = kBase * 4 / 5;
static const QColor kSelectedBackgroundColor = QColor(Qt::magenta);
static const QColor kDefaultBackgroundColor = QColor(Qt::blue);
static const QList<Qt::Alignment> sides = { Qt::AlignLeft, Qt::AlignTop, Qt::AlignRight, Qt::AlignBottom };
static const qreal kInterfaceTitleMaxLength = 80;

namespace taste3 {
namespace aadl {

AADLInterfaceGraphicsItem::AADLInterfaceGraphicsItem(AADLObjectIface *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_type(new QGraphicsPathItem(this))
    , m_iface(new QGraphicsPathItem(this))
    , m_text(new QGraphicsTextItem(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(utils::kInterfaceZLevel);
    updateKind();

    QPainterPath pp;
    pp.addPolygon(QVector<QPointF> { QPointF(-kHeight / 3, -kBase / 2), QPointF(-kHeight / 3, kBase / 2),
                                     QPointF(2 * kHeight / 3, 0) });
    pp.closeSubpath();
    m_iface->setPath(pp);
    //    setInterfaceName(ifaceLabel());

    connect(entity, &AADLObject::attributeChanged, this, &AADLInterfaceGraphicsItem::onAttrOrPropChanged);
    connect(entity, &AADLObjectIface::titleChanged, this, &AADLInterfaceGraphicsItem::updateLabel);
    if (AADLObjectIfaceRequired *ri = qobject_cast<AADLObjectIfaceRequired *>(entity))
        connect(ri, &AADLObjectIfaceRequired::inheritedLabelsChanged, this, &AADLInterfaceGraphicsItem::updateLabel);

    colorSchemeUpdated();
}

AADLObjectIface *AADLInterfaceGraphicsItem::entity() const
{
    return qobject_cast<aadl::AADLObjectIface *>(aadlObject());
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
    if (name != m_text->toPlainText()) {
        const QFontMetrics fm(m_text->font());
        m_text->setPlainText(fm.elidedText(name, Qt::ElideRight, kInterfaceTitleMaxLength));
        instantLayoutUpdate();
    }
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
        counterClockwiseRect.moveTop(intersectedItemRect.bottom() + kBase);
        break;
    case Qt::AlignRight:
        counterClockwiseRect.moveBottom(intersectedItemRect.top() - kBase);
        break;
    case Qt::AlignTop:
        counterClockwiseRect.moveRight(intersectedItemRect.left() - kBase);
        break;
    case Qt::AlignBottom:
        counterClockwiseRect.moveLeft(intersectedItemRect.right() + kBase);
        break;
    default:
        return;
    }
    if (!parentBoundingRect.intersects(counterClockwiseRect))
        moveCounterClockwise(intersectedItemRect, rect, --idx, parentBoundingRect);
    else
        rect = counterClockwiseRect;
};

QRectF AADLInterfaceGraphicsItem::updateInternalItems(Qt::Alignment alignment)
{
    const bool insideOut = entity()->direction() == AADLObjectIface::IfaceType::Required;
    const qreal offset = kBase + 2;

    qreal rotationDegree = 0.;
    QPointF shift(0., 0.);
    switch (alignment) {
    case Qt::AlignLeft:
        rotationDegree = insideOut ? 180 : 0;
        shift.setX(-offset);
        break;
    case Qt::AlignRight:
        rotationDegree = insideOut ? 0 : 180;
        shift.setX(offset);
        break;
    case Qt::AlignTop:
        rotationDegree = insideOut ? 270 : 90;
        shift.setY(-offset);
        break;
    case Qt::AlignBottom:
        rotationDegree = insideOut ? 90 : 270;
        shift.setY(offset);
        break;
    }

    m_iface->setTransform(QTransform().rotate(rotationDegree));
    m_type->setPos(shift);
    m_shape = composeShape();

    return mapRectFromItem(m_iface, m_iface->boundingRect());
};

void AADLInterfaceGraphicsItem::rebuildLayout()
{
    prepareGeometryChange();
    if (!parentItem()) {
        m_boundingRect = QRectF();
        return;
    }

    const QPointF ifacePos = pos();
    if (utils::pos(entity()->coordinates()).isNull()) {
        layout();
        return;
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
    m_shape = composeShape();

    setPos(stickyPos);
}

QPainterPath AADLInterfaceGraphicsItem::shape() const
{
    if (m_shape.isEmpty())
        return composeShape();

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

void AADLInterfaceGraphicsItem::onSelectionChanged(bool isSelected)
{
    const ColorHandler &h = colorHandler();
    m_iface->setBrush(isSelected ? kSelectedBackgroundColor : h.brush());
}

QList<QVariantList> AADLInterfaceGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QList<QVariantList> params = { { QVariant::fromValue(entity()),
                                     QVariant::fromValue(QVector<QPointF> { scenePos() }) } };
    for (auto connection : connectionItems()) {
        if (connection) {
            params.append(
                    { QVariant::fromValue(connection->entity()),
                      QVariant::fromValue(
                              connection->graphicsPoints()) }); // connection->prepareChangeCoordinatesCommandParams()
                                                                // - will be fixed during work on Undo/Redo issues
        }
    }

    return params;
}

void AADLInterfaceGraphicsItem::layout()
{
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
                //                    entity()->setCoordinates(utils::coordinates(sidePos));
                setPos(parentItem()->mapFromScene(sidePos));
                mergeGeometry();
            }
        }
    } else {
        /// NOTE: iface items without connections are put close to top left corner
        /// because of null pos
    }
}

void AADLInterfaceGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void AADLInterfaceGraphicsItem::onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(grip)
    Q_UNUSED(from)

    if (!scene() || !m_connections.isEmpty())
        return;

    const QPointF shift = { to - from };
    if (shift.isNull())
        return;

    QPointF newPos = scenePos() + shift;
    if (parentItem()) {
        const QRectF contentRect = parentItem()->sceneBoundingRect();

        newPos.setX(qBound(contentRect.left(), newPos.x(), contentRect.right()));
        newPos.setY(qBound(contentRect.top(), newPos.y(), contentRect.bottom()));
    }
    setPos(mapToParent(mapFromScene(newPos)));

    rebuildLayout();
    updateGripPoints();

    Q_EMIT needUpdateLayout();
}

ColorManager::HandledColors AADLInterfaceGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Iface;
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
    setInterfaceName(ifaceLabel());
}

void AADLInterfaceGraphicsItem::updateKind()
{
    AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(entity());
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
    case AADLObjectIface::OperationKind::Protected: {
        const qreal kindBaseValue = kHeight;
        const QRectF rect { -kindBaseValue / 2, -kindBaseValue / 2, kindBaseValue, kindBaseValue * 2 / 3 };
        kindPath.addRoundedRect(rect, 2, 2);
        QRectF arcRect(rect.adjusted(rect.width() / 5, 0, -rect.width() / 5, 0));
        arcRect.moveCenter(QPointF(rect.center().x(), rect.top()));
        kindPath.moveTo(arcRect.center());
        kindPath.arcTo(arcRect, 0, 180);
        kindPath.translate(0, rect.height() / 3);
        break;
    }
    default:
        break;
    }
    m_type->setPath(kindPath);
    m_shape = composeShape();
}

QString AADLInterfaceGraphicsItem::ifaceLabel() const
{
    if (entity()->isRequired())
        if (AADLObjectIfaceRequired *ri = qobject_cast<AADLObjectIfaceRequired *>(entity())) {
            if (ri->isInheritPI()) {
                const QStringList &labels = ri->inheritedLables();
                if (labels.size())
                    return labels.join(", ");
            }
        }
    return entity()->title();
}

QString AADLInterfaceGraphicsItem::prepareTooltip() const
{
    QString toolTip = InteractiveObject::prepareTooltip();
    if (entity()->isProvided())
        return toolTip;

    const AADLObjectIfaceRequired *ri = entity()->as<const AADLObjectIfaceRequired *>();
    if (!ri)
        return toolTip;

    const QString label = ifaceLabel();
    if (toolTip != label)
        toolTip = QString("%1<br><i><b>%2</b></i>").arg(label, toolTip);

    return toolTip;
}

void AADLInterfaceGraphicsItem::onAttrOrPropChanged(taste3::aadl::meta::Props::Token t)
{
    switch (t) {
        //    case taste3::aadl::meta::Props::Token::name: // handled in AADLInterfaceGraphicsItem::updateLabel
    case taste3::aadl::meta::Props::Token::InheritPI: {
#ifdef QT_DEBUG
        qWarning() << "check for a call duplication" << t;
#endif
        updateLabel();
        break;
    }
    case taste3::aadl::meta::Props::Token::kind:
        updateKind();
        break;
    default:
        break;
    }
}

QPainterPath AADLInterfaceGraphicsItem::composeShape() const
{
    QPainterPath path = m_text->shape();
    for (auto sub : { m_type, m_iface }) {
        QPainterPath subPath = sub->transform().map(sub->path()).simplified();
        subPath.translate(sub->pos());
        path.addPath(subPath);
    }
    return path;
}

} // namespace aadl
} // namespace taste3
