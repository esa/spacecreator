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

#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"
#include "aadlobject.h"
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

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    // TODO: Move this after creating a shared InteractiveObject class
    const QPoint &screenPos(event->pos());
    const QPointF &scenePos(mapToScene(screenPos));

    const QString infoTemplate("%1: [%2;%3]; ");
    auto coordinatesInfo = [infoTemplate](const QPointF &point, const QString &name) {
        return infoTemplate.arg(name, QString::number(point.x()), QString::number(point.y()));
    };

    QString info = coordinatesInfo(screenPos, "Screen");
    info.append(coordinatesInfo(scenePos, "Scene"));

    QList<QGraphicsItem *> itemsUnderCursor = items(screenPos);
    for (QGraphicsItem *item : itemsUnderCursor) {
        if (auto iObj = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
            info.append(coordinatesInfo(item->mapFromScene(scenePos),
                                        iObj->aadlObject() ? iObj->aadlObject()->objectName() : QLatin1String("None")));
        }
    }

    Q_EMIT mouseMoved(info);

    shared::ui::GraphicsViewBase::mouseMoveEvent(event);
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
                        // Not the qgraphicsitem_cast due its "successfull" cast of AADLFunctionName/Text Graphics Items
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

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    // TODO: Use this or just setting a background image like in the MSC editor?
    static const QImage brushImage { QLatin1String(":/backgrounds/texture.png") };
    const int zoomPercent = zoom();
    const QRectF scaleSceneRect { rect.topLeft() * zoomPercent / 100, rect.size() * zoomPercent / 100 };
    painter->save();
    painter->scale(100 / zoomPercent, 100 / zoomPercent);
    painter->setPen(Qt::NoPen);
    painter->setBrush(brushImage);
    painter->drawRect(scaleSceneRect);
    painter->restore();
}

}
