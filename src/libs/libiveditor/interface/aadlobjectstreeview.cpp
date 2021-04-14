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

#include "ivmodel.h"
#include "commonvisualizationmodel.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QModelIndex>
#include <QMouseEvent>

namespace ive {

AADLObjectsTreeView::AADLObjectsTreeView(shared::DropType componentType, QWidget *parent)
    : QTreeView(parent)
    , m_componentType(componentType)
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
            const shared::Id id = index.data(CommonVisualizationModel::IdRole).toUuid();
            mimeData->setText(id.toString());
            drag->setMimeData(mimeData);
            QPixmap pix;
            Qt::DropAction dropAction;
            if (m_componentType == shared::DropType::ImportableType) {
                static QPixmap icon =
                        QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")).pixmap(128, 128);
                pix = icon;
                dropAction = Qt::CopyAction;
            } else if (m_componentType == shared::DropType::InstantiatableType) {
                static QPixmap icon =
                        QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")).pixmap(128, 128);
                pix = icon;
                dropAction = Qt::LinkAction;
            }
            drag->setPixmap(pix);
            drag->exec(dropAction);
            return;
        }
    }
    QTreeView::mouseMoveEvent(event);
}

} // namespace ive
