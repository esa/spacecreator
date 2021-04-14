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

#include "aadlcommentgraphicsitem.h"
#include "ivconnection.h"
#include "aadlconnectiongraphicsitem.h"
#include "ivfunction.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctiontypegraphicsitem.h"
#include "ivinterface.h"
#include "ivnamevalidator.h"
#include "baseitems/common/ivutils.h"
#include "baseitems/common/positionlookuphelper.h"
#include "colors/colormanager.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

static const qreal kBase = 12;
static const qreal kHeight = kBase * 4 / 5;
static const QColor kSelectedBackgroundColor = QColor(Qt::magenta);
static const QColor kDefaultBackgroundColor = QColor(Qt::blue);
static const int kInterfaceMinDistance = 20;
static const int kTextMargin = 2;

namespace ive {

AADLInterfaceGraphicsItem::AADLInterfaceGraphicsItem(ivm::IVInterface *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_type(new QGraphicsPathItem(this))
    , m_iface(new QGraphicsPathItem(this))
    , m_text(new QGraphicsTextItem(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(ZOrder.Interface);
}

ivm::IVInterface *AADLInterfaceGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVInterface *>(m_dataObject);
}

void AADLInterfaceGraphicsItem::init()
{
    InteractiveObject::init();
    connect(entity(), qOverload<ivm::meta::Props::Token>(&ivm::IVObject::attributeChanged), this,
            &AADLInterfaceGraphicsItem::onAttrOrPropChanged);
    connect(entity(), &ivm::IVInterface::titleChanged, this, &AADLInterfaceGraphicsItem::updateLabel);
    if (auto ri = qobject_cast<ivm::IVInterfaceRequired *>(entity()))
        connect(ri, &ivm::IVInterfaceRequired::inheritedLabelsChanged, this, &AADLInterfaceGraphicsItem::updateLabel);
    connect(this, &AADLInterfaceGraphicsItem::needUpdateLayout, this, [this]() {
        if (auto function = targetItem()) {
            function->update();
        }
    });

    updateLabel();
    updateIface();
    updateKind();
    setInterfaceName(ifaceLabel());
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

AADLFunctionTypeGraphicsItem *AADLInterfaceGraphicsItem::targetItem() const
{
    return parentItem() ? qobject_cast<AADLFunctionTypeGraphicsItem *>(parentItem()->toGraphicsObject()) : nullptr;
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
    QString text = ivm::IVNameValidator::decodeName(entity()->type(), name);

    int maxTextWidth = maxWidth();
    if (maxTextWidth > 0) {
        // text is always at 0, interface item might (probably is) negative
        maxTextWidth = maxTextWidth + (boundingRect().x() - kTextMargin);
        const QFontMetrics fm(m_text->font());
        text = fm.elidedText(text, Qt::ElideRight, maxTextWidth);
    }

    if (text != m_text->toPlainText()) {
        m_text->setPlainText(text);
        instantLayoutUpdate();
    }
}

QPointF AADLInterfaceGraphicsItem::connectionEndPoint(const bool nestedConnection) const
{
    const qreal borderWidth = m_iface->pen().widthF() + 1;
    const QRectF ifaceRect =
            m_iface->sceneBoundingRect().adjusted(borderWidth / 2, borderWidth / 2, -borderWidth / 2, -borderWidth / 2);
    if (!ifaceRect.isValid()) {
        return {};
    }
    if (auto parentGraphicsItem = parentItem()) {
        const QRectF parentRect = parentGraphicsItem->boundingRect();
        const Qt::Alignment alignment = getNearestSide(parentRect, pos());
        switch (alignment) {
        case Qt::AlignLeft:
            if (nestedConnection) {
                return { ifaceRect.right(), QLineF(ifaceRect.topRight(), ifaceRect.bottomRight()).center().y() };
            } else {
                return { ifaceRect.left(), QLineF(ifaceRect.topLeft(), ifaceRect.bottomLeft()).center().y() };
            }
        case Qt::AlignTop:
            if (nestedConnection) {
                return { QLineF(ifaceRect.bottomLeft(), ifaceRect.bottomRight()).center().x(), ifaceRect.bottom() };
            } else {
                return { QLineF(ifaceRect.topLeft(), ifaceRect.topRight()).center().x(), ifaceRect.top() };
            }
        case Qt::AlignRight:
            if (nestedConnection) {
                return { ifaceRect.left(), QLineF(ifaceRect.topLeft(), ifaceRect.bottomLeft()).center().y() };
            } else {
                return { ifaceRect.right(), QLineF(ifaceRect.topRight(), ifaceRect.bottomRight()).center().y() };
            }
        case Qt::AlignBottom:
            if (nestedConnection) {
                return { QLineF(ifaceRect.topLeft(), ifaceRect.topRight()).center().x(), ifaceRect.top() };
            } else {
                return { QLineF(ifaceRect.bottomLeft(), ifaceRect.bottomRight()).center().x(), ifaceRect.bottom() };
            }
        default:
            return {};
        }
    }
    return {};
}

QPointF AADLInterfaceGraphicsItem::connectionEndPoint(AADLConnectionGraphicsItem *connection) const
{
    if (connection) {
        const bool innerConnection = connection->entity()->parentObject() == entity()->parentObject();
        return connectionEndPoint(innerConnection);
    }
    return m_iface->sceneBoundingRect().center();
}

QPainterPath AADLInterfaceGraphicsItem::ifaceShape() const
{
    const QRectF parentRect = parentItem()->boundingRect();
    const Qt::Alignment alignment = getNearestSide(parentRect, pos());
    return mapToScene(ifaceTransform(alignment).map(ifacePath()));
}

void AADLInterfaceGraphicsItem::updateInternalItems(Qt::Alignment alignment)
{
    prepareGeometryChange();

    m_iface->setTransform(ifaceTransform(alignment));
    m_type->setTransform(typeTransform(alignment));
    m_shape = composeShape();
    setBoundingRect(childrenBoundingRect());
}

void AADLInterfaceGraphicsItem::rebuildLayout()
{
    InteractiveObject::rebuildLayout();

    if (!targetItem()) {
        prepareGeometryChange();
        setBoundingRect(QRectF());
        return;
    }

    const QPointF ifacePos = pos();
    const QRectF parentRect = targetItem()->boundingRect();
    const Qt::Alignment alignment = getNearestSide(parentRect, ifacePos);
    updateInternalItems(alignment);
    if (entity() && ive::pos(entity()->coordinates()).isNull()) {
        layout();
        return;
    }

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
    const ivm::IVInterface *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    setInterfaceName(ifaceLabel());
    const QPointF coordinates = ive::pos(obj->coordinates());
    if (coordinates.isNull())
        instantLayoutUpdate();
    else
        setTargetItem(parentItem(), coordinates);
    adjustItem();
}

void AADLInterfaceGraphicsItem::onSelectionChanged(bool isSelected)
{
    const shared::ColorHandler &h = colorHandler();
    m_iface->setBrush(isSelected ? kSelectedBackgroundColor : h.brush());
}

QList<QPair<ivm::IVObject *, QVector<QPointF>>>
AADLInterfaceGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QVector<QPointF> pos;
    pos.append(scenePos());
    QList<QPair<ivm::IVObject *, QVector<QPointF>>> params = { { entity(), pos } };
    for (const auto &connection : connectionItems()) {
        if (connection) {
            params.append({ connection->entity(),
                    connection->graphicsPoints() }); // connection->prepareChangeCoordinatesCommandParams()
                                                     // - will be fixed during work on Undo/Redo issues
        }
    }

    return params;
}

void AADLInterfaceGraphicsItem::layout()
{
    static const QList<ivm::meta::Props::Token> types { ivm::meta::Props::Token::coordinates,
        ivm::meta::Props::Token::InnerCoordinates, ivm::meta::Props::Token::RootCoordinates };

    QPointF pos = ive::pos(entity()->coordinates());
    int idx = 0;
    ivm::meta::Props::Token token = entity()->coordinatesType();
    while (pos.isNull() && idx < types.size()) {
        token = types.at(idx);
        const QString strCoordinates = entity()->prop(ivm::meta::Props::token(token)).toString();
        pos = ive::pos(ivm::IVObject::coordinatesFromString(strCoordinates));
        ++idx;
    }
    if (pos.isNull()) {
        adjustItem();
        /// NOTE: iface items without connections are put close to top left corner
        /// because of null pos
        return;
    }

    const auto parentFn = entity()->parentObject()->as<ivm::IVFunctionType *>();
    const QRectF fnRect =
            ive::rect(ivm::IVObject::coordinatesFromString(parentFn->prop(ivm::meta::Props::token(token)).toString()))
                    .normalized();
    const auto side = getNearestSide(fnRect, pos);
    pos = getSidePosition(fnRect, pos, side);

    const QRectF sceneParentFnRect = targetItem()->sceneBoundingRect();
    if (qFuzzyCompare(fnRect.top(), pos.y())) {
        const qreal sf = (pos.x() - fnRect.left()) / (fnRect.right() - fnRect.left());
        pos = QLineF(sceneParentFnRect.topLeft(), sceneParentFnRect.topRight()).pointAt(sf);
    } else if (qFuzzyCompare(fnRect.bottom(), pos.y())) {
        const qreal sf = (pos.x() - fnRect.left()) / (fnRect.right() - fnRect.left());
        pos = QLineF(sceneParentFnRect.bottomLeft(), sceneParentFnRect.bottomRight()).pointAt(sf);
    } else if (qFuzzyCompare(fnRect.left(), pos.x())) {
        const qreal sf = (pos.y() - fnRect.top()) / (fnRect.bottom() - fnRect.top());
        pos = QLineF(sceneParentFnRect.topLeft(), sceneParentFnRect.bottomLeft()).pointAt(sf);
    } else if (qFuzzyCompare(fnRect.right(), pos.x())) {
        const qreal sf = (pos.y() - fnRect.top()) / (fnRect.bottom() - fnRect.top());
        pos = QLineF(sceneParentFnRect.topRight(), sceneParentFnRect.bottomRight()).pointAt(sf);
    }
    updateInternalItems(side);
    setPos(targetItem()->mapFromScene(pos));
}

/*!
   Returns the maximal width of this item. The width is limited by other functions and interfaces on the right.
   If there is no other item on the right, -1 (unlimited) is returned.
 */
qreal AADLInterfaceGraphicsItem::maxWidth() const
{
    qreal width = -1.;
    if (!scene()) {
        return width;
    }

    const QRectF itemRect = sceneBoundingRect();
    const qreal itemLeft = itemRect.left();
    QRectF rect = sceneBoundingRect();
    rect.setWidth(9e12); // extend to the right very far (infinite)
    for (QGraphicsItem *rootItem : scene()->items(rect)) {
        if (dynamic_cast<ive::AADLFunctionTypeGraphicsItem *>(rootItem)
                || dynamic_cast<ive::AADLCommentGraphicsItem *>(rootItem)) {
            QList<QGraphicsItem *> items;
            for (QGraphicsItem *item : rootItem->childItems()) {
                if (dynamic_cast<ive::AADLFunctionTypeGraphicsItem *>(item)
                        || dynamic_cast<ive::AADLInterfaceGraphicsItem *>(item)) {
                    items.append(item);
                }
            }
            items.append(rootItem);

            for (QGraphicsItem *item : items) {
                if (item->isVisible() && item != this && item != parentItem()) {
                    const QRectF otherRect = item->sceneBoundingRect();
                    if (otherRect.bottom() > itemRect.top() && otherRect.top() < itemRect.bottom()) {
                        // the items do vertically intersect
                        if (otherRect.left() > itemLeft) {
                            if (width < 0.) {
                                width = otherRect.left() - itemLeft;
                            } else {
                                width = std::min(otherRect.left() - itemLeft, width);
                            }
                        }
                    }
                }
            }
        }
    }

    return width;
}

void AADLInterfaceGraphicsItem::onManualMoveProgress(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    if (!scene())
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

    AADLConnectionGraphicsItem::layoutInterfaceConnections(this, AADLConnectionGraphicsItem::LayoutPolicy::LastSegment,
            AADLConnectionGraphicsItem::CollisionsPolicy::Rebuild, true);

    Q_EMIT boundingBoxChanged();
}

void AADLInterfaceGraphicsItem::onManualMoveFinish(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    const QPointF shift = { to - from };
    if (shift.isNull())
        return;

    updateEntity();
}

shared::ColorManager::HandledColors AADLInterfaceGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Iface;
}

void AADLInterfaceGraphicsItem::adjustItem()
{
    if (!parentItem()) {
        return;
    }

    QList<QRectF> siblingsRects;
    const QList<QGraphicsItem *> siblingItems = parentItem()->childItems();
    std::for_each(siblingItems.cbegin(), siblingItems.cend(), [this, &siblingsRects](const QGraphicsItem *sibling) {
        if (sibling->type() == AADLInterfaceGraphicsItem::Type && sibling != this) {
            QRectF itemRect = sibling->boundingRect();
            itemRect.setWidth(kInterfaceMinDistance - itemRect.x());
            itemRect = sibling->mapRectToParent(itemRect);
            siblingsRects.append(itemRect);
        }
    });

    const QPointF initialOffset = boundingRect().topLeft();
    QRectF itemRect = boundingRect();
    itemRect.setWidth(kInterfaceMinDistance - itemRect.x());
    itemRect = mapRectToParent(itemRect);
    const QRectF parentRect = parentItem()->boundingRect();

    QRectF intersectedRect;
    if (isCollided(siblingsRects, itemRect, &intersectedRect) && parentRect.isValid()) {
        const QHash<Qt::Alignment, QPainterPath> kSidePaths {
            { Qt::AlignLeft, itemPath(Qt::AlignLeft) },
            { Qt::AlignTop, itemPath(Qt::AlignTop) },
            { Qt::AlignRight, itemPath(Qt::AlignRight) },
            { Qt::AlignBottom, itemPath(Qt::AlignBottom) },
        };
        PositionLookupHelper cwHelper(kSidePaths, parentRect, siblingsRects, itemRect, initialOffset, true);
        PositionLookupHelper ccwHelper(kSidePaths, parentRect, siblingsRects, itemRect, initialOffset, false);
        while (cwHelper.hasNext() || ccwHelper.hasNext()) {
            if (cwHelper.lookup()) {
                if (cwHelper.isSideChanged())
                    updateInternalItems(cwHelper.side());
                setPos(cwHelper.mappedOriginPoint());
                break;
            } else if (ccwHelper.lookup()) {
                if (ccwHelper.isSideChanged())
                    updateInternalItems(ccwHelper.side());
                setPos(ccwHelper.mappedOriginPoint());
                break;
            }
        }
        for (auto connection : m_connections) {
            connection->layout();
        }
    }
}

qreal AADLInterfaceGraphicsItem::typeIconHeight() const
{
    return kHeight;
}
qreal AADLInterfaceGraphicsItem::baseLength() const
{
    return kBase;
}

void AADLInterfaceGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
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
    m_type->setPath(typePath());
    m_shape = composeShape();
}

void AADLInterfaceGraphicsItem::updateIface()
{
    m_iface->setPath(ifacePath());
    m_shape = composeShape();
}

QString AADLInterfaceGraphicsItem::ifaceLabel() const
{
    if (entity()) {
        return entity()->ifaceLabel();
    }
    return {};
}

QString AADLInterfaceGraphicsItem::prepareTooltip() const
{
    QString toolTip = InteractiveObject::prepareTooltip();
    if (entity()->isProvided())
        return toolTip;

    auto ri = entity()->as<const ivm::IVInterfaceRequired *>();
    if (!ri)
        return toolTip;

    const QString label = ivm::IVNameValidator::decodeName(entity()->type(), ifaceLabel());
    if (toolTip != label)
        toolTip = QString("%1<br><i><b>%2</b></i>").arg(label, toolTip);

    return toolTip;
}

void AADLInterfaceGraphicsItem::onAttrOrPropChanged(ivm::meta::Props::Token t)
{
    switch (t) {
        //    ivm::meta::Props::Token::name: // handled in AADLInterfaceGraphicsItem::updateLabel
    case ivm::meta::Props::Token::InheritPI: {
#ifdef QT_DEBUG
        qWarning() << "check for a call duplication" << t;
#endif
        updateLabel();
        break;
    }
    case ivm::meta::Props::Token::kind:
        updateKind();
        break;
    default:
        break;
    }
}

QTransform AADLInterfaceGraphicsItem::typeTransform(Qt::Alignment alignment) const
{
    const qreal offset = kBase + 2;

    QPointF shift(0., 0.);
    switch (alignment) {
    case Qt::AlignLeft:
        shift.setX(-offset);
        break;
    case Qt::AlignRight:
        shift.setX(offset);
        break;
    case Qt::AlignTop:
        shift.setY(-offset);
        break;
    case Qt::AlignBottom:
        shift.setY(offset);
        break;
    }

    return QTransform().translate(shift.x(), shift.y());
}

QTransform AADLInterfaceGraphicsItem::ifaceTransform(Qt::Alignment alignment) const
{
    const bool insideOut = entity()->direction() == ivm::IVInterface::InterfaceType::Required;
    qreal rotationDegree = 0.;
    switch (alignment) {
    case Qt::AlignLeft:
        rotationDegree = insideOut ? 180 : 0;
        break;
    case Qt::AlignRight:
        rotationDegree = insideOut ? 0 : 180;
        break;
    case Qt::AlignTop:
        rotationDegree = insideOut ? 270 : 90;
        break;
    case Qt::AlignBottom:
        rotationDegree = insideOut ? 90 : 270;
        break;
    }

    return QTransform().rotate(rotationDegree);
}

QPainterPath AADLInterfaceGraphicsItem::ifacePath() const
{
    QPainterPath path;
    path.addPolygon(QVector<QPointF> {
            QPointF(-kHeight / 3, -kBase / 2), QPointF(-kHeight / 3, kBase / 2), QPointF(2 * kHeight / 3, 0) });
    path.closeSubpath();
    return path;
}

QPainterPath AADLInterfaceGraphicsItem::typePath() const
{
    auto iface = qobject_cast<ivm::IVInterface *>(entity());
    if (!iface)
        return {};

    QPainterPath kindPath;
    switch (iface->kind()) {
    case ivm::IVInterface::OperationKind::Cyclic: {
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
    case ivm::IVInterface::OperationKind::Sporadic: {
        const qreal kindBaseValue = kHeight;
        kindPath.moveTo(-kindBaseValue / 2, 0);
        kindPath.lineTo(0, -kindBaseValue / 4);
        kindPath.lineTo(0, kindBaseValue / 4);
        kindPath.lineTo(kindBaseValue / 2, 0);
        break;
    }
    case ivm::IVInterface::OperationKind::Protected: {
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
    return kindPath;
}

QPainterPath AADLInterfaceGraphicsItem::itemPath(Qt::Alignment alignment) const
{
    QPainterPath path = m_text->shape();
    path.addPath(typeTransform(alignment).map(m_type->path()));
    path.addPath(ifaceTransform(alignment).map(m_iface->path()));
    return path;
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
        for (const auto &connection : m_connections)
            connection->setVisible(connection->startItem()->isVisible() && connection->endItem()->isVisible());
        break;
    }
    default:
        break;
    }
    return InteractiveObject::itemChange(change, value);
}

}
