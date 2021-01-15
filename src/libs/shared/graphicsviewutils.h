#pragma once

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

}
}
