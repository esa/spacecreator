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

#include "highlightrectitem.h"
#include "utils.h"

#include <QGraphicsRectItem>
#include <QPropertyAnimation>

namespace msc {

/*!
  \class msc::HighlightRectItem
  \brief Just a splashable rect, used as an items visualizer during development, but looks nice :)

  \inmodule MscWidgets

*/

HighlightRectItem::HighlightRectItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_rectItem(new QGraphicsRectItem(this))
{
}

void HighlightRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF HighlightRectItem::boundingRect() const
{
    return m_rectItem->boundingRect();
}

void HighlightRectItem::setRect(const QRectF &rect)
{
    m_rectItem->setRect(rect);
}

void HighlightRectItem::setPen(const QPen &pen)
{
    m_rectItem->setPen(pen);
}

void HighlightRectItem::setBrush(const QBrush &brush)
{
    m_rectItem->setBrush(brush);
}

void HighlightRectItem::highlight()
{
    static constexpr qreal from = 1.;
    static constexpr qreal to = 0.;
    static constexpr int duration = 400;

    show();

    if (QPropertyAnimation *anim =
                utils::createLinearAnimation(this, "opacity", from, to, duration)) {
        anim->setLoopCount(1);
        connect(anim, &QPropertyAnimation::finished, [this]() {
            hide();
            Q_EMIT highlighted();
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

} // ns msc
