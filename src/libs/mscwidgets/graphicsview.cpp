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

#include "graphicsview.h"

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"

#include <QGraphicsItem>
#include <QMouseEvent>
#include <mscentity.h>

namespace msc {

/*!
  Constructs a MSV view object with the parent \a parent.
*/
GraphicsView::GraphicsView(QWidget *parent)
    : shared::ui::GraphicsViewBase(parent)
{
    setMinZoomPercent(50);

    setBackgroundBrush(QImage(":/resources/resources/texture.png"));

    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
        Q_EMIT createMessageToolRequested();
        return;
    }

    shared::ui::GraphicsViewBase::mousePressEvent(event);
}

QList<QPair<QPointF, QString>> GraphicsView::mouseMoveCoordinates(QGraphicsScene* scene, const QPoint&, const QPointF& scenePos) const
{
    QList<QPair<QPointF, QString>> coords;
    coords.push_back({utils::CoordinatesConverter::sceneToCif(scenePos), "CIF"});
    for (InteractiveObject *item : utils::itemByPos<InteractiveObject>(scene, scenePos)) {
        coords.push_back({item->mapFromScene(scenePos), item->modelEntity() ? item->modelEntity()->name() : QLatin1String("None")});
    }
    return coords;
}

}
