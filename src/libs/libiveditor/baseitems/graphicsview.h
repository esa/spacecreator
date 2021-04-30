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

#include "common.h"
#include "ui/graphicsviewbase.h"

#include <QList>
#include <QPair>

namespace ive {

/*!
  \class GraphicsView
  \brief The QGraphicsView wrapper for displaying the IV diagrams.
*/
class GraphicsView : public shared::ui::GraphicsViewBase
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget *parent = nullptr);

Q_SIGNALS:
    void importEntity(const shared::Id &id, const QPointF &scenePos);
    void instantiateEntity(const shared::Id &id, const QPointF &scenePos);

protected:
    QList<QPair<QPointF, QString>> mouseMoveCoordinates(
            QGraphicsScene *scene, const QPoint &screenPos, const QPointF &scenePos) const override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

}
