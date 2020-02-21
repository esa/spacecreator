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

#include "aadlcommentgraphicsitem.h"
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
    m_textItem->setVisible(!isRootItem());
    colorSchemeUpdated();
}

AADLObjectFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<aadl::AADLObjectFunction *>(aadlObject());
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
    QRectF nestedItemsInternalRect;
    bool needRelayout = false;
    for (QGraphicsItem *item : childItems()) {
        if (auto iObj = qobject_cast<aadl::AADLRectGraphicsItem *>(item->toGraphicsObject())) {
            if (auto entity = iObj->aadlObject()) {
                const QRectF objRect = utils::rect(entity->coordinates());
                if (objRect.isValid())
                    nestedItemsInternalRect |= objRect;
                else
                    needRelayout = true;
            }
        }
    }
    if (isRootItem()) {
        QRectF itemRect;
        QRectF viewportRect;
        auto view = scene()->views().value(0);
        if (view) {
            const QRect viewportGeometry =
                    view->viewport()->geometry().marginsRemoved(utils::kContentMargins.toMargins());
            viewportRect = QRectF(view->mapToScene(viewportGeometry.topLeft()),
                                  view->mapToScene(viewportGeometry.bottomRight()));
        }
        const QRectF nestedRect = nestedItemsInternalRect.isValid()
                ? nestedItemsInternalRect.marginsAdded(utils::kRootMargins)
                : QRectF();
        if (nestedRect.isValid()) {
            viewportRect.moveCenter(nestedRect.center());
            itemRect = viewportRect.united(nestedRect);
        } else {
            itemRect = viewportRect;
        }

        const QPointF offset = scenePos() - itemRect.topLeft();
        if (itemRect != sceneBoundingRect())
            setGeometry(itemRect);

        if (nestedItemsInternalRect.isValid()) {
            for (QGraphicsItem *child : childItems()) {
                if (auto function = qobject_cast<aadl::AADLRectGraphicsItem *>(child->toGraphicsObject()))
                    function->moveBy(offset.x(), offset.y());
            }
            scene()->setSceneRect({});
        }
        if (view)
            view->centerOn(itemRect.center());
    }
    if (needRelayout)
        doAutoLayout();

    AADLFunctionTypeGraphicsItem::rebuildLayout();

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
        scheduleUpdateNestedItems();
    }
    return AADLFunctionTypeGraphicsItem::itemChange(change, value);
}

void AADLFunctionGraphicsItem::onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (isRootItem())
        return;

    AADLFunctionTypeGraphicsItem::onManualMoveProgress(grip, from, to);
    layoutOuterConnections();
}

void AADLFunctionGraphicsItem::onManualMoveFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (allowGeometryChange(pressedAt, releasedAt) && !isRootItem()) {
        updateEntity();
    } else {
        for (auto child : childItems()) {
            if (auto interface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(child)) {
                for (auto connection : interface->connectionItems()) {
                    if (connection->parent() != this) // outer connections
                        connection->updateFromEntity();
                }
            }
        }
    }
}

void AADLFunctionGraphicsItem::onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QRectF rect = adjustRectToParent(grip, from, to);

    if (isRootItem() && !rect.marginsRemoved(utils::kRootMargins).contains(nestedItemsSceneBoundingRect()))
        return;

    if (rect.width() >= minimalSize().width() && rect.height() >= minimalSize().height()) {
        const QPointF offset = parentItem()
                ? (sceneBoundingRect().topLeft() - parentItem()->mapRectToScene(rect).topLeft())
                : sceneBoundingRect().topLeft() - rect.topLeft();
        if (isRootItem())
            setGeometry(rect);
        else
            setRect(parentItem() ? parentItem()->mapRectToScene(rect) : rect);

        for (QGraphicsItem *child : childItems()) {
            if (auto iObj = qobject_cast<aadl::AADLRectGraphicsItem *>(child->toGraphicsObject()))
                iObj->moveBy(offset.x(), offset.y());
        }
    }

    //    rebuildLayout();
    updateGripPoints();

    layoutConnections();
    //    Q_EMIT needUpdateLayout();
}

void AADLFunctionGraphicsItem::onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt,
                                                    const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (allowGeometryChange(pressedAt, releasedAt) && !isRootItem()) {
        const QRectF nestedRect = nestedItemsSceneBoundingRect();
        if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect))
            doAutoLayout();
        else
            updateEntity();
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

void AADLFunctionGraphicsItem::setGeometry(const QRectF &sceneGeometry)
{
    /// Silent geometry change without relayouting
    setPos(sceneGeometry.topLeft());
    prepareGeometryChange();
    m_boundingRect.setSize(sceneGeometry.size());
}

void AADLFunctionGraphicsItem::updateTextPosition()
{
    QRectF textRect { m_textItem->boundingRect() };
    textRect.moveCenter(boundingRect().center());
    m_textItem->setPos(textRect.topLeft());
}

void AADLFunctionGraphicsItem::updateNestedItems()
{
    const QRectF sceneRect = sceneBoundingRect();
    QList<QRectF> nestedRects;
    for (auto item : childItems()) {
        if (auto iObj = qobject_cast<aadl::AADLRectGraphicsItem *>(item->toGraphicsObject())) {
            const QRectF nestedRect = item->sceneBoundingRect();

            if (nestedRect.isEmpty() || !sceneRect.contains(nestedRect.marginsAdded(utils::kContentMargins))) {
                doAutoLayout();
                break;
            }

            auto it = std::find_if(nestedRects.constBegin(), nestedRects.constEnd(),
                                   [nestedRect, margins = utils::kContentMargins](const QRectF &rect) {
                                       return rect.marginsAdded(margins).intersects(nestedRect);
                                   });
            if (it != nestedRects.constEnd()) {
                doAutoLayout();
                break;
            }

            nestedRects.append(nestedRect);
        }
    }
    m_pendingLayout = false;
}

void AADLFunctionGraphicsItem::scheduleUpdateNestedItems()
{
    if (m_pendingLayout)
        return;

    m_pendingLayout = QMetaObject::invokeMethod(this, "updateNestedItems", Qt::QueuedConnection);
}

QRectF AADLFunctionGraphicsItem::nestedItemsSceneBoundingRect() const
{
    QRectF nestedItemsBoundingRect;
    for (auto item : childItems()) {
        if (auto iObj = qobject_cast<aadl::AADLRectGraphicsItem *>(item->toGraphicsObject())) {
            const QRectF nestedRect = item->sceneBoundingRect();
            if (nestedRect.isValid())
                nestedItemsBoundingRect |= nestedRect;
        }
    }
    return nestedItemsBoundingRect;
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

void AADLFunctionGraphicsItem::doAutoLayout()
{
    QSizeF minSize { std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max() };
    QList<AADLRectGraphicsItem *> nestedInteractiveObjects;
    for (auto item : childItems()) {
        if (auto nestedItem = qobject_cast<AADLRectGraphicsItem *>(item->toGraphicsObject())) {
            nestedInteractiveObjects.append(nestedItem);
            minSize = minSize.boundedTo(nestedItem->minimalSize());
        }
    }

    if (nestedInteractiveObjects.isEmpty())
        return;

    const int columnCount = qCeil(qSqrt(nestedInteractiveObjects.size()));
    const int rowCount = qCeil(qreal(nestedInteractiveObjects.size()) / columnCount);
    const QRectF itemRect =
            sceneBoundingRect().marginsRemoved(isRootItem() ? utils::kRootMargins : utils::kContentMargins);
    const QSizeF nestedSize = QSizeF { itemRect.width() / columnCount, itemRect.height() / rowCount };
    if (nestedSize.width() < minSize.width() || nestedSize.height() < minSize.height())
        return;

    QRectF nestedRect { QPointF(0, 0), nestedSize * 0.75 };
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            const QPointF nestedCenter { (column + 0.5) * nestedSize.width(), (row + 0.5) * nestedSize.height() };
            nestedRect.moveCenter(nestedCenter + itemRect.topLeft());
            const int idx = row * columnCount + column;
            if (auto item = nestedInteractiveObjects.value(idx)) {
                const bool isItemVisible = itemRect.contains(nestedRect);
                item->setVisible(isItemVisible);
                if (isItemVisible)
                    item->setRect(nestedRect);
            }
        }
    }
    colorSchemeUpdated();

    //    for (auto nestedItem : nestedFunctions)
    //        nestedItem->colorSchemeUpdated();

    layoutConnections();
    updateEntity();
}

} // namespace aadl
} // namespace taste3
