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

#include "common/abstractinteractiveobject.h"

#include "grippoint.h"
#include <QGraphicsObject>

namespace msc {

class GripPointsHandler : public QGraphicsObject,
                          public AbstractInteractiveObject
{
    Q_OBJECT

public:
    GripPointsHandler(QGraphicsItem *parent = nullptr);

    void handleGripPointMove(GripPoint *handle, const QPointF &from, const QPointF &to) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QVector<GripPoint *> gripPoints() const;
    GripPoint *gripPoint(GripPoint::Location pnt) const;
    void updateLayout();

    QSizeF minSize() const;

    void setUsedPoints(GripPoint::Locations points);
    GripPoint::Locations usedPoints() const;

public Q_SLOTS:
    void showAnimated();
    void hideAnimated();

protected Q_SLOTS:
    void onVisibilityFinished();

Q_SIGNALS:
    void rectChanged(GripPoint::Location pos, const QPointF &from, const QPointF &to);

protected:
    void changeVisibilityAnimated(bool appear);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

    const QMap<GripPoint::Location, GripPoint *> m_gripPoints;
    DrawRectInfo m_highlighter;
    QPainterPath m_borderPath, m_bodyPath;
    bool m_visible = false;
    QBrush m_bodyBrush = QBrush(QColor::fromRgbF(0., 0., 0., 0.25), Qt::Dense4Pattern);

    GripPoint::Locations m_usedPoints;
};

} // namespace msc
