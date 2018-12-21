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

#include "grippoint.h"
#include "common/abstractinteractiveobject.h"

#include <QGraphicsObject>
#include <QSet>

namespace msc {

class GripPointsHandler : public QGraphicsObject,
                          public AbstractInteractiveObject
{
    Q_OBJECT

public:
    GripPointsHandler(QGraphicsItem *parent = nullptr);

    void handleGripPointPress(GripPoint *handle, const QPointF &from, const QPointF &to) override;
    void handleGripPointMove(GripPoint *handle, const QPointF &from, const QPointF &to) override;
    void handleGripPointRelease(GripPoint *handle, const QPointF &from, const QPointF &to) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QVector<GripPoint *> gripPoints() const;
    GripPoint *gripPoint(GripPoint::Location pnt) const;
    void updateLayout();

    QSizeF minSize() const;

    void setUsedPoints(GripPoint::Locations points);
    GripPoint::Locations usedPoints() const;

    void setGripPointPos(GripPoint::Location location, const QPointF &pos);

public Q_SLOTS:
    void showAnimated();
    void hideAnimated();

protected Q_SLOTS:
    void onOpacityAnimationFinished();

Q_SIGNALS:
    void manualGeometryChangeStart(GripPoint::Location pos, const QPointF &from, const QPointF &to);
    void manualGeometryChangeProgress(GripPoint::Location pos, const QPointF &from, const QPointF &to);
    void manualGeometryChangeFinish(GripPoint::Location pos, const QPointF &from, const QPointF &to);

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

    /*
     * To keep the selection frame and its grippoints unscaled,
     * the QGraphicsItem::ItemIgnoresTransformations flag is used.
     * That means that the own bounding box and/or child positions should be managed
     * manually to reflect the current view scale.
     *
     * The result routine is too fragile, though: it depends on scene()->views(),
     * plus the scale factor could be obtained from QTransform only in case
     * there were no additional transformations performed (no translate nor rotate).
     * For now it seems enough, but it might need refactoring afterwhile.
    */
    QPointF viewScale() const;
};

} // namespace msc
