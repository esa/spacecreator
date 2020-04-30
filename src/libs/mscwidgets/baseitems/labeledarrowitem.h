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
#pragma once

#include "baseitems/common/objectanchor.h"

#include <QGraphicsObject>

namespace msc {

class ArrowItem;
class InteractiveObject;
class MsgIdentificationItem;

class LabeledArrowItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit LabeledArrowItem(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    QRectF textBoundingRect() const;

    QString text() const;
    MsgIdentificationItem *identificationItem() const;
    ArrowItem *arrow() const;

    QPointF startSignPos() const;
    QPointF endSignPos() const;

    bool updateSource(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap);
    bool updateTarget(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap);
    void updatePoints(const QPointF &source, const QPointF &target);

    void setColor(const QColor &color);

    void setDashed(bool dashed);

    void enableEditMode();

Q_SIGNALS:
    void layoutChanged() const;
    void textEdited(const QString &text);
    void textChanged();

public Q_SLOTS:
    void setText(const QString &text);
    void onTextEdited(const QString &text);

protected Q_SLOTS:
    void updateLayout();
    void onTextChanged();

private:
    MsgIdentificationItem *m_itemText;
    ArrowItem *m_itemArrow;
    QPainterPath m_shape;
    QPen m_selectionHighlighterPen;

    static constexpr qreal TextMargin = 1.;
};

} // ns msc
