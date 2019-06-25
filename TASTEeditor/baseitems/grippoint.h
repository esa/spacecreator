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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "common/drawrectinfo.h"

#include <QBrush>
#include <QCursor>
#include <QGraphicsItem>
#include <QObject>
#include <QPainterPath>
#include <QPen>

namespace taste3 {

class AbstractInteractiveObject;
class GripPointsHandler;
class GripPoint : public QGraphicsItem
{
    Q_GADGET
public:
    enum Location
    {
        Top,
        Left,
        Bottom,
        Right,
        Center,
        TopLeft,
        BottomLeft,
        TopRight,
        BottomRight
    };
    Q_ENUM(Location)
    typedef QSet<GripPoint::Location> Locations;

    enum class GripType
    {
        Mover,
        Resizer
    };

    GripPoint(Location pos, GripPointsHandler *parent = nullptr,
              GripPoint::GripType gpType = GripPoint::GripType::Resizer);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static void setSideSize(qreal sz);
    static qreal sideSize();

    static void setBorderWidth(qreal w);
    static qreal borderWidth();

    static void setBorderColor(const QColor &color);
    static QColor borderColor();

    static void setBodyColor(const QColor &color);
    static QColor bodyColor();

    GripPoint::GripType gripType() const;
    void setGripType(GripPoint::GripType gpType);
    bool isMover() const;

    bool isUsed() const;
    void setIsUsed(bool used);

    Location location() const { return m_location; }
    void updateLayout();

protected:
    static DrawRectInfo m_uiDescr;

    AbstractInteractiveObject *const m_listener;
    Location m_location;
    GripPoint::GripType m_type;

    QRectF m_boundRect;
    QCursor m_cursor;
    QPainterPath m_path;

    bool m_used = true;

    QPointF m_posStart, m_posFinish;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

} // namespace taste3
