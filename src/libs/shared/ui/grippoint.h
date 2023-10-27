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

#include <QCursor>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QSet>

namespace shared {
namespace ui {

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
        BottomRight,
        Absolute // Used for connection endpoints where only one GripPoint is needed.
    };
    Q_ENUM(Location)
    typedef QSet<GripPoint::Location> Locations;

    enum class GripType
    {
        Mover,
        Resizer
    };

    enum
    {
        Type = UserType + std::numeric_limits<uchar>::max()
    };

    explicit GripPoint(Location pos, GripPointsHandler *parent = nullptr);

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int type() const override { return Type; }

    static void setSideSize(qreal sz);
    static qreal sideSize();

    static void setBorderWidth(qreal w);
    static qreal borderWidth();

    static void setBorderColor(const QColor &color);
    static QColor borderColor();

    static void setBodyColor(const QColor &color);
    static QColor bodyColor();

    GripPoint::GripType gripType() const;
    bool isMover() const;

    bool isUsed() const;
    void setIsUsed(bool used);

    Location location() const { return m_location; }

    /**
     * @brief updateLayout updates position and cursor-icon
     */
    void updateLayout();

protected:
    GripPointsHandler *const m_gripPointsHandler;
    Location m_location;

    const QRectF m_boundRect;
    QCursor m_cursor;

    bool m_used = true;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void showHelperLines(bool show);

private:
    QGraphicsLineItem *m_horizontalHelper = nullptr;
    QGraphicsLineItem *m_verticalHelper = nullptr;
};

} // namespace ui
} // namespace shared
