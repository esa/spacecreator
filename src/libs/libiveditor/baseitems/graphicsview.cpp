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

#include "aadlobject.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/interactiveobject.h"
#include "interface/aadlrectgraphicsitem.h"

#include <QGraphicsItem>
#include <QMouseEvent>

namespace aadlinterface {

GraphicsView::GraphicsView(QWidget *parent)
    : shared::ui::GraphicsViewBase(parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    setDragMode(QGraphicsView::DragMode::RubberBandDrag);
    setRubberBandSelectionMode(Qt::IntersectsItemShape);
}

QList<QPair<QPointF, QString>> GraphicsView::mouseMoveCoordinates(
        QGraphicsScene *, const QPoint &screenPos, const QPointF &scenePos) const
{
    QList<QPair<QPointF, QString>> coords;
    QList<QGraphicsItem *> itemsUnderCursor = items(screenPos);
    for (QGraphicsItem *item : itemsUnderCursor) {
        if (auto iObj = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
            coords.push_back({ item->mapFromScene(scenePos),
                    iObj->aadlObject() ? iObj->aadlObject()->objectName() : QLatin1String("None") });
        }
    }
    return coords;
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    bool keyHandled(false);
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down: {
        for (QGraphicsItem *item : scene()->selectedItems()) {
            if (aadlinterface::AADLRectGraphicsItem *rectItem =
                            // Not the qgraphicsitem_cast due its "successfull" cast of AADLFunctionName/Text Graphics
                            // Items
                    qobject_cast<aadlinterface::AADLRectGraphicsItem *>(item->toGraphicsObject())) {
                rectItem->singleStepMove(aadlinterface::AADLRectGraphicsItem::MoveStep(event->key()));
                keyHandled = true;
            }
        }
        break;
    }
    default:
        break;
    }

    if (keyHandled)
        event->accept();
    else
        GraphicsViewBase::keyPressEvent(event);
}

}
