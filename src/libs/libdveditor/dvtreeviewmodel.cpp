/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvtreeviewmodel.h"

#include "dvmodel.h"

namespace dve {

DVTreeViewModel::DVTreeViewModel(dvm::DVModel *dvModel, shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : shared::AbstractVisualizationModel(dvModel, commandsStack, parent)
{
}

void DVTreeViewModel::updateItemData(QStandardItem *item, shared::VEObject *object)
{
    shared::AbstractVisualizationModel::updateItemData(item, object);

    dvm::DVObject *obj = qobject_cast<dvm::DVObject *>(object);
    if (!obj) {
        return;
    }
    QPixmap pix;
    switch (obj->type()) {
    case dvm::DVObject::Type::Connection: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Device: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/port.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Function: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Message: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/message.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Node: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/node.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Partition: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/partition.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    default:
        break;
    }
    item->setData(pix, Qt::DecorationRole);
}

QStandardItem *DVTreeViewModel::createItem(shared::VEObject *obj)
{
    QStandardItem *item = shared::AbstractVisualizationModel::createItem(obj);
    updateItemData(item, obj);
    return item;
}

} // namespace dve
