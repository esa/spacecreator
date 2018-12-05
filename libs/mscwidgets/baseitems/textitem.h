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

#pragma once

#include <QGraphicsTextItem>
#include <QLinearGradient>

namespace msc {

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    TextItem(QGraphicsItem *parent = nullptr);

    QBrush background() const;
    void setBackgroundColor(const QColor &color);
    void setBackgroundGradient(const QLinearGradient &color);

    QColor frameColor() const;
    void setFrameColor(const QColor &color);

    qreal frameWidth() const;
    void setFrameWidth(qreal w);

    Qt::Alignment textAlignment() const;
    void setTextAllignment(Qt::Alignment alignment);

    bool framed() const;
    void setFramed(bool to);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isEditable() const;
    void setEditable(bool editable);

Q_SIGNALS:
    void edited(const QString &newText);

protected:
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QColor m_bgrColor = Qt::white;
    QLinearGradient m_gradient;
    QColor m_frameColor = Qt::black;
    qreal m_frameWidth = 0.5;
    bool m_showFrame = false;
    bool m_gradientUsed = false;
    bool m_editable = false;
    QString m_prevText;

    void selectText(bool select);
};

} // ns msc
