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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class QUndoStack;
namespace msc {

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);

    void setZoom(double percent);

    QUndoStack *undoStack() const;

Q_SIGNALS:
    void mouseMoved(const QPoint &screenPos, const QPointF &scenePos, const QPointF &itemPos) const;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    QUndoStack *m_undoStack = nullptr;
};

} // namespace msc

#endif // GRAPHICSVIEW_H
