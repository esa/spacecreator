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

#include "animation.h"

#include <QGraphicsRectItem>
#include <QPropertyAnimation>

namespace shared {
namespace ui {

HighlightRectItem::HighlightRectItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_pathItem(new QGraphicsPathItem(this))
{
}

void HighlightRectItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
}

QRectF HighlightRectItem::boundingRect() const
{
    return m_pathItem->boundingRect();
}

QPainterPath HighlightRectItem::shape() const
{
    return m_pathItem->shape();
}

void HighlightRectItem::setPath(const QPainterPath &path)
{
    return m_pathItem->setPath(path);
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
    const qreal from = 1.;
    const qreal to = 0.;
    const int duration = 250;

    show();

    delete m_lastAnimation.data();

    m_lastAnimation = utils::Animation::createLinearAnimation(this, "opacity", from, to, duration);
    connect(m_lastAnimation, &QPropertyAnimation::finished, this, [this]() {
        hide();
        Q_EMIT highlighted();
    });
    m_lastAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

}
}
