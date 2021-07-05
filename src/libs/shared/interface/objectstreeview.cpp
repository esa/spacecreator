/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "objectstreeview.h"

#include "abstractvisualizationmodel.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QModelIndex>
#include <QMouseEvent>

namespace shared {
namespace ui {

ObjectsTreeView::ObjectsTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
}

void ObjectsTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void ObjectsTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)) {
        if ((event->pos() - m_dragStartPosition).manhattanLength() > QApplication::startDragDistance()) {
            const QModelIndex index = indexAt(event->pos());
            if (!index.isValid()) {
                return;
            }

            QDrag *drag = new QDrag(this);
            DropData *mimeData = new DropData;
            const Id id = index.data(AbstractVisualizationModel::IdRole).toUuid();
            mimeData->entityId = id.toString();
            const int dropType = index.data(AbstractVisualizationModel::DropRole).toInt();
            mimeData->dropType = static_cast<DropData::Type>(dropType);
            drag->setMimeData(mimeData);
            const QPixmap pix = index.data(AbstractVisualizationModel::CursorPixmapRole).value<QPixmap>();
            drag->setDragCursor(pix, Qt::DropAction::CopyAction);
            drag->exec(Qt::DropAction::CopyAction);
        }
    }
    QTreeView::mouseMoveEvent(event);
}

} // namespace ui
} // namespace ive
