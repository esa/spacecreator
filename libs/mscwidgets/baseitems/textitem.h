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

#include <QGraphicsTextItem>

namespace msc {

class TextItem : public QGraphicsTextItem
{
public:
    TextItem(QGraphicsItem *parent = nullptr);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor frameColor() const;
    void setFrameColor(const QColor &color);

    qreal frameWidth() const;
    void setFrameWidth(qreal w);

    Qt::Alignment textAlignment() const;
    void setTextAllignment(Qt::Alignment alignment);

    bool framed() const;
    void setFramed(bool to);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor m_bgrColor = Qt::white;
    QColor m_frameColor = Qt::black;
    qreal m_frameWidth = 0.5;
    bool m_showFrame = false;
};

} // ns msc
