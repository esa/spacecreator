#pragma once

#include "utils.h"

#include <QGraphicsItem>
#include <QGraphicsScene>

class QPainterPath;

namespace shared {
namespace graphicsviewutils {

QPainterPath lineShape(const QLineF &line, qreal span);
QPainterPath edgeCuttedRectShape(const QRectF &rect, qreal cut);
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos = nullptr);
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos = nullptr);
QVector<QPointF> intersectionPoints(const QRectF &rect, const QPolygonF &polygon);
qreal distanceLine(const QPointF &pnt1, const QPointF &pnt2);

Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos);

QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side);

QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QPointF &pos, const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QRectF &area, const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &center, qreal offset,
        const QList<int> &acceptableTypes = QList<int>());

bool alignedLine(QLineF &line, int angleTolerance = 10);

QRectF adjustedRect(const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side,
        const topohelp::LookupDirection direction);

bool isCollided(const QList<QRectF> &itemRects, const QRectF &itemRect, QRectF *collidingRect = nullptr);

QList<QVector<QPointF>> generateSegments(const QPointF &startPoint, const QPointF &endPoint);
QVector<QPointF> generateSegments(const QLineF &startDirection, const QLineF &endDirection);
QLineF ifaceSegment(const QRectF &sceneRect, const QPointF &firstEndPoint, const QPointF &lastEndPoint);
QVector<QPointF> path(const QList<QRectF> &existingRects, const QPointF &startPoint, const QPointF &endPoint);
QVector<QPointF> path(const QList<QRectF> &existingRects, const QLineF &startDirection, const QLineF &endDirection);

QVector<QPointF> createConnectionPath(const QList<QRectF> &existingRects, const QPointF &startIfacePos,
        const QRectF &sourceRect, const QPointF &endIfacePos, const QRectF &targetRect);
QVector<QPointF> simplifyPoints(const QVector<QPointF> &points);
bool comparePolygones(const QVector<QPointF> &v1, const QVector<QPointF> &v2);

QPointF pos(const QVector<qint32> &coordinates);
QRectF rect(const QVector<qint32> &coordinates);
QVector<QPointF> polygon(const QVector<qint32> &coordinates);

QVector<qint32> coordinates(const QPointF &point);
QVector<qint32> coordinates(const QRectF &rect);
QVector<qint32> coordinates(const QVector<QPointF> &points);

QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment);

QRectF getNearestIntersectedRect(const QList<QRectF> &existingRects, const QVector<QPointF> &points,
        topohelp::IntersectionType intersectionType);

bool isOnVerticalSide(const QRectF &rect, const QPointF &point);
bool isOnHorizontalSide(const QRectF &rect, const QPointF &point);
bool rectContainsPoint(const QRectF &rect, const QPointF &point, bool proper = true);

QList<QRectF> siblingItemsRects(const QGraphicsItem *item);

QRectF collidingRect(const QRectF &rect, const QList<QRectF> &existingRects);
void findGeometryForPoint(QPointF &itemRect, const QRectF &boundedRect, const QList<QRectF> &existingRects = {},
        const QMarginsF &margins = topohelp::kRootMargins);
void findGeometryForRect(QRectF &itemRect, QRectF &boundedRect, const QList<QRectF> &existingRects = {},
        const QMarginsF &margins = topohelp::kRootMargins);

bool isRectBounded(const QRectF &outerRect, const QRectF &innerRect);

bool isBounded(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect);

bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect);

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
