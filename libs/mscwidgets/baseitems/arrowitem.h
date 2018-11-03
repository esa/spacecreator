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

#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QPainterPath>

class QGraphicsTextItem;

namespace msc {
class ArrowItem : public QGraphicsLineItem
{
public:
    static constexpr qreal ARROW_HEIGHT = 10.0;
    static constexpr qreal ARROW_WIDTH = 20.0;
    static constexpr qreal DEFAULT_WIDTH = 100.0;

    ArrowItem(QGraphicsItem *parent = nullptr);

    void setPointFrom(const QPointF &from);
    QPointF pointFrom() const;

    void setPointTo(const QPointF &to);
    QPointF pointTo() const;

    void setFromArrowVisible(bool visible);
    bool isFromArrowVisible() const { return m_fromShown; }
    void setToArrowVisible(bool visible);
    bool isToArrowVisible() const { return m_toShown; }

    void setText(const QString &txt);
    QString text() const;

    static QPainterPath lineShape(const QLineF &line, qreal span);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setOrphanFrom(bool orphan);
    bool isOrphanFrom() const;

    void setOrphanTo(bool orphan);
    bool isOrphanTo() const;

protected:
    QRectF m_boundingRect;
    QGraphicsTextItem *m_txtItem;
    QPainterPath m_fromArrow, m_toArrow;
    bool m_fromShown = false;
    bool m_toShown = true;
    bool m_noInstanceFrom = true;
    bool m_noInstanceTo = true;
    QColor m_fromColor = Qt::black;
    QColor m_toColor = Qt::black;

    void updateArrows();
    void updateBounding();
};

} // ns msc
