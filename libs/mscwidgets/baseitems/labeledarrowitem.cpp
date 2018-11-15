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
#include "labeledarrowitem.h"
#include "textitem.h"
#include "arrowitem.h"
#include "common/objectslink.h"
#include "common/utils.h"

#include <QPainter>

namespace msc {

/*!
  \class msc::LabeledArrowItem
  \brief Source-to-end arrow with a text holder.

  \inmodule MscWidgets

*/

LabeledArrowItem::LabeledArrowItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_itemText(new TextItem(this))
    , m_itemArrow(new ArrowItem(this))
{
    m_itemText->setFramed(true);
    m_itemArrow->setStartSignShown(false);
    m_itemArrow->setEndSignShown(true);
    updateLayout();

    connect(m_itemArrow, &ArrowItem::geometryChanged, this, &LabeledArrowItem::updateLayout);
}

void LabeledArrowItem::setText(const QString &text)
{
    const QString currentText = m_itemText->toPlainText();
    if (currentText == text)
        return;

    m_itemText->setPlainText(text);
    m_itemText->adjustSize();

    updateLayout();
}

QString LabeledArrowItem::text() const
{
    return m_itemText->toPlainText();
}

ArrowItem *LabeledArrowItem::arrow() const
{
    return m_itemArrow;
}

QPointF LabeledArrowItem::arrowHead() const
{
    return mapFromItem(m_itemArrow, m_itemArrow->startSignLocal());
}

QPointF LabeledArrowItem::arrowTail() const
{
    return mapFromItem(m_itemArrow, m_itemArrow->endSignLocal());
}

bool LabeledArrowItem::replaceSource(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return arrow()->updateStart(source, anchorPoint, snap);
}

bool LabeledArrowItem::replaceTarget(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return arrow()->updateEnd(target, anchorPoint, snap);
}

void LabeledArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath LabeledArrowItem::shape() const
{
    QPainterPath arrowShape(m_itemArrow->shape());
    QPainterPath textShape(m_itemText->shape());
    textShape.translate(m_itemText->pos());
    arrowShape.addPath(textShape);
    return arrowShape;
}

QRectF LabeledArrowItem::boundingRect() const
{
    QRectF arrowRect = m_itemArrow->boundingRect();
    QRectF textRect = m_itemText->boundingRect().translated(m_itemText->pos());
    return arrowRect.united(textRect);
}

void LabeledArrowItem::updateLayout()
{
    const QRectF arrowRect(m_itemArrow->boundingRect());
    m_itemText->adjustSize();
    const QRectF textRectCurrent(m_itemText->boundingRect().translated(m_itemText->pos()));
    QRectF textRect(textRectCurrent);

    const QLineF axis(arrowHead(), arrowTail());
    textRect.moveCenter(utils::lineCenter(axis));
    m_itemText->setZValue(m_itemArrow->zValue() + 1);
    const qreal textWidth = textRect.width();
    const qreal lineWidth = arrowRect.width() + 2 * ArrowSign::ARROW_WIDTH - 2 * utils::LineHoverTolerance;
    if (textWidth >= lineWidth) {
        textRect.moveBottom(arrowRect.top() - Span);
        m_itemText->setZValue(m_itemArrow->zValue() - 2);
    }

    const QPointF delta = textRect.center() - textRectCurrent.center();
    m_itemText->moveBy(delta.x(), delta.y());
    prepareGeometryChange();

    Q_EMIT layoutChanged();
}

} // ns msc
