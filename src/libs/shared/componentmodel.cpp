/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "componentmodel.h"

#include "common.h"
#include "diskutils.h"
#include "vemodel.h"
#include "veobject.h"

#include <QDirIterator>
#include <QFileSystemWatcher>

namespace shared {

struct ComponentModelPrivate {
    QString modelName;
    QString libraryPath;
    QHash<Id, QSharedPointer<ComponentModel::Component>> components;
    QFileSystemWatcher watcher;
};

ComponentModel::ComponentModel(const QString &modelName, QObject *parent)
    : QStandardItemModel(parent)
    , d(new ComponentModelPrivate)
{
    d->modelName = modelName;
}

ComponentModel::~ComponentModel() { }

void ComponentModel::removeComponent(const Id &id)
{
    if (auto item = itemById(id))
        removeRow(item->row());
}

void ComponentModel::reloadComponent(const Id &id)
{
    auto item = itemById(id);
    if (!item)
        return;

    if (!item->index().isValid())
        return;

    const int row = item->index().row();
    const QString path = d->components.value(id)->componentPath;
    removeComponent(id);
    if ((item = loadComponent(path))) {
        insertRow(row, item);
    } else {
        d->watcher.removePath(path);
    }
}

QList<Id> ComponentModel::componentIDs() const
{
    return d->components.keys();
}

QSharedPointer<ComponentModel::Component> ComponentModel::component(const Id &id) const
{
    return d->components.value(id);
}

void ComponentModel::addComponent(QSharedPointer<Component> component)
{
    for (auto id : qAsConst(component->rootIds))
        d->components.insert(id, component);

    d->watcher.addPath(component->componentPath);
}

QStandardItem *ComponentModel::itemById(const Id &id)
{
    for (int idx = 0; idx < rowCount(); ++idx) {
        if (item(idx)->data(IdRole) == id) {
            return item(idx);
        }
    }
    return nullptr;
}

} // namespace shared
