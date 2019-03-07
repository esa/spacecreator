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

#include <QGraphicsRectItem>

namespace msc {

class InstanceEndItem : public QGraphicsRectItem
{
public:
    static constexpr qreal EndSymbolHeight = 15.;
    static constexpr qreal StopSymbolHeight = 25.;

    InstanceEndItem(bool stop = false, QGraphicsItem *parent = nullptr);

    bool isStop() const;
    void setStop(bool stop);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void setRect(const QRectF &r);

    qreal height() const;

private:
    bool m_stop = false;
    QColor m_bgrColor = Qt::black;

    void setStopImpl(bool stop);
};

} // ns msc
