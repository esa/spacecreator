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
#pragma once

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPointF>
#include <QPointer>

class QLineF;
class QPropertyAnimation;

namespace msc {

class InstanceItem;

namespace utils {

static constexpr qreal LineHoverTolerance = 10.;

enum class CifUpdatePolicy
{
    DontChange,
    ForceCreate,
    UpdateIfExists
};

template<class ItemType, class TargetPositioning>
QVector<ItemType *> itemByPos(QGraphicsScene *scene, const TargetPositioning &scenePos)
{
    QVector<ItemType *> items;
    if (scene)
        for (QGraphicsItem *item : scene->items(scenePos, Qt::IntersectsItemBoundingRect)) {
            if (item->parentItem())
                continue;

            if (ItemType *instance = dynamic_cast<ItemType *>(item))
                items << instance;
        }
    return items;
}

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

/*!
  Returns the item of a specific entity in the given scene;
 */
template<typename ItemType, typename MscEntityType>
ItemType *itemForEntity(MscEntityType *event, QGraphicsScene *scene)
{
    if (event)
        for (ItemType *item : utils::toplevelItems<ItemType>(scene))
            if (item && item->modelEntity() && item->modelEntity()->internalId() == event->internalId())
                return item;

    return nullptr;
}

/*!
  Returns the specified Entities from given scene;
 */
template<typename ItemType, typename MscEntityType>
QVector<MscEntityType *> getEntityFromSelection(QGraphicsScene *scene)
{
    QVector<MscEntityType *> items;
    for (auto item : scene->selectedItems()) {
        auto obj = dynamic_cast<ItemType *>(item);
        if (obj && obj->modelItem()) {
            items.append(obj->modelItem());
        }
    }
    return items;
}

QPainterPath lineShape(const QLineF &line, qreal span);
QPointF lineCenter(const QLineF &line);
QPointF pointFromPath(const QPainterPath &path, int num);
QPropertyAnimation *createLinearAnimation(QObject *target, const QString &propName, const QVariant &from,
                                          const QVariant &to, const int durationMs);
QPointF snapToPointByX(const QPointF &target, const QPointF &source, qreal tolerance);
bool removeSceneItem(QGraphicsItem *item);
bool intersects(const QRectF &rect, const QLineF &line);
QVector<InstanceItem *> instanceItemsByPos(QGraphicsScene *scene, const QPointF &scenePos);

} // ns utils
} // ns msc
