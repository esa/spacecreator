/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QPointF>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QPointer>
#include <QGraphicsObject>

class QLineF;
class QPropertyAnimation;

namespace msc {

class InstanceItem;

namespace utils {

static constexpr qreal LineHoverTolerance = 10;

template<class ItemType, class TargetPositioning>
QVector<ItemType *> itemByPos(QGraphicsScene *scene, const TargetPositioning &scenePos)
{
    QVector<ItemType *> items;
    if (scene)
        for (QGraphicsItem *item : scene->items(scenePos)) {
            if (item->parentItem())
                continue;

            if (ItemType *instance = dynamic_cast<ItemType *>(item))
                items << instance;
        }
    return items;
}

QPainterPath lineShape(const QLineF &line, qreal span);
QPointF lineCenter(const QLineF &line);
QPointF pointFromPath(const QPainterPath &path, int num);
QPropertyAnimation *createLinearAnimation(QObject *target,
                                          const QString &propName,
                                          const QVariant &from,
                                          const QVariant &to,
                                          const int durationMs);
QPointF snapToPointByX(const QPointF &target, const QPointF &source, qreal tolerance);
bool removeSceneItem(QGraphicsItem *item);
QList<QGraphicsItem *> toplevelItems(QGraphicsScene *scene);
} // ns utils
} // ns msc
