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

#include "drawrectinfo.h"
#include "grippoint.h"

#include <QGraphicsObject>

namespace shared {
namespace ui {

/*!
  \class shared::ui::GripPointsHandler
  \brief Set of handlers (up to 9) for resizing/moving item by mouse.

  \ingroup shared
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

    QRectF targetBoundingRect() const;

    void setUsedPoints(GripPoint::Locations points);
    GripPoint::Locations usedPoints() const;

    void setGripPointPos(GripPoint::Location location, const QPointF &scenePos);
    void setGripPointPos(GripPoint *gripPoint, const QPointF &scenePos);
    GripPoint *gripPoint(GripPoint::Location location) const;

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

    QList<GripPoint *> m_gripPoints;
    DrawRectInfo m_highlighter;
    bool m_visible = false;

    GripPoint::Locations m_usedPoints;
};

}
}
