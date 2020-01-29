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

#include "aadlfunctiongraphicsitem.h"

#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "baseitems/common/utils.h"
#include "colors/colormanager.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectsmodel.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QtDebug>
#include <QtMath>
#include <baseitems/grippointshandler.h>

static const qreal kBorderWidth = 2.0;
static const qreal kRadius = 10.0;
static const qreal kOffset = kBorderWidth / 2.0;

namespace taste3 {
namespace aadl {

AADLFunctionGraphicsItem::AADLFunctionGraphicsItem(AADLObjectFunction *entity, QGraphicsItem *parent)
    : AADLFunctionTypeGraphicsItem(entity, parent)
{
    setObjectName(QLatin1String("AADLFunctionGraphicsItem"));
    m_textItem->setVisible(!isRootItem());
    colorSchemeUpdated();
}

AADLObjectFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectFunction *>(dataObject());
}

QPainterPath AADLFunctionGraphicsItem::shape() const
{
    QPainterPath pp;
    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        pp.addRect(br);
    else
        pp.addRoundedRect(br, kRadius, kRadius);
    return pp;
}

bool AADLFunctionGraphicsItem::isRootItem() const
{
    return !parentItem() && entity() && entity()->isRootObject();
}

void AADLFunctionGraphicsItem::rebuildLayout()
{
    AADLFunctionTypeGraphicsItem::rebuildLayout();

    //    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    //    if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect))
    //        doAutoLayout(this);

    //    layoutConnections();
    colorSchemeUpdated();
}

void AADLFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? m_selectedPen : m_pen);
    painter->setBrush(brush());

    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        painter->drawRect(br);
    else
        painter->drawRoundedRect(br, kRadius, kRadius);
    painter->restore();
}

QVariant AADLFunctionGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemParentHasChanged) {
        m_textItem->setVisible(!isRootItem());
        colorSchemeUpdated();
    } else if (change == QGraphicsItem::ItemChildAddedChange) {
        colorSchemeUpdated();
    }
    return AADLFunctionTypeGraphicsItem::itemChange(change, value);
}

void AADLFunctionGraphicsItem::onManualMoveProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    if (isRootItem())
        return;

    AADLFunctionTypeGraphicsItem::onManualMoveProgress(grip, from, to);
    layoutOuterConnections();
}

void AADLFunctionGraphicsItem::onManualMoveFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                  const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (handlePositionChanged(pressedAt, releasedAt) && !isRootItem())
        createCommand();
}

void AADLFunctionGraphicsItem::onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = mapRectToParent(boundingRect());
    const QRectF contentRect =
            parentItem() ? parentItem()->boundingRect().marginsRemoved(utils::kContentMargins) : QRectF();
    switch (grip) {
    case GripPoint::Left: {
        const qreal left = rect.left() + shift.x();
        if (contentRect.isNull() || left >= contentRect.left())
            rect.setLeft(left);
    } break;
    case GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        if (contentRect.isNull() || top >= contentRect.top())
            rect.setTop(top);
    } break;
    case GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        if (contentRect.isNull() || right <= contentRect.right())
            rect.setRight(right);
    } break;
    case GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        if (contentRect.isNull() || bottom <= contentRect.bottom())
            rect.setBottom(bottom);
    } break;
    case GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(topLeft))
            rect.setTopLeft(topLeft);
    } break;
    case GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        if (contentRect.isNull() || contentRect.contains(topRight))
            rect.setTopRight(topRight);
    } break;
    case GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomLeft))
            rect.setBottomLeft(bottomLeft);
    } break;
    case GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomRight))
            rect.setBottomRight(bottomRight);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    const QRectF normalized = rect.normalized();
    if (isRootItem() && !normalized.marginsRemoved(utils::kContentMargins).contains(nestedItemsSceneBoundingRect()))
        return;

    if (normalized.width() >= minimalSize().width() && normalized.height() >= minimalSize().height()) {
        const QPointF offset = parentItem()
                ? (sceneBoundingRect().topLeft() - parentItem()->mapRectToScene(normalized).topLeft())
                : sceneBoundingRect().topLeft() - normalized.topLeft();
        setRect(parentItem() ? parentItem()->mapRectToScene(normalized) : normalized);
        for (QGraphicsItem *child : childItems()) {
            if (auto function = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(child))
                function->moveBy(offset.x(), offset.y());
        }
    }

    //    rebuildLayout();
    updateGripPoints();

    layoutConnections();
    //    Q_EMIT needUpdateLayout();
}

void AADLFunctionGraphicsItem::onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                    const QPointF &releasedAt)
{
    if (handleGeometryChanged(grip, pressedAt, releasedAt) && !isRootItem()) {
        const QRectF nestedRect = nestedItemsSceneBoundingRect();
        if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect))
            doAutoLayout(this);
        createCommand();
    }
}

void AADLFunctionGraphicsItem::layoutConnections()
{
    /// TODO: filter connections should be relayouted to avoid unnecessary paths generation
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            for (AADLConnectionGraphicsItem *connection : iface->connectionItems()) {
                if (!connection->startItem() || !connection->endItem()) {
                    qCritical() << "Connection without end point(s):" << connection;
                    continue;
                }
                connection->instantLayoutUpdate();
                //  connection->scheduleLayoutUpdate();
            }
        } else if (auto connection = qgraphicsitem_cast<AADLConnectionGraphicsItem *>(item)) {
            if (connection->sourceItem() != this && connection->targetItem() != this)
                connection->instantLayoutUpdate();
            //  connection->scheduleLayoutUpdate();
        }
    }
}

void AADLFunctionGraphicsItem::layoutOuterConnections()
{
    /// TODO: filter connections should be relayouted to avoid unnecessary paths generation
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            for (AADLConnectionGraphicsItem *connection : iface->connectionItems()) {
                if (!connection->startItem() || !connection->endItem()) {
                    qCritical() << "Connection without end point(s):" << connection;
                    continue;
                }
                if (!isAncestorOf(connection->startItem()) || !isAncestorOf(connection->endItem())) {
                    connection->instantLayoutUpdate();
                    //  connection->scheduleLayoutUpdate();
                }
            }
        }
    }
}

void AADLFunctionGraphicsItem::updateTextPosition()
{
    const QTransform tr = scene()->views().isEmpty() ? QTransform() : scene()->views().front()->viewportTransform();
    const QTransform dt = deviceTransform(tr);
    const QPointF currScale { dt.m11(), dt.m22() };

    QRectF textRect { 0, 0, m_textItem->boundingRect().width() / currScale.x(),
                      m_textItem->boundingRect().height() / currScale.y() };
    textRect.moveCenter(boundingRect().center());
    m_textItem->setPos(textRect.topLeft());
}

QRectF AADLFunctionGraphicsItem::nestedItemsSceneBoundingRect() const
{
    QRectF nestedItemsBoundingRect;
    for (auto item : childItems()) {
        if (item->type() == AADLFunctionTypeGraphicsItem::Type || item->type() == AADLFunctionGraphicsItem::Type) {
            const QRectF nestedRect = item->sceneBoundingRect();
            if (nestedRect.isValid())
                nestedItemsBoundingRect |= nestedRect;
        }
    }
    return nestedItemsBoundingRect;
}

void AADLFunctionGraphicsItem::updateFromEntity()
{
    aadl::AADLObjectFunction *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const auto innerCoordinates = obj->innerCoordinates();
    const auto coordinates = obj->coordinates();
    const bool usingInnerPoints =
            obj->parentObject() && obj->parentObject()->isRootObject() && !innerCoordinates.isEmpty();
    const auto &points = usingInnerPoints ? innerCoordinates : coordinates;

    const QRectF sceneRect { QPointF(points.value(0), points.value(1)), QPointF(points.value(2), points.value(3)) };
    if (points.isEmpty() || !sceneRect.isValid())
        instantLayoutUpdate();
    else
        setRect(sceneRect);

    //    if (auto parentFunction = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem())) {
    //        const QRectF parentSceneRect = parentFunction->sceneBoundingRect();
    //        if (!parentFunction->isRootItem() && (!sceneRect.isValid() || !parentSceneRect.contains(sceneRect)))
    //            doAutoLayout(parentFunction);
    //    }

    //    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    //    if ((nestedRect.isValid() && !sceneRect.contains(nestedRect)))
    //        doAutoLayout(this);

    //    const QList<QGraphicsItem *> collidedItems = scene()->items(sceneRect);
    //    auto collidedIt =
    //            std::find_if(collidedItems.constBegin(), collidedItems.constEnd(), [this, sceneRect](QGraphicsItem
    //            *item) {
    //                if (item->parentItem() != parentItem())
    //                    return false;

    //                if (item->type() != AADLFunctionGraphicsItem::Type
    //                    || item->type() != AADLFunctionTypeGraphicsItem::Type)
    //                    return false;

    //                return sceneRect.contains(item->sceneBoundingRect());
    //            });
    //    if (collidedIt != collidedItems.constEnd())
    //        doAutoLayout(qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem()));
}

ColorManager::HandledColors AADLFunctionGraphicsItem::handledColorType() const
{
    if (isRootItem())
        return ColorManager::HandledColors::FunctionRoot;

    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect))
        return ColorManager::HandledColors::FunctionPartial;

    return ColorManager::HandledColors::FunctionRegular;
}

AADLObject *AADLFunctionGraphicsItem::aadlObject() const
{
    return entity();
}

void AADLFunctionGraphicsItem::colorSchemeUpdated()
{
    const ColorHandler &h = colorHandler();
    QPen p = h.pen();
    QBrush b = h.brush();

    if (auto parentFunction = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem()))
        if (!parentFunction->entity()->props().contains("color") && !entity()->props().contains("color")) {
            b.setColor(parentFunction->brush().color().darker(125));
            p.setColor(parentFunction->pen().color().darker(125));
        }

    setPen(p);
    setBrush(b);

    update();
}

void AADLFunctionGraphicsItem::doAutoLayout(AADLFunctionGraphicsItem *function)
{
    if (!function)
        return;

    QList<aadl::AADLFunctionGraphicsItem *> nestedFunctions;
    for (auto nestedItem : function->childItems())
        if (auto function = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(nestedItem))
            nestedFunctions.append(function);
    if (nestedFunctions.isEmpty())
        return;

    const int columnCount = qCeil(qSqrt(nestedFunctions.size()));
    const int rowCount = qCeil(qreal(nestedFunctions.size()) / columnCount);
    const QRectF itemRect = function->sceneBoundingRect();
    const QSizeF nestedSize = QSizeF { itemRect.width() / columnCount, itemRect.height() / rowCount };
    QRectF nestedRect { QPointF(0, 0), nestedSize * 0.75 };
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            const QPointF nestedCenter { (column + 0.5) * nestedSize.width(), (row + 0.5) * nestedSize.height() };
            nestedRect.moveCenter(nestedCenter + itemRect.topLeft());
            const int idx = row * columnCount + column;
            if (auto item = nestedFunctions.value(idx)) {
                const bool isItemVisible = itemRect.contains(nestedRect);
                item->setVisible(isItemVisible);
                if (isItemVisible)
                    item->setRect(nestedRect);
            }
        }
    }
    function->colorSchemeUpdated();

    for (auto nestedItem : nestedFunctions)
        nestedItem->colorSchemeUpdated();

    function->layoutConnections();
}

} // namespace aadl
} // namespace taste3
