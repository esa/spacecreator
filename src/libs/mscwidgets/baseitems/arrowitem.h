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

#include "common/arrowsign.h"
#include "common/objectanchor.h"
#include "objectslinkitem.h"

namespace msc {

class ArrowItem : public ObjectsLinkItem
{
    Q_OBJECT
public:
    static qreal defaultWidth();

    explicit ArrowItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    QPointF startSignLocal() const;
    QPointF endSignLocal() const;

    QPointF makeArrow(InteractiveObject *source, const QPointF &sourceAnchorPoint, InteractiveObject *target,
            const QPointF &targetAnchorPoint);

    bool updateStart(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap);
    bool updateEnd(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap);

    void addTurnPoint(const QPointF &scenePoint);
    void setTurnPoints(const QVector<QPointF> &scenePoints);
    QVector<QPointF> turnPoints() const;

    QPainterPath bodyPath() const override;

protected:
    PairOf<ArrowSign> m_arrowHeads;
    QPainterPath m_bodyPath;

    void drawBody(QPainter *painter) override;
    void drawStartSign(QPainter *painter) override;
    void drawEndSign(QPainter *painter) override;

    void buildLayout() override;
    void updateLine(const QLineF &to);

private:
    QVector<QPointF> m_polyLine;
    QRectF m_bounds;
    QPointF pathPoint(int num) const;
    bool updateAnchor(
            ObjectAnchor *anchor, InteractiveObject *anchorObject, const QPointF &anchorPoint, ObjectAnchor::Snap snap);

    void updatePath();

    QPainterPath createShape(qreal lineWidth) const;

    bool consistentcyCheck() const;
};

} // ns msc
