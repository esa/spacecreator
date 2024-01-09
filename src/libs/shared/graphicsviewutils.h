#pragma once

#include "topohelper/geometry.h"

#include <QGraphicsItem>
#include <QGraphicsScene>

class QPainterPath;

namespace shared {
namespace graphicsviewutils {

QPainterPath lineShape(const QLineF &line, qreal span);
QPainterPath edgeCuttedRectShape(const QRectF &rect, qreal cut);

/// Enum to select if the item closer to the border of the selcted point more inside should be preferred
enum class DistanceCondition
{
    CloseToBorder, /// Prefer the closest border if several items are close to the selection point/area
    InsideItem /// Prefer the "most inside the item" if several items are close to the selection point/area
};
QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QPointF &pos, const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QRectF &area,
        const QList<int> &acceptableTypes = QList<int>(),
        const DistanceCondition distanceCondition = DistanceCondition::CloseToBorder);
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &center, qreal offset,
        const QList<int> &acceptableTypes = QList<int>(),
        const DistanceCondition distanceCondition = DistanceCondition::CloseToBorder);

bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect);

QList<QRectF> siblingItemsRects(const QGraphicsItem *item);

void findGeometryForPoint(QPointF &itemRect, const QRectF &boundedRect, const QList<QRectF> &existingRects = {},
        const QMarginsF &margins = topohelp::kRootMargins);

bool isBounded(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect);

//! \brief Get the top level items in a scene
template<class ItemType>
QList<ItemType *> toplevelItems(QGraphicsScene *scene)
{
    QList<ItemType *> items;
    if (scene) {
        for (QGraphicsItem *item : scene->items())
            if (!item->parentItem())
                if (ItemType *casted = dynamic_cast<ItemType *>(item))
                    items.append(casted);
    }
    return items;
}

//! \brief Get all top level items of a specific class at a specific position
template<class ItemType, class TargetPositioning>
QVector<ItemType *> itemByPos(QGraphicsScene *scene, const TargetPositioning &scenePos)
{
    QVector<ItemType *> items;
    if (scene) {
        for (QGraphicsItem *item : scene->items(scenePos, Qt::IntersectsItemBoundingRect)) {
            if (item->parentItem() != nullptr) {
                continue;
            }

            if (ItemType *instance = dynamic_cast<ItemType *>(item)) {
                items << instance;
            }
        }
    }
    return items;
}

template<typename ItemType>
ItemType *itemAt(const QGraphicsScene *scene, const QPointF &point)
{
    QList<QGraphicsItem *> items = scene->items(point);
    if (items.isEmpty())
        return nullptr;
    auto it = std::find_if(items.constBegin(), items.constEnd(),
            [](const QGraphicsItem *item) { return item->type() == ItemType::Type; });
    if (it == items.constEnd())
        return nullptr;

    return qgraphicsitem_cast<ItemType *>(*it);
}

void drawText(QPainter *painter, const QRectF &rect, const QString &text, qreal margin);

/*!
 * Draws a horizontal helper line as child of \p parent. Helper lines in a very long dashed gray line
 */
QGraphicsLineItem *horizontalHelper(qreal y, QGraphicsItem *parent = nullptr);

/*!
 * Draws a horizontal helper line as child of \p parent. Helper lines in a very long dashed gray line
 */
QGraphicsLineItem *verticalHelper(qreal x, QGraphicsItem *parent = nullptr);

/*!
 * If snapping is enabled and supported, the given point is snapped to a virtual grid
 */
QPointF snappedPoint(QGraphicsScene *scene, const QPointF &pos);

/*!
 * If snapping is enabled and supported, the all the sides of the given rectangle are snapped to a virtual grid
 */
QRectF snappedRect(QGraphicsScene *scene, const QRectF &rect);

} // namespace graphicsviewutils
} // namespace shared
