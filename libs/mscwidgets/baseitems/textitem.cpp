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

#include "textitem.h"

#include <QPainter>
#include <QTextDocument>

namespace msc {

/*!
  \class msc::TextItem
  \brief Text holder with customizable alignment, background and frame.

  \inmodule MscWidgets

*/

TextItem::TextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setTextAllignment(Qt::AlignCenter);
}

QColor TextItem::backgroundColor() const
{
    return m_bgrColor;
}

void TextItem::setBackgroundColor(const QColor &color)
{
    if (m_bgrColor == color)
        return;

    m_bgrColor = color;
    update();
}

QColor TextItem::frameColor() const
{
    return m_frameColor;
}

void TextItem::setFrameColor(const QColor &color)
{
    if (color == m_frameColor)
        return;

    m_frameColor = color;
    update();
}

qreal TextItem::frameWidth() const
{
    return m_frameWidth;
}

void TextItem::setFrameWidth(qreal w)
{
    if (qFuzzyCompare(m_frameWidth, w))
        return;

    m_frameWidth = w;
}

Qt::Alignment TextItem::textAlignment() const
{
    return document()->defaultTextOption().alignment();
}

void TextItem::setTextAllignment(Qt::Alignment alignment)
{
    QTextOption txtOpt = document()->defaultTextOption();
    txtOpt.setAlignment(alignment);
    document()->setDefaultTextOption(txtOpt);
}

bool TextItem::framed() const
{
    return m_showFrame;
}

void TextItem::setFramed(bool to)
{
    if (to == m_showFrame)
        return;

    m_showFrame = to;
    update();
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if (framed()) {
        QPen pen(painter->pen());
        pen.setWidthF(frameWidth());
        pen.setColor(frameColor());
        painter->setPen(pen);
    }

    const qreal shift = painter->pen().widthF();
    const QRectF body = boundingRect().adjusted(shift, shift, -shift, -shift);

    painter->fillRect(body, backgroundColor());

    if (framed())
        painter->drawRect(body);

    painter->restore();

    QGraphicsTextItem::paint(painter, option, widget);
}

} // namespace msc
