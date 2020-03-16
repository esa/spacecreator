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

#include "highlightrectitem.h"

#include "utils.h"

#include <QGraphicsRectItem>
#include <QPropertyAnimation>

namespace taste3 {

HighlightRectItem::HighlightRectItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_pathItem(new QGraphicsPathItem(this))
{
}

HighlightRectItem::~HighlightRectItem()
{
    clearAnimation();
}

void HighlightRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF HighlightRectItem::boundingRect() const
{
    return m_pathItem->boundingRect();
}

void HighlightRectItem::setRect(const QRectF &rect)
{
    QPainterPath pp;
    pp.addRect(rect);
    m_pathItem->setPath(pp);
}

void HighlightRectItem::setPen(const QPen &pen)
{
    m_pathItem->setPen(pen);
}

void HighlightRectItem::setBrush(const QBrush &brush)
{
    m_pathItem->setBrush(brush);
}

void HighlightRectItem::highlight()
{
    static constexpr qreal from = 1.;
    static constexpr qreal to = 0.;
    static constexpr int duration = 250;

    show();

    clearAnimation();

    m_lastAnimation = utils::createLinearAnimation(this, "opacity", from, to, duration);
    connect(m_lastAnimation, &QPropertyAnimation::finished, this, [this]() {
        hide();
        Q_EMIT highlighted();
    });
    m_lastAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void HighlightRectItem::clearAnimation()
{
    if (m_lastAnimation) {
        m_lastAnimation->stop();
        delete m_lastAnimation;
    }
}

QPainterPath HighlightRectItem::shape() const
{
    return m_pathItem->shape();
}

void HighlightRectItem::setPath(const QPainterPath &path)
{
    return m_pathItem->setPath(path);
}

} // ns taste3
