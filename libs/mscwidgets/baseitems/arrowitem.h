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

#include "objectslinkitem.h"
#include "common/arrowsign.h"
#include "common/objectanchor.h"

namespace msc {

class ArrowItem : public ObjectsLinkItem
{
    Q_OBJECT
public:
    static constexpr qreal DEFAULT_WIDTH = 100.0;

    ArrowItem(QGraphicsItem *parent);
    void buildLayout() override;
    QPainterPath bodyPath() const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    QPointF startSignLocal() const;
    QPointF endSignLocal() const;

    QPointF makeArrow(InteractiveObject *source, const QPointF &sourceAnchorPoint,
                      InteractiveObject *target, const QPointF &targetAnchorPoint);

    void updateLine(const QLineF &to);

    bool updateStart(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap);
    bool updateEnd(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap);

protected:
    PairOf<ArrowSign> m_arrowHeads;
    QPainterPath m_bodyPath;

    void drawStartSign(QPainter *painter) override;
    void drawEndSign(QPainter *painter) override;

private:
    QPointF pathPoint(int num) const;
    bool updateAnchor(ObjectAnchor *anchor, InteractiveObject *anchorObject,
                      const QPointF &anchorPoint, ObjectAnchor::Snap snap);
};

} // ns msc
