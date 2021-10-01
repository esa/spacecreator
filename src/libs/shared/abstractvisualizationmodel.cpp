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

#include "abstractvisualizationmodel.h"

#include "commandsstackbase.h"
#include "vemodel.h"
#include "veobject.h"

namespace shared {

AbstractVisualizationModel::AbstractVisualizationModel(
        shared::VEModel *veModel, cmd::CommandsStackBase *commandsStack, QObject *parent)
    : QStandardItemModel(parent)
    , m_veModel(veModel)
    , m_commandsStack(commandsStack)
{
    connect(m_veModel, &VEModel::modelReset, this, [this]() {
        m_itemCache.clear();
        removeRows(0, rowCount());
    });
    connect(m_veModel, &VEModel::objectsAdded, this, &AbstractVisualizationModel::addItems);
    connect(m_veModel, &VEModel::objectRemoved, this, &AbstractVisualizationModel::removeItem);
    setSortRole(TypeRole);
}

QStandardItem *AbstractVisualizationModel::getItem(const Id id)
{
    return id.isNull() ? nullptr : m_itemCache.value(id);
}

void AbstractVisualizationModel::updateItemData(QStandardItem *item, VEObject *obj)
{
    item->setData(obj->titleUI(), Qt::DisplayRole);
}

QList<QStandardItem *> AbstractVisualizationModel::createItems(VEObject *obj)
{
    auto item = new QStandardItem(obj->titleUI());
    item->setDragEnabled(true);
    item->setData(obj->id(), IdRole);
    return { item };
}

QStandardItem *AbstractVisualizationModel::getParentItem(VEObject *obj)
{
    if (!obj) {
        return nullptr;
    }

    return obj->parentObject() ? getItem(obj->parentObject()) : invisibleRootItem();
}

QStandardItem *AbstractVisualizationModel::getItem(VEObject *obj)
{
    return obj ? getItem(obj->id()) : nullptr;
}

void AbstractVisualizationModel::addItems(const QVector<shared::Id> &objectsIds)
{
    for (auto objId : objectsIds) {
        addItem(m_veModel->getObject(objId));
    }
}

void AbstractVisualizationModel::removeItem(shared::Id objId)
{
    const QStandardItem *item = m_itemCache.take(objId);
    if (!item) {
        return;
    } else {
        QStandardItem *parentItem = item->parent() ? item->parent() : invisibleRootItem();
        if (parentItem) {
            parentItem->removeRow(item->row());
        }
    }
}

void AbstractVisualizationModel::updateItem()
{
    if (auto obj = qobject_cast<VEObject *>(sender())) {
        if (auto item = getItem(obj->id())) {
            updateItemData(item, obj);
        }
    }
}

void AbstractVisualizationModel::addItem(VEObject *obj)
{
    if (m_itemCache.contains(obj->id())) {
        return;
    }

    QList<QStandardItem *> items = createItems(obj);
    if (!items.isEmpty()) {
        if (auto parentItem = getParentItem(obj)) {
            parentItem->appendRow(items);
            parentItem->sortChildren(0);
            m_itemCache.insert(obj->id(), items[0]);
        }
    }
}
} // namespace shared
