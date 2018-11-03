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

#include "common/drawrectinfo.h"

#include <QObject>
#include <QBrush>
#include <QCursor>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPen>

namespace msc {

class AbstractInteractiveObject;
class GripPointsHandler;
class GripPoint : public QGraphicsItem
{
    Q_GADGET
public:
    enum Location {
        GPP_Top = 1,
        GPP_Left = 2,
        GPP_Bottom = 4,
        GPP_Right = 8,
        GPP_Center = 16,
        GPP_TopLeft = 32,
        GPP_BottomLeft = 64,
        GPP_TopRight = 128,
        GPP_BottomRight = 256,
    };
    Q_ENUM(Location)
    Q_DECLARE_FLAGS(Locations, Location)

    GripPoint(Location pos, GripPointsHandler *parent = nullptr,
              bool isAnchor = false);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    static void setSideSize(qreal sz);
    static qreal sideSize();

    static void setBorderWidth(qreal w);
    static qreal borderWidth();

    static void setBorderColor(const QColor &color);
    static QColor borderColor();

    static void setBodyColor(const QColor &color);
    static QColor bodyColor();

    bool isAnchor() const;
    void setIsAnchor(bool is);

    bool isUsed() const;
    void setIsUsed(bool used);

    Location location() const { return m_pos; }
    void updateLayout();

protected:
    static DrawRectInfo m_uiDescr;

    AbstractInteractiveObject *const m_listener;
    Location m_pos;
    bool m_anchor;

    QRectF m_boundRect;
    QCursor m_cursor;
    QPainterPath m_path;

    bool m_used = true;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(GripPoint::Locations)

} // namespace msc
