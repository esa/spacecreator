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
#include "commandsstack.h"
#include "colors/colormanager.h"
#include "commands/cmdentitygeometrychange.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QPainter>
#include <QtDebug>
#include <baseitems/common/utils.h>
#include <baseitems/grippointshandler.h>
#include <aadlobjectconnection.h>
#include <aadlobjectfunction.h>
#include <aadlobjectiface.h>

static const qreal kBase = 15;
static const qreal kHeight = kBase * 4 / 5;
static const QColor kSelectedBackgroundColor = QColor(Qt::magenta);
static const QColor kDefaultBackgroundColor = QColor(Qt::blue);
static const QList<Qt::Alignment> kRectSides = { Qt::AlignLeft, Qt::AlignTop, Qt::AlignRight, Qt::AlignBottom };
static const qreal kInterfaceTitleMaxLength = 80;

namespace aadlinterface {

AADLInterfaceGraphicsItem::AADLInterfaceGraphicsItem(aadl::AADLObjectIface *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_type(new QGraphicsPathItem(this))
    , m_iface(new QGraphicsPathItem(this))
    , m_text(new QGraphicsTextItem(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(kInterfaceZLevel);
    updateKind();

    QPainterPath pp;
    pp.addPolygon(QVector<QPointF> { QPointF(-kHeight / 3, -kBase / 2), QPointF(-kHeight / 3, kBase / 2),
                                     QPointF(2 * kHeight / 3, 0) });
    pp.closeSubpath();
    m_iface->setPath(pp);
    //    setInterfaceName(ifaceLabel());

    connect(entity, &aadl::AADLObject::attributeChanged, this, &AADLInterfaceGraphicsItem::onAttrOrPropChanged);
    connect(entity, &aadl::AADLObjectIface::titleChanged, this, &AADLInterfaceGraphicsItem::updateLabel);
    if (auto ri = qobject_cast<aadl::AADLObjectIfaceRequired *>(entity))
        connect(ri, &aadl::AADLObjectIfaceRequired::inheritedLabelsChanged, this, &AADLInterfaceGraphicsItem::updateLabel);

    applyColorScheme();
}

aadl::AADLObjectIface *AADLInterfaceGraphicsItem::entity() const
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

    m_connections.removeAll(item);
}

QList<QPointer<AADLConnectionGraphicsItem>> AADLInterfaceGraphicsItem::connectionItems() const
{
    return m_connections;
}

AADLFunctionGraphicsItem *AADLInterfaceGraphicsItem::targetItem() const
{
    return qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem());
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

static inline void moveIface(const QRectF &intersectedItemRect, QRectF &rect, int &idx,
                             const QRectF &parentBoundingRect, const QPointF &offset, bool invert)
{
    if (!intersectedItemRect.isValid())
        return;

    QRectF itemRect { rect };
    const int currentIdx = invert ? idx + 2 : idx;
    const int index =
            idx < 0 ? kRectSides.size() - qAbs(currentIdx) % kRectSides.size() : currentIdx % kRectSides.size();
    switch (kRectSides.value(index % kRectSides.size())) {
    case Qt::AlignLeft:
        itemRect.moveBottom(intersectedItemRect.top() - kBase);
        break;
    case Qt::AlignRight:
        itemRect.moveTop(intersectedItemRect.bottom() + kBase);
        break;
    case Qt::AlignTop:
        itemRect.moveLeft(intersectedItemRect.right() + kBase);
        break;
    case Qt::AlignBottom:
        itemRect.moveRight(intersectedItemRect.left() - kBase);
        break;
    default:
        return;
    }
    if (!parentBoundingRect.contains(itemRect.topLeft() - offset))
        moveIface(intersectedItemRect, rect, invert ? ++idx : --idx, parentBoundingRect, offset, invert);
    else
        rect = itemRect;
}

void AADLInterfaceGraphicsItem::updateInternalItems(Qt::Alignment alignment)
{
    prepareGeometryChange();

    const bool insideOut = entity()->direction() == aadl::AADLObjectIface::IfaceType::Required;
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
    m_boundingRect = childrenBoundingRect();
}

void AADLInterfaceGraphicsItem::doRebuildLayout()
{
    if (!parentItem()) {
        prepareGeometryChange();
        m_boundingRect = QRectF();
        return;
    }

    const QPointF ifacePos = pos();
    if (aadlinterface::pos(entity()->coordinates()).isNull()) {
        layout();
        return;
    }

    const QRectF parentRect = parentItem()->boundingRect();
    const Qt::Alignment alignment = getNearestSide(parentRect, ifacePos);

    updateInternalItems(alignment);
    const QPointF stickyPos = getSidePosition(parentRect, ifacePos, alignment);
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
    const aadl::AADLObjectIface *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const QPointF coordinates = aadlinterface::pos(obj->coordinates());
    if (coordinates.isNull())
        instantLayoutUpdate();
    else
        setTargetItem(parentItem(), coordinates);
    adjustItem();
    setInterfaceName(ifaceLabel());
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
            const QPointF theirPos = aadlinterface::pos(theirIface->entity()->coordinates());
            const QPointF ourPos = ourRect.center();
            Qt::Alignment side = Qt::AlignAbsolute;
            QPointF sidePos;
            if (theirItem->isRootItem()) {
                if (theirPos.isNull()) {
                    side = getNearestSide(theirRect, ourPos);
                    sidePos = getSidePosition(theirRect, ourPos, side);
                } else {
                    side = getNearestSide(ourRect, theirPos);
                    sidePos = getSidePosition(ourRect, ourPos, side);
                }
            } else if (targetItem()->isRootItem()) {
                side = getNearestSide(ourRect, theirRect.center());
                sidePos = getSidePosition(ourRect, theirRect.center(), side);
            } else if (theirRect.isValid()) {
                if (theirPos.isNull())
                    side = getNearestSide(ourRect, theirRect.center());
                else
                    side = getNearestSide(ourRect, theirPos);
                sidePos = getSidePosition(ourRect, ourRect.center(), side);
            }
            if (side != Qt::AlignAbsolute) {
                updateInternalItems(side);
                setPos(parentItem()->mapFromScene(sidePos));
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

void AADLInterfaceGraphicsItem::onManualMoveFinish(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(grip)

    const QPointF shift = { to - from };
    if (shift.isNull())
        return;

    updateEntity();
}

ColorManager::HandledColors AADLInterfaceGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Iface;
}

void AADLInterfaceGraphicsItem::adjustItem()
{
    QList<QRectF> siblingsRects;
    const QList<QGraphicsItem *> siblingItems = parentItem()->childItems();
    std::for_each(siblingItems.cbegin(), siblingItems.cend(), [this, &siblingsRects](const QGraphicsItem *sibling) {
        if (sibling->type() == AADLInterfaceGraphicsItem::Type && sibling != this)
            siblingsRects.append(sibling->mapRectToParent(sibling->boundingRect()));
    });

    auto checkCollision = [](const QList<QRectF> &itemRects, const QRectF &itemRect, QRectF &collidingRect) {
        auto it = std::find_if(itemRects.cbegin(), itemRects.cend(),
                               [itemRect](const QRectF &siblibgRect) { return siblibgRect.intersects(itemRect); });
        if (it != itemRects.cend()) {
            collidingRect = *it;
            return true;
        }
        return false;
    };
    const QRectF parentRect = parentItem()->boundingRect();
    const Qt::Alignment alignment = getNearestSide(parentRect, pos());
    const int initialAlignment = kRectSides.indexOf(alignment);
    const QPointF offset = m_boundingRect.topLeft();
    const QRectF itemRect = mapRectToParent(m_boundingRect);

    QRectF intersectedRect;
    if (checkCollision(siblingsRects, itemRect, intersectedRect) && parentRect.isValid()) {
        QRectF cIntersectedRect { intersectedRect };
        QRectF ccIntersectedRect { intersectedRect };
        QRectF br { itemRect };
        QRectF cbr { itemRect };
        QRectF ccbr { itemRect };
        int alignmentIdx { initialAlignment };
        int cAlignmentIdx { initialAlignment };
        int ccAlignmentIdx { initialAlignment };

        while (qAbs(cAlignmentIdx - initialAlignment) < kRectSides.size()
               && qAbs(ccAlignmentIdx - initialAlignment) < kRectSides.size()) {
            moveIface(cIntersectedRect, cbr, cAlignmentIdx, parentRect, offset, false);
            if (!checkCollision(siblingsRects, cbr, cIntersectedRect)) {
                alignmentIdx = cAlignmentIdx;
                br = cbr;
                break;
            }

            moveIface(ccIntersectedRect, ccbr, ccAlignmentIdx, parentRect, offset, true);
            if (!checkCollision(siblingsRects, ccbr, ccIntersectedRect)) {
                alignmentIdx = ccAlignmentIdx;
                br = ccbr;
                break;
            }
        }
        if (alignmentIdx != initialAlignment)
            updateInternalItems(kRectSides.value(alignmentIdx));
        setPos(br.topLeft() - offset);
    }
}

void AADLInterfaceGraphicsItem::applyColorScheme()
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
    auto iface = qobject_cast<aadl::AADLObjectIface *>(entity());
    if (!iface)
        return;

    QPainterPath kindPath;
    switch (iface->kind()) {
    case aadl::AADLObjectIface::OperationKind::Cyclic: {
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
    case aadl::AADLObjectIface::OperationKind::Sporadic: {
        const qreal kindBaseValue = kHeight;
        kindPath.moveTo(-kindBaseValue / 2, 0);
        kindPath.lineTo(0, -kindBaseValue / 4);
        kindPath.lineTo(0, kindBaseValue / 4);
        kindPath.lineTo(kindBaseValue / 2, 0);
        break;
    }
    case aadl::AADLObjectIface::OperationKind::Protected: {
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
        if (auto ri = qobject_cast<aadl::AADLObjectIfaceRequired *>(entity())) {
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

    auto ri = entity()->as<const aadl::AADLObjectIfaceRequired *>();
    if (!ri)
        return toolTip;

    const QString label = ifaceLabel();
    if (toolTip != label)
        toolTip = QString("%1<br><i><b>%2</b></i>").arg(label, toolTip);

    return toolTip;
}

void AADLInterfaceGraphicsItem::onAttrOrPropChanged(aadl::meta::Props::Token t)
{
    switch (t) {
        //    case taste3::aadl::meta::Props::Token::name: // handled in AADLInterfaceGraphicsItem::updateLabel
    case aadl::meta::Props::Token::InheritPI: {
#ifdef QT_DEBUG
        qWarning() << "check for a call duplication" << t;
#endif
        updateLabel();
        break;
    }
    case aadl::meta::Props::Token::kind:
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

QVariant AADLInterfaceGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemVisibleHasChanged: {
        for (auto connection : m_connections)
            connection->setVisible(connection->startItem()->isVisible() && connection->endItem()->isVisible());
        break;
    }
    default:
        break;
    }
    return InteractiveObject::itemChange(change, value);
}

}
