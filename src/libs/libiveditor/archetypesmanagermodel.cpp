/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "archetypesmanagermodel.h"

#include "ivarchetypelibraryreference.h"
#include "ivmodel.h"

#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QMessageBox>
#include <QMetaEnum>
#include <algorithm>

namespace ive {

ArchetypesManagerModel::ArchetypesManagerModel(
        ivm::IVModel *objectsModel, cmd::CommandsStack::Macro *macro, QObject *parent)
    : QAbstractItemModel(parent)
    , m_cmdMacro(macro)
    , m_objectsModel(objectsModel)
    , m_archetypeLibraryReferences(objectsModel->getArchetypeLibraryReferences())
{
}

QVector<ivm::IVArchetypeLibraryReference *> ArchetypesManagerModel::getArchetypeLibraryReferences()
{
    return m_archetypeLibraryReferences;
}

QVariant ArchetypesManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Column::LibraryName:
            return tr("Library Name");
        case Column::LibraryPath:
            return tr("Library Path");
        }
    }
    return QVariant();
}

QModelIndex ArchetypesManagerModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ArchetypesManagerModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    // no nesting levels -> no parents
    return QModelIndex();
}

int ArchetypesManagerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (m_objectsModel == nullptr) {
        return 0;
    }

    return m_archetypeLibraryReferences.size();
}

int ArchetypesManagerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    static const QMetaEnum &me = QMetaEnum::fromType<Column>();
    return me.keyCount();
}

QVariant ArchetypesManagerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_objectsModel == nullptr) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Column::LibraryName:
            return QVariant(m_archetypeLibraryReferences.value(index.row())->getLibraryName());
        case Column::LibraryPath:
            return QVariant(m_archetypeLibraryReferences.value(index.row())->getLibraryPath());
        }
        return QVariant();
    }

    if (role == Qt::EditRole) {
        switch (index.column()) {
        case Column::LibraryName:
            return QVariant(m_archetypeLibraryReferences.value(index.row())->getLibraryName());
        case Column::LibraryPath:
            return QVariant(m_archetypeLibraryReferences.value(index.row())->getLibraryPath());
        }
        return QVariant();
    }

    return QVariant();
}

bool ArchetypesManagerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || m_objectsModel == nullptr) {
        return false;
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole)) {

        switch (index.column()) {
        case Column::LibraryName:
            m_archetypeLibraryReferences.value(index.row())->setLibraryName(value.toString());
            break;
        case Column::LibraryPath:
            m_archetypeLibraryReferences.value(index.row())->setLibraryPath(value.toString());
            break;
        default:
            return false;
        }

        Q_EMIT dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags ArchetypesManagerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool ArchetypesManagerModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (m_objectsModel == nullptr) {
        return false;
    }

    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        ivm::IVArchetypeLibraryReference *reference = new ivm::IVArchetypeLibraryReference();

        reference->setLibraryName(QString());
        reference->setLibraryPath(QString());

        m_archetypeLibraryReferences.append(reference);

        std::sort(m_archetypeLibraryReferences.begin(), m_archetypeLibraryReferences.end(),
                [](ivm::IVArchetypeLibraryReference *firstReference,
                        ivm::IVArchetypeLibraryReference *secondReference) -> bool {
                    return firstReference->getLibraryName() < secondReference->getLibraryName()
                            && firstReference->getLibraryPath() < secondReference->getLibraryPath();
                });
    }

    endInsertRows();
    return true;
}

bool ArchetypesManagerModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (m_objectsModel == nullptr) {
        return false;
    }
    const auto result = QMessageBox::question(qApp->activeWindow(), tr("Remove archetype library reference"),
            tr("Are you sure you want to remove selected archetype library reference?"));
    if (QMessageBox::StandardButton::Yes != result) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);

    m_archetypeLibraryReferences.remove(row, count);

    endRemoveRows();
    return true;
}

} // namespace ive
