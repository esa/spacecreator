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

#include "graphicsviewutils.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPointF>

class QLineF;

namespace msc {

class InstanceItem;

constexpr qreal LineHoverTolerance = 10.;
constexpr qreal LineHorizontalityTolerance = 15.;

enum class CifUpdatePolicy
{
    DontChange,
    ForceCreate,
    UpdateIfExists
};

/*!
  Returns the item of a specific entity in the given scene;
 */
template<typename ItemType, typename MscEntityType>
ItemType *itemForEntity(MscEntityType *event, QGraphicsScene *scene)
{
    if (event)
        for (ItemType *item : shared::graphicsviewutils::toplevelItems<ItemType>(scene))
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

bool removeSceneItem(QGraphicsItem *item);
QVector<InstanceItem *> instanceItemsByPos(QGraphicsScene *scene, const QPointF &scenePos, bool extendToBottom = false);

bool isHorizontal(const QLineF &line, const qreal verticalTolerance = LineHorizontalityTolerance);
bool isHorizontal(const QPointF &p1, const QPointF &p2, const qreal verticalTolerance = LineHorizontalityTolerance);
bool isHorizontal(const QVector<QPointF> &twoPoints, const qreal verticalTolerance = LineHorizontalityTolerance);

QRectF framedRect(const QRectF &rect, qreal frameWidth);

}
