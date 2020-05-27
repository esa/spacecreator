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
#include "mscutils.h"

#include "baseitems/interactiveobject.h"
#include "instanceitem.h"

#include <QDebug>
#include <QGraphicsView>
#include <QtGlobal>

namespace msc {

bool removeSceneItem(QGraphicsItem *item)
{
    // Removing an item by QGraphicsScene::removeItem + delete
    // crashes the application in QGraphicsSceneFindItemBspTreeVisitor::visit.
    // It seems I messed something up with InteractiveItem's geometry updates,
    // but it's not clear what exactly :(
    //
    // https://forum.qt.io/topic/71316/qgraphicsscenefinditembsptreevisitor-visit-crashes-due-to-an-obsolete-paintevent-after-qgraphicsscene-removeitem
    // https://stackoverflow.com/questions/38458830/crash-after-qgraphicssceneremoveitem-with-custom-item-class
    //
    // Workaround from discussions above:
    // Just delete the item (without removing it from scene). I'm not sure how,
    // but with manual adding/removing it seems to work.
    // But the crash is here agin when stresstesting with QBENCHMARK
    //
    // https://bugreports.qt.io/browse/QTBUG-18021
    //
    // The permanent switch of ItemIndexMethod to the QGraphicsScene::NoIndex leads to not so smooth
    // expirience in a brief manual test.
    // This way a temporrary switch to QGraphicsScene::NoIndex seems to be a best option for now:

    if (!item || !item->scene())
        return false;

    QGraphicsScene *scene(item->scene());
    const QGraphicsScene::ItemIndexMethod original = scene->itemIndexMethod();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->removeItem(item);
    scene->setItemIndexMethod(original);
    return true;
}

QVector<InstanceItem *> instanceItemsByPos(QGraphicsScene *scene, const QPointF &scenePos)
{
    QVector<InstanceItem *> items;
    if (scene)
        for (InstanceItem *item : shared::graphicsviewutils::toplevelItems<InstanceItem>(scene))
            if (item->sceneBoundingRect().contains(scenePos))
                items << item;

    return items;
}

bool isHorizontal(const QLineF &line, const qreal verticalTolerance)
{
    return qAbs(line.dy()) < verticalTolerance;
}
bool isHorizontal(const QPointF &p1, const QPointF &p2, const qreal verticalTolerance)
{
    return isHorizontal(QLineF(p1, p2), verticalTolerance);
}
bool isHorizontal(const QVector<QPointF> &twoPoints, const qreal verticalTolerance)
{
    return isHorizontal(twoPoints.first(), twoPoints.last(), verticalTolerance);
}

QRectF framedRect(const QRectF &rect, qreal frameWidth)
{
    if (qFuzzyIsNull(frameWidth))
        return rect;

    const qreal halfWidth = frameWidth / 2.;
    return rect.adjusted(halfWidth, halfWidth, -halfWidth, -halfWidth);
}

}
