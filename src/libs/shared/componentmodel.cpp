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
    connect(&d->watcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString &path) {
        if (path != d->libraryPath)
            return;

        /// Get all interfaceview paths from fs
        const QDir libDir(d->libraryPath);
        const QStringList entries = libDir.entryList(QDir::Dirs);
        QSet<QString> componentsPaths;
        for (const QString &name : qAsConst(entries)) {
            const QString relInterfaceviewPath = name + QDir::separator() + shared::kDefaultInterfaceViewFileName;
            if (libDir.exists(relInterfaceviewPath)) {
                componentsPaths.insert(libDir.absoluteFilePath(relInterfaceviewPath));
            }
        }

        /// Get all loaded components with removing them in previously loaded paths
        QSet<QString> existingComponents;
        for (auto it = d->components.cbegin(); it != d->components.cend(); ++it) {
            existingComponents.insert(it.value()->componentPath);
            componentsPaths.remove(it.value()->componentPath);
        }

        /// Remove from the model non-existing in fs component
        existingComponents.subtract(componentsPaths);
        QList<Id> idsToRemove;
        for (const QString &path : qAsConst(existingComponents)) {
            for (auto it = d->components.cbegin(); it != d->components.cend(); ++it) {
                if (it.value()->componentPath == path) {
                    idsToRemove << it.key();
                    break;
                }
            }
        }
        for (const Id &id : qAsConst(idsToRemove)) {
            removeComponent(id);
        }

        /// Add to the model new components
        for (const QString &path : qAsConst(componentsPaths)) {
            if (auto item = loadComponent(path)) {
                appendRow(item);
                d->watcher.addPath(path);
            }
        }
    });
    connect(&d->watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
        for (auto it = d->components.cbegin(); it != d->components.cend(); ++it) {
            if ((*it)->componentPath == path) {
                if (auto item = itemById(it.key())) {
                    item->setData(true, UpdateRole);
                }
                break;
            }
        }
    });
    connect(this, &QAbstractItemModel::rowsAboutToBeRemoved, this,
            [this](const QModelIndex &parent, int first, int last) {
                if (parent == indexFromItem(invisibleRootItem())) {
                    for (auto idx = first; idx <= last; ++idx) {
                        const Id id = index(idx, 0, parent).data(IdRole).toUuid();
                        auto component = d->components.take(id);
                        d->watcher.removePath(component->componentPath);
                    }
                }
            });
}

ComponentModel::~ComponentModel() { }

QString ComponentModel::componentPath(const shared::Id &id) const
{
    auto it = d->components.constFind(id);
    if (it != d->components.constEnd())
        return (*it)->componentPath;

    return {};
}

QString ComponentModel::libraryPath() const
{
    return d->libraryPath;
}

VEObject *ComponentModel::getObject(const shared::Id &id)
{
    auto it = d->components.constFind(id);
    if (it != d->components.constEnd()) {
        auto objIt = std::find_if((*it)->objects.constBegin(), (*it)->objects.constEnd(),
                [&id](VEObject *obj) { return obj->id() == id; });
        if (objIt != (*it)->objects.constEnd())
            return *objIt;
    }
    return nullptr;
}

void ComponentModel::load(const QString &path)
{
    clear();
    /// TODO: check
    d->watcher.removePaths(d->watcher.directories());
    d->watcher.removePaths(d->watcher.files());
    d->components.clear();
    /// end
    d->libraryPath = path;

    auto headerItem = new QStandardItem(d->modelName);
    headerItem->setTextAlignment(Qt::AlignCenter);
    setHorizontalHeaderItem(0, headerItem);

    if (path.isEmpty() || !QDir(path).exists())
        return;

    QDirIterator importableIt(path, QDir::Dirs | QDir::NoDotAndDotDot);
    while (importableIt.hasNext()) {
        if (auto item = loadComponent(
                    importableIt.next() + QDir::separator() + shared::kDefaultInterfaceViewFileName)) {
            appendRow(item);
        }
    }
    d->watcher.addPath(path);
}

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
    if (item = loadComponent(path)) {
        insertRow(row, item);
    } else {
        d->watcher.removePath(path);
    }
}

void ComponentModel::addComponent(QSharedPointer<Component> component)
{
    for (auto id : qAsConst(component->rootIds))
        d->components.insert(id, component);

    d->watcher.addPath(component->componentPath + QDir::separator() + shared::kDefaultInterfaceViewFileName);
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
