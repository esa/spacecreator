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

#include "arrowitem.h"
#include "interactiveobject.h"
#include "textitem.h"

#include <QPainter>
#include <QGraphicsTextItem>

#include <QDebug>
#define LOG qDebug() << Q_FUNC_INFO

namespace msc {

ArrowItem::ArrowItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
    , m_txtItem(new TextItem(this))
{
}

void ArrowItem::setPointFrom(const QPointF &from)
{
    QLineF l = line();
    if (l.p1() != from) {
        l.setP1(from);
        setLine(l);
        updateArrows();
    }
}

QPointF ArrowItem::pointFrom() const
{
    return line().p1();
}

void ArrowItem::setPointTo(const QPointF &to)
{
    QLineF l = line();
    if (l.p2() != to) {
        l.setP2(to);
        setLine(l);
        updateArrows();
    }
}

QPointF ArrowItem::pointTo() const
{
    return line().p2();
}

void ArrowItem::setFromArrowVisible(bool visible)
{
    if (m_fromShown == visible)
        return;

    m_fromShown = visible;
    update();
}

void ArrowItem::setToArrowVisible(bool visible)
{
    if (m_toShown == visible)
        return;

    m_toShown = visible;
    update();
}

void ArrowItem::setText(const QString &txt)
{
    if (m_txtItem->toPlainText() == txt)
        return;

    m_txtItem->setPlainText(txt);
    m_txtItem->adjustSize();

    updateArrows();
}

QString ArrowItem::text() const
{
    return m_txtItem->toPlainText();
}

QPainterPath ArrowItem::lineShape(const QLineF &line, qreal span)
{
    QPainterPath res;

    QLineF normalSpan(line.normalVector());
    normalSpan.setLength(span);

    const QPointF delta = line.p1() - normalSpan.p2();
    const QLineF dec(line.translated(delta));
    const QLineF inc(line.translated(-delta));

    res.moveTo(dec.p1());
    res.lineTo(dec.p2());
    res.lineTo(inc.p2());
    res.lineTo(inc.p1());

    return res;
}

void ArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    {
        // draw thin text frame:
        painter->save();
        static const qreal penWidth = 0.5;
        QPen pen = painter->pen();
        pen.setWidthF(penWidth);
        painter->setPen(pen);
        painter->drawRect(m_txtItem->boundingRect().translated(m_txtItem->pos()).adjusted(penWidth, penWidth, -penWidth, -penWidth));
        painter->restore();
    }

    painter->save();
    QPen pen(Qt::black);
    pen.setWidthF(1.5);
    QBrush brush(Qt::black);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);

    QGraphicsLineItem::paint(painter, option, widget);

    auto markOrphan = [](QPainter *painter, bool orphan) {
        QBrush b(painter->brush());
        QColor color = b.color();
        color.setAlphaF(orphan ? 0.5 : 1.);
        b.setColor(color);
        painter->setBrush(b);
    };

    if (m_fromShown) {
        painter->save();

        markOrphan(painter, m_noInstanceFrom);

        painter->translate(line().p1());
        painter->rotate(-line().angle());
        painter->translate(-line().p1());

        painter->drawPath(m_fromArrow);
        painter->restore();
    }
    if (m_toShown) {
        painter->save();

        markOrphan(painter, m_noInstanceTo);

        painter->translate(line().p2());
        painter->rotate(-line().angle());
        painter->translate(-line().p2());

        painter->drawPath(m_toArrow);
        painter->restore();
    }

    painter->restore();
}

QPainterPath ArrowItem::shape() const
{
    QPainterPath res;
    res.addPath(lineShape(line(), InteractiveObject::SPAN));
    return res;
}

QRectF ArrowItem::boundingRect() const
{
    return m_boundingRect;
}

void ArrowItem::updateBounding()
{
    const QRectF textRect = m_txtItem->boundingRect().translated(m_txtItem->pos());
    const QRectF lineRect = QGraphicsLineItem::boundingRect();

    const qreal minHeight = textRect.height() + ARROW_HEIGHT;
    // NOTE: cast is used here to fix "coverage" build on the CI server:
    const qreal minWidth = qMax(static_cast<double>(DEFAULT_WIDTH), qMax(lineRect.width(), textRect.width()));

    m_boundingRect = lineRect.united(textRect).translated(pos());
    m_boundingRect.setWidth(qMax(minWidth, m_boundingRect.width()));
    m_boundingRect.setHeight(qMax(minHeight, m_boundingRect.height()));
}

void ArrowItem::updateArrows()
{
    auto createArrow = [](const QPointF &hit, bool isLeft) {
        QPointF pntFrom(hit);
        QPainterPath arrow;
        arrow.moveTo(pntFrom);
        pntFrom.rx() += ARROW_WIDTH * (isLeft ? 1. : -1.);
        pntFrom.ry() -= ARROW_HEIGHT / 2.;
        arrow.lineTo(pntFrom);
        pntFrom.ry() += ARROW_HEIGHT;
        arrow.lineTo(pntFrom);
        arrow.closeSubpath();
        return arrow;
    };

    const QPointF pntTo(pointTo());
    m_fromArrow = createArrow(pointFrom(), true);
    m_toArrow = createArrow(pntTo, false);

    const QRectF lineBounds = QGraphicsLineItem::boundingRect();
    const QRectF textBounds = m_txtItem->boundingRect().translated(m_txtItem->pos());
    const QPointF delta = lineBounds.center() - textBounds.center();
    const bool textAboveLine = lineBounds.height() <= textBounds.height() && lineBounds.width() <= textBounds.width();
    m_txtItem->moveBy(delta.x(), textAboveLine ? lineBounds.top() - textBounds.bottom() : delta.y());

    prepareGeometryChange();
    updateBounding();
    update();
}

void ArrowItem::setOrphanFrom(bool orphan)
{
    if (m_noInstanceFrom == orphan)
        return;

    m_noInstanceFrom = orphan;
    update();
}

bool ArrowItem::isOrphanFrom() const
{
    return m_noInstanceFrom;
}

void ArrowItem::setOrphanTo(bool orphan)
{
    if (m_noInstanceTo == orphan)
        return;

    m_noInstanceTo = orphan;
    update();
}

bool ArrowItem::isOrphanTo() const
{
    return m_noInstanceTo;
}

} // ns msc
