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

#include "arrowitem.h"

#include "common/coordinatesconverter.h"
#include "common/objectslink.h"
#include "common/utils.h"

#include <QGraphicsView>
#include <QPainter>

namespace msc {

/*!
  \class msc::ArrowItem
  \brief Arrow pointing to the "end" ObjectAnchor instance.

  \inmodule MscWidgets

*/

qreal ArrowItem::defaultWidth()
{
    static constexpr int defaultWidthCif { 200 };
    static qreal defaultWidthScene { 0. };
    if (qFuzzyIsNull(defaultWidthScene)) {
        defaultWidthScene = utils::CoordinatesConverter::widthInScene(defaultWidthCif);
    }
    return defaultWidthScene;
}

ArrowItem::ArrowItem(QGraphicsItem *parent)
    : ObjectsLinkItem(parent)
    , m_arrowHeads({ ArrowSign::createArrowLeft() }, { ArrowSign::createArrowRight() })
{
}

void ArrowItem::drawBody(QPainter *painter)
{
    painter->save();
    painter->setPen(m_bodyPen);
    painter->strokePath(m_bodyPath, m_bodyPen);
    painter->restore();
}

void ArrowItem::drawStartSign(QPainter *painter)
{
    if (m_bodyPath.isEmpty())
        return;

    ObjectsLinkItem::drawStartSign(painter);
}

void ArrowItem::drawEndSign(QPainter *painter)
{
    if (m_bodyPath.isEmpty())
        return;

    ObjectsLinkItem::drawEndSign(painter);
}

bool ArrowItem::updateStart(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return updateAnchor(link()->source(), source, anchorPoint, snap);
}

bool ArrowItem::updateEnd(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return updateAnchor(link()->target(), target, anchorPoint, snap);
}

bool ArrowItem::updateAnchor(
        ObjectAnchor *anchor, InteractiveObject *anchorObject, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
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

void ArrowItem::addTurnPoint(const QPointF &scenePoint)
{
    m_polyLine.append(mapFromScene(scenePoint));
    updatePath();
}

void ArrowItem::setTurnPoints(const QVector<QPointF> &scenePoints)
{
    QVector<QPointF> newPolyLine;
    newPolyLine.reserve(scenePoints.size());
    for (const QPointF &scenePoint : scenePoints)
        newPolyLine << mapFromScene(scenePoint);

    if (m_polyLine != newPolyLine) {
        m_polyLine = newPolyLine;
        QSignalBlocker silently(this);
        updatePath();
    }
}

/*!
  \brief ArrowItem::turnPointsScene
  \return A collection of points in local coordinates that represent the whole arrow.
  This includes start, end and subline edges in between, thus a valid collection
  should contain at least two entries - the start and end points.
*/
QVector<QPointF> ArrowItem::turnPoints() const
{
    return m_polyLine;
}

void ArrowItem::updateLine(const QLineF &newLine)
{
    if (m_polyLine.isEmpty()) {
        m_polyLine = { newLine.p1(), newLine.p2() };
    } else {
        if (m_polyLine.size() == 2)
            m_polyLine.replace(0, newLine.p1());
        m_polyLine.replace(m_polyLine.size() - 1, newLine.p2());
    }

    updatePath();
}

void ArrowItem::updatePath()
{
    prepareGeometryChange();

    m_bodyPath = QPainterPath();
    const int pointsCount = m_polyLine.size();
    if (pointsCount) {
        m_bodyPath.moveTo(m_polyLine.first());
        int point(1);
        while (point < pointsCount) {
            m_bodyPath.lineTo(m_polyLine.at(point++));
        }

        m_arrowHeads.Source.pointTo(m_polyLine.first(), QLineF(m_polyLine.first(), m_polyLine.last()).angle());
        m_arrowHeads.Target.pointTo(m_polyLine.last(), QLineF(m_polyLine.last(), m_polyLine.first()).angle());
    }
    m_symbols.Source = m_arrowHeads.Source.path();
    m_symbols.Target = m_arrowHeads.Target.path();

    m_bounds = m_bodyPath.boundingRect();
    if (m_symbolShown.Source)
        m_bounds |= m_symbols.Source.boundingRect();
    if (m_symbolShown.Target)
        m_bounds |= m_symbols.Target.boundingRect();

    Q_EMIT geometryChanged(boundingRect());
}

QPainterPath ArrowItem::bodyPath() const
{
    return m_bodyPath;
}

QRectF ArrowItem::boundingRect() const
{
    return m_bounds;
}

QPointF ArrowItem::startSignLocal() const
{
    return pathPoint(0);
}

QPointF ArrowItem::endSignLocal() const
{
    return pathPoint(m_bodyPath.elementCount() - 1);
}

QPointF ArrowItem::makeArrow(InteractiveObject *source, const QPointF &sourceAnchorPoint, InteractiveObject *target,
        const QPointF &targetAnchorPoint)
{
    const QPointF result(link()->makeLink(source, sourceAnchorPoint, target, targetAnchorPoint));
    rebuildLayout();
    return result;
}

QPointF ArrowItem::pathPoint(int num) const
{
    QPointF result;
    if (!m_polyLine.isEmpty()) {
        num = qBound(0, num, m_polyLine.size() - 1);
        result = m_polyLine.at(num);
    }
    return result;
}

QPainterPath ArrowItem::shape() const
{
    QPainterPath result = createShape(utils::LineHoverTolerance);
    if (result.isEmpty())
        result = m_bodyPath;

    return result;
}

QPainterPath ArrowItem::createShape(qreal lineWidth) const
{
    QPainterPath result;

    const int pointsCount = m_bodyPath.elementCount();
    int point(0);
    while (point < pointsCount) {
        const QPointF p1(m_bodyPath.elementAt(point));
        if (++point < pointsCount) {
            const QPointF p2(m_bodyPath.elementAt(point));
            const QLineF line(p1, p2);
            result.addPath(ObjectsLinkItem::hoverableLine(line, lineWidth));
        } else {
            break;
        }
    }

    return result;
}

} // ns msc
