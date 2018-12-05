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
#include <QTextCursor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

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
    setTextInteractionFlags(Qt::NoTextInteraction);
}

QBrush TextItem::background() const
{
    if (m_gradientUsed) {
        const QRectF &bounds = boundingRect();
        QLinearGradient gradient(m_gradient);
        gradient.setStart(bounds.topLeft());
        gradient.setFinalStop(bounds.bottomRight());
        return m_gradient;
    }
    return m_bgrColor;
}

void TextItem::setBackgroundColor(const QColor &color)
{
    if (m_bgrColor == color)
        return;

    m_gradientUsed = false;

    m_bgrColor = color;
    update();
}

void TextItem::setBackgroundGradient(const QLinearGradient &gradient)
{
    if (gradient == m_gradient)
        return;

    m_gradientUsed = true;

    m_gradient = gradient;
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

    painter->fillRect(body, background());

    if (framed())
        painter->drawRect(body);

    painter->restore();

    QGraphicsTextItem::paint(painter, option, widget);
}

bool TextItem::isEditable() const
{
    return m_editable;
}

void TextItem::setEditable(bool editable)
{
    if (editable == m_editable)
        return;

    m_editable = editable;

    if (m_editable)
        setTextInteractionFlags(Qt::TextEditorInteraction | Qt::TextEditable);
    else
        setTextInteractionFlags(Qt::NoTextInteraction);
}

void TextItem::focusOutEvent(QFocusEvent *event)
{
    QGraphicsTextItem::focusOutEvent(event);

    if (!isEditable())
        return;

    const QString newText(toPlainText());
    if (m_prevText != newText) {
        Q_EMIT edited(newText);
    }

    selectText(false);

    m_prevText.clear();
}

void TextItem::keyPressEvent(QKeyEvent *event)
{
    if (isEditable()) {
        switch (event->key()) {
        case Qt::Key_Escape: {
            setPlainText(m_prevText);
            clearFocus();
            return;
        }
        case Qt::Key_Return:
        case Qt::Key_Enter: {
            if (event->modifiers() == Qt::NoModifier) {
                clearFocus();
                return;
            }
            break;
        }
        }
    }

    QGraphicsTextItem::keyPressEvent(event);
}

void TextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);

    if (!isEditable())
        return;

    if (m_prevText.isEmpty()) {
        m_prevText = toPlainText();
        selectText(true);
    }
}

void TextItem::selectText(bool select)
{
    QTextCursor txtCursor = textCursor();
    if (select)
        txtCursor.select(QTextCursor::Document);
    else if (txtCursor.hasSelection())
        txtCursor.clearSelection();
    setTextCursor(txtCursor);
}

} // namespace msc
