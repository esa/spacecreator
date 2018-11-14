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

#include "arrowitem.h"
#include "common/objectslink.h"
#include "common/utils.h"

#include <QPainter>

namespace msc {

/*!
  \class msc::ArrowItem
  \brief Arrow pointing to the "end" ObjectAnchor instance.

  \inmodule MscWidgets

*/

ArrowItem::ArrowItem(QGraphicsItem *parent)
    : ObjectsLinkItem(parent)
    , m_arrowHeads({ ArrowSign::createArrowLeft() }, { ArrowSign::createArrowRight() })
{
}

void ArrowItem::drawStartSign(QPainter *painter)
{
    if (m_bodyPath.isEmpty())
        return;

    painter->save();

    const QPointF p1(m_bodyPath.elementAt(0));
    const QPointF p2(m_bodyPath.elementAt(m_bodyPath.elementCount() - 1));

    painter->translate(p1.x(), p1.y());
    painter->rotate(-QLineF(p1, p2).angle());
    painter->translate(-p1.x(), -p1.y());

    ObjectsLinkItem::drawStartSign(painter);

    painter->restore();
}

void ArrowItem::drawEndSign(QPainter *painter)
{
    if (m_bodyPath.isEmpty())
        return;

    painter->save();

    const QPointF p1(m_bodyPath.elementAt(0));
    const QPointF p2(m_bodyPath.elementAt(m_bodyPath.elementCount() - 1));

    painter->translate(p2.x(), p2.y());
    painter->rotate(-QLineF(p1, p2).angle());
    painter->translate(-p2.x(), -p2.y());

    ObjectsLinkItem::drawEndSign(painter);

    painter->restore();
}

bool ArrowItem::updateStart(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return updateAnchor(link()->source(), source, anchorPoint, snap);
}

bool ArrowItem::updateEnd(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return updateAnchor(link()->target(), target, anchorPoint, snap);
}

bool ArrowItem::updateAnchor(ObjectAnchor *anchor, InteractiveObject *anchorObject,
                             const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    // keep silent to avoid rebuilding the layout,
    // otherwise it will recenter the new line to (0,0)
    QSignalBlocker anchorLinkSilent(link());
    QSignalBlocker anchorSilent(anchor);

    const bool updated = anchor->replace(anchorObject, anchorPoint, snap);
    if (updated) {
        const QPointF currStartLocal(mapFromScene(link()->source()->point()));
        const QPointF currEndLolcal(mapFromScene(link()->target()->point()));
        const QLineF line(currStartLocal, currEndLolcal);
        updateLine(line);
    }
    return updated;
}

void ArrowItem::buildLayout()
{
    const QPointF currStart(link()->source()->point());
    const QPointF currStartLocal(mapFromScene(currStart));
    const QPointF currEnd(link()->target()->point());
    const QPointF currEndLolcal(mapFromScene(currEnd));

    QLineF line(currStartLocal, currEndLolcal);
    line.translate(-utils::lineCenter(line));

    updateLine(line);
}

void ArrowItem::updateLine(const QLineF &newLine)
{
    prepareGeometryChange();

    m_bodyPath = QPainterPath();
    m_bodyPath.moveTo(newLine.p1());
    m_bodyPath.lineTo(newLine.p2());

    m_arrowHeads.Source.pointTo(newLine.p1());
    m_arrowHeads.Target.pointTo(newLine.p2());

    m_symbols.Source = m_arrowHeads.Source.path();
    m_symbols.Target = m_arrowHeads.Target.path();

    Q_EMIT geometryChanged(boundingRect());
}

QPainterPath ArrowItem::bodyPath() const
{
    return m_bodyPath;
}

QRectF ArrowItem::boundingRect() const
{
    return ArrowItem::shape().boundingRect();
}

QPointF ArrowItem::startSignLocal() const
{
    return pathPoint(0);
}

QPointF ArrowItem::endSignLocal() const
{
    return pathPoint(m_bodyPath.elementCount() - 1);
}

QPointF ArrowItem::makeArrow(InteractiveObject *source, const QPointF &sourceAnchorPoint,
                             InteractiveObject *target, const QPointF &targetAnchorPoint)
{
    const QPointF result(link()->makeLink(source, sourceAnchorPoint, target, targetAnchorPoint));
    rebuildLayout();
    return result;
}

QPointF ArrowItem::pathPoint(int num) const
{
    return utils::pointFromPath(m_bodyPath, num);
}

QPainterPath ArrowItem::shape() const
{
    QPainterPath result;

    const int pointsCount = m_bodyPath.elementCount();
    int point(0);
    while (point < pointsCount) {
        const QPointF p1(m_bodyPath.elementAt(point));
        if (++point < pointsCount) {
            const QPointF p2(m_bodyPath.elementAt(point));
            const QLineF line(p1, p2);
            result.addPath(ObjectsLinkItem::hoverableLine(line));
        } else {
            break;
        }
    }

    if (result.isEmpty())
        result = m_bodyPath;

    return result;
}

} // ns msc
