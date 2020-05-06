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

#include "ui/graphicsviewbase.h"

namespace msc {

/*!
  \class msc::GraphicsView
  \brief Basic view for an MSC diagram

  \inmodule MscWidgets

  * Handles the middle mouse button to pan the view.
  * Provides zoom functionality.
  * Reports the current mouse position as signal
*/
class GraphicsView : public shared::ui::GraphicsViewBase
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget *parent = nullptr);

Q_SIGNALS:
    void createMessageToolRequested();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    QList<QPair<QPointF, QString>> mouseMoveCoordinates(QGraphicsScene* scene, const QPoint& screenPos, const QPointF& scenePos) const override;
};

}
