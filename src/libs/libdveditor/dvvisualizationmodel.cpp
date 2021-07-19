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

#include "dvvisualizationmodel.h"

#include "dvmodel.h"

namespace dve {

DVVisualizationModel::DVVisualizationModel(
        dvm::DVModel *dvModel, shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : shared::AbstractVisualizationModel(dvModel, commandsStack, parent)
{
}

Qt::ItemFlags DVVisualizationModel::flags(const QModelIndex &index) const
{
    return shared::AbstractVisualizationModel::flags(index) & ~Qt::ItemIsEditable;
}

void DVVisualizationModel::updateItemData(QStandardItem *item, shared::VEObject *obj)
{
    item->setData(static_cast<int>(shared::DropData::Type::ImportableType), DropRole);
}

QStandardItem *DVVisualizationModel::createItem(shared::VEObject *obj)
{
    QStandardItem *item = shared::AbstractVisualizationModel::createItem(obj);
    updateItemData(item, obj);
    return item;
}

} // namespace dve
