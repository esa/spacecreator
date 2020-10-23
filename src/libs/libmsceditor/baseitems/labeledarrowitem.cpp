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

#include "aadlsystemchecks.h"
#include "arrowitem.h"
#include "baseitems/msgidentificationitem.h"
#include "chartitem.h"
#include "common/mscutils.h"
#include "common/objectslink.h"

#include <QPainter>

namespace msc {

/*!
  \class msc::LabeledArrowItem
  \brief Source-to-end arrow with a text holder.

  \ingroup MscWidgets

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
    connect(m_itemText, &TextItem::textChanged, this, &LabeledArrowItem::onTextChanged);
    connect(m_itemText, &TextItem::edited, this, &LabeledArrowItem::onTextEdited);
    connect(m_itemText, &TextItem::doubleClicked, this, &LabeledArrowItem::doubleClicked);

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
    return m_itemArrow->shape();
}

QRectF LabeledArrowItem::boundingRect() const
{
    const QRectF &arrowBounds = m_itemArrow->boundingRect();
    const QVector<QPointF> &currPoints = m_itemArrow->turnPoints();
    if (currPoints.size() == 2 && isHorizontal(currPoints)) {
        const QRectF &titleBounds = m_itemText->boundingRect().translated(m_itemText->pos());
        const QRectF &titledArrowBounds = arrowBounds | titleBounds;
        return titledArrowBounds.adjusted(0., 0., 0., ArrowSign::ARROW_HEIGHT);
    }
    return arrowBounds;
}

QRectF LabeledArrowItem::textBoundingRect() const
{
    return m_itemText->sceneBoundingRect();
}

void LabeledArrowItem::updateLayout()
{
    const qreal textWidth =
            QFontMetrics(m_itemText->font()).horizontalAdvance(m_itemText->toPlainText()) + TextMargin * 2;
    const QLineF axis = startSignPos().x() < endSignPos().x() ? QLineF { startSignPos(), endSignPos() }
                                                              : QLineF { endSignPos(), startSignPos() };
    const qreal angle = axis.angle();
    const QMarginsF margins = ChartItem::chartMargins();

    qreal margin = 0;
    if (arrow()->link()->source()->object() == nullptr)
        margin = startSignPos().x() < endSignPos().x() ? margins.left() : -margins.right();
    else if (arrow()->link()->target()->object() == nullptr)
        margin = startSignPos().x() > endSignPos().x() ? margins.left() : -margins.right();

    m_itemText->setTextWidth(axis.isNull() ? textWidth : (axis.length() - (margin + TextMargin * 2)));

    QLineF normalVector = axis.normalVector();
    normalVector.setLength(m_itemText->boundingRect().height());
    QLineF destVector;
    destVector.setP1(normalVector.p2());
    destVector.setAngle(angle);
    destVector.setLength((axis.length() - m_itemText->textWidth() + margin) / 2);
    m_itemText->setPos(destVector.p2());
    m_itemText->setRotation(-angle);
    m_itemText->setZValue(m_itemArrow->zValue() + 1);
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

void LabeledArrowItem::setEditable(bool edit)
{
    m_itemText->setEditable(edit);
}

void LabeledArrowItem::onTextChanged()
{
    {
        QSignalBlocker suppressLayoutUpdated(this);
        updateLayout();
    }
    Q_EMIT textChanged();
}

} // ns msc
