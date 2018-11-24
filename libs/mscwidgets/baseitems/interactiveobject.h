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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "grippoint.h"

#include <QGraphicsObject>

namespace msc {

class GripPointsHandler;
class HighlightRectItem;
class InteractiveObject : public QGraphicsObject
{
    Q_OBJECT
public:
    InteractiveObject(QGraphicsItem *parent = nullptr);

    // QGraphicsItem interface
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    bool isHovered() const;

    void highlightConnected();
    void highlightDisconnected();

Q_SIGNALS:
    void relocated(const QPointF &from, const QPointF &to) const;

protected Q_SLOTS:
    void gripPointMoved(GripPoint::Location pos, const QPointF &from, const QPointF &to);

protected:
    GripPointsHandler *m_gripPoints;
    HighlightRectItem *m_highlighter;
    QRectF m_boundingRect;
    bool m_hovered = false;
    qreal m_storedZ = 0.;
    QPointF m_prevPos;

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

    virtual void handleGripPointMovement(GripPoint *gp, const QPointF &from, const QPointF &to);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) = 0;
    virtual void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) = 0;

    virtual void updateGripPoints();
};

} // namespace msc
