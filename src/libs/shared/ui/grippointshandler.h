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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "grippoint.h"

#include <QGraphicsObject>
#include <QSet>

namespace shared {
namespace ui {

/*!
  \class shared::ui::GripPointsHandler
  \brief Set of handlers (up to 9) for resizing/moving item by mouse.

  \inmodule shared
*/
class GripPointsHandler : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit GripPointsHandler(QGraphicsItem *parent = nullptr);

    GripPoint *createGripPoint(GripPoint::Location location, int idx = -1);
    void removeGripPoint(GripPoint *handle);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QList<GripPoint *> gripPoints() const;
    void updateLayout();

    QSizeF minSize() const;

    void setUsedPoints(GripPoint::Locations points);
    GripPoint::Locations usedPoints() const;

    void setGripPointPos(GripPoint *grip, const QPointF &pos);
    void setGripPointPos(GripPoint::Location location, const QPointF &pos);

    GripPoint* gripPoint(GripPoint::Location location);

public Q_SLOTS:
    void showAnimated();
    void hideAnimated();

protected Q_SLOTS:
    void opacityAnimationFinished();

Q_SIGNALS:
    void manualGeometryChangeStart(GripPoint *gripPoint, const QPointF &at);
    void manualGeometryChangeProgress(GripPoint *gripPoint, const QPointF &from, const QPointF &to);
    void manualGeometryChangeFinish(GripPoint *gripPoint, const QPointF &startedAt, const QPointF &releasedAt);

private:
    void changeVisibilityAnimated(bool appear);

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

    QList<GripPoint *> m_gripPoints;
    DrawRectInfo m_highlighter;
    bool m_visible = false;

    GripPoint::Locations m_usedPoints;
};

}
}
