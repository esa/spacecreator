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
#include "labeledarrowitem.h"

#include "arrowitem.h"
#include "baseitems/msgidentificationitem.h"
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
    , m_itemText(new MsgIdentificationItem(this))
    , m_itemArrow(new ArrowItem(this))
    , m_selectionHighlighterPen(Qt::green)
{
    m_itemText->setFramed(false);
    m_itemText->setBackgroundColor(Qt::transparent);
    m_itemText->setEditable(true);
    m_itemText->setTextMargin(TextMargin);
    m_itemArrow->setStartSignShown(false);
    m_itemArrow->setEndSignShown(true);
    updateLayout();

    connect(m_itemArrow, &ArrowItem::geometryChanged, this, &LabeledArrowItem::updateLayout);
    connect(m_itemText, &TextItem::keyPressed, this, &LabeledArrowItem::onKeyPressed);
    connect(m_itemText, &TextItem::edited, this, &LabeledArrowItem::onTextEdited);

    m_selectionHighlighterPen.setWidthF(m_itemArrow->bodyPen().widthF() * 2.);
}

void LabeledArrowItem::setText(const QString &text)
{
    const QString currentText = m_itemText->toPlainText();
    if (currentText == text)
        return;

    m_itemText->setPlainText(text);
    onTextEdited(text);
}

QString LabeledArrowItem::text() const
{
    return m_itemText->toPlainText();
}

MsgIdentificationItem *LabeledArrowItem::identificationItem() const
{
    return m_itemText;
}

ArrowItem *LabeledArrowItem::arrow() const
{
    return m_itemArrow;
}

QPointF LabeledArrowItem::startSignPos() const
{
    return mapFromItem(m_itemArrow, m_itemArrow->startSignLocal());
}

QPointF LabeledArrowItem::endSignPos() const
{
    return mapFromItem(m_itemArrow, m_itemArrow->endSignLocal());
}

bool LabeledArrowItem::updateSource(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return arrow()->updateStart(source, anchorPoint, snap);
}

bool LabeledArrowItem::updateTarget(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return arrow()->updateEnd(target, anchorPoint, snap);
}

void LabeledArrowItem::updatePoints(const QPointF &source, const QPointF &target)
{
    QSignalBlocker arrowBlock(m_itemArrow);

    updateSource(arrow()->link()->source()->object(), source, ObjectAnchor::Snap::NoSnap);
    updateTarget(arrow()->link()->target()->object(), target, ObjectAnchor::Snap::NoSnap);

    QSignalBlocker keepSilent(this);
    updateLayout();
}

void LabeledArrowItem::setColor(const QColor &color)
{
    m_itemArrow->setBodyPen(color);
    m_itemArrow->setBodyBrush(color);
    m_itemArrow->setSourcePen(color);
    m_itemArrow->setSourceBrush(color);
    m_itemArrow->setTargetPen(color);
    m_itemArrow->setTargetBrush(color);
}

void LabeledArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (parentItem() && parentItem()->isSelected()) {
        painter->save();
        painter->setPen(m_selectionHighlighterPen);
        painter->drawPath(arrow()->bodyPath());
        painter->restore();
    }
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
    const QVector<QPointF> &trajectory = arrow()->turnPoints();
    if (trajectory.isEmpty())
        return m_itemText->boundingRect().translated(m_itemText->pos());

    const QPointF &tail = trajectory.first();
    const QPointF &head = trajectory.last();

    if (qFuzzyCompare(tail.y(), head.y()) && trajectory.size() == 2) {
        // this takes into account dimensions of the triangle sign
        return childrenBoundingRect();
    }

    // construct a rect where one couple of corners is the tail and head points
    // (in the case when the arrow consits of just a pair of points)
    // or includes all the points set (for more complex arrows).

    QPointF topLeft(tail), bottomRight(head);
    for (const QPointF &currentPoint : trajectory) {
        topLeft.rx() = qMin(topLeft.x(), currentPoint.x());
        topLeft.ry() = qMin(topLeft.y(), currentPoint.y());

        bottomRight.rx() = qMax(bottomRight.x(), currentPoint.x());
        bottomRight.ry() = qMax(bottomRight.y(), currentPoint.y());
    }

    const QRectF arrowRect { topLeft, bottomRight };
    return m_itemText->boundingRect().translated(m_itemText->pos()).united(arrowRect);
}

void LabeledArrowItem::updateLayout()
{
    const QRectF arrowRect(m_itemArrow->boundingRect());
    m_itemText->adjustSize();
    const QRectF textRectCurrent(m_itemText->boundingRect().translated(m_itemText->pos()));
    QRectF textRect(textRectCurrent);

    const QLineF axis(startSignPos(), endSignPos());
    textRect.moveCenter(utils::lineCenter(axis));

    m_itemText->setZValue(m_itemArrow->zValue() + 1);

    const QPointF &arrowCenter(arrowRect.center());
    textRect.moveBottom(arrowCenter.y() - ArrowSign::ARROW_HEIGHT / 2 - TextMargin);

    const QPointF delta = textRect.center() - textRectCurrent.center();
    m_itemText->moveBy(delta.x(), delta.y());
    prepareGeometryChange();

    Q_EMIT layoutChanged();
}

void LabeledArrowItem::onTextEdited(const QString &text)
{
    m_itemText->adjustSize();
    updateLayout();

    Q_EMIT textEdited(text);
}

void LabeledArrowItem::setDashed(bool dashed)
{
    QPen linePen = m_itemArrow->bodyPen();
    linePen.setStyle(dashed ? Qt::DashLine : Qt::SolidLine);
    m_itemArrow->setBodyPen(linePen);
}

void LabeledArrowItem::enableEditMode()
{
    m_itemText->enableEditMode();
}

void LabeledArrowItem::onKeyPressed()
{
    {
        QSignalBlocker suppressLayoutUpdated(this);
        updateLayout();
    }
    Q_EMIT textChanged();
}

} // ns msc
