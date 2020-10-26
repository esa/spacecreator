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

#include "aadlobjectstreeview.h"

#include "aadlobjectsmodel.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QModelIndex>
#include <QMouseEvent>

namespace aadlinterface {

AADLObjectsTreeView::AADLObjectsTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
}

void AADLObjectsTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void AADLObjectsTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)) {
        if ((event->pos() - m_dragStartPosition).manhattanLength() > QApplication::startDragDistance()) {
            const QModelIndex index = indexAt(event->pos());
            if (!index.isValid()) {
                return;
            }

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            const shared::Id id = index.data(static_cast<int>(aadl::AADLObjectsModel::AADLRoles::IdRole)).toUuid();
            mimeData->setText(id.toString());
            drag->setMimeData(mimeData);
            drag->setPixmap({});
            Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
            return;
        }
    }
    QTreeView::mouseMoveEvent(event);
}

} // namespace aadlinterface
