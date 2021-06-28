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

#include <QGraphicsObject>
#include <QPointer>

class QGraphicsPathItem;
class QPropertyAnimation;

namespace shared {
namespace ui {

/*!
 * \class HighlightRectItem
 * \brief Helper class for graphics items to support animated color change.
 */
class HighlightRectItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit HighlightRectItem(QGraphicsItem *parent);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setPath(const QPainterPath &path);
    void setRect(const QRectF &rect);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);

    void highlight(int duration = 250);

Q_SIGNALS:
    void highlighted();

private:
    QGraphicsPathItem *m_pathItem = nullptr;
    QPointer<QPropertyAnimation> m_lastAnimation;
};

}
}
