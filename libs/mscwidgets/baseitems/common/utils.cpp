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
#include "utils.h"
#include "baseitems/interactiveobject.h"
#include "instanceitem.h"

#include <QtGlobal>
#include <QPropertyAnimation>

namespace msc {
namespace utils {

/*!
  \namespace msc::utils
  \brief Collection of utils for internal use.

  \inmodule MscWidgets

*/

QPainterPath lineShape(const QLineF &line, qreal span)
{
    QPainterPath result;

    QLineF normalSpan(line.normalVector());
    normalSpan.setLength(span);

    const QPointF delta = line.p1() - normalSpan.p2();
    const QLineF dec(line.translated(delta));
    const QLineF inc(line.translated(-delta));

    result.moveTo(dec.p1());
    result.lineTo(dec.p2());
    result.lineTo(inc.p2());
    result.lineTo(inc.p1());

    return result;
}

QPointF lineCenter(const QLineF &line)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    return line.center();
#else
    return 0.5 * line.p1() + 0.5 * line.p2();
#endif
}

QPointF pointFromPath(const QPainterPath &path, int num)
{
    QPointF result;
    if (!path.isEmpty()) {
        num = qBound(0, num, path.elementCount() - 1);
        result = path.elementAt(num);
    }
    return result;
}

QPropertyAnimation *createLinearAnimation(QObject *target,
                                          const QString &propName,
                                          const QVariant &from,
                                          const QVariant &to,
                                          const int durationMs)
{
    QPropertyAnimation *anim = new QPropertyAnimation(target, propName.toUtf8());
    anim->setDuration(durationMs);
    anim->setEasingCurve(QEasingCurve::Linear);

    anim->setStartValue(from);
    anim->setEndValue(to);

    return anim;
}

QPointF snapToPointByX(const QPointF &target, const QPointF &source, qreal tolerance)
{
    QPointF result(source);
    const QLineF delta(source, target);
    if (qAbs(delta.dx()) <= tolerance) {
        result.rx() += delta.dx();
    }
    return result;
}

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

QList<QGraphicsItem *> toplevelItems(QGraphicsScene *scene)
{
    QList<QGraphicsItem *> items;
    if (scene) {
        for (QGraphicsItem *item : scene->items())
            if (!item->parentItem())
                items.append(item);
    }
    return items;
}

} // ns utils
} // ns msc
