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

#include "archetypeswidgetmodel.h"

#include "archetypes/archetypemodel.h"
#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdfunctionimplementationdefaultchange.h"
#include "commands/cmdfunctionimplementationinsert.h"
#include "commands/cmdfunctionimplementationremove.h"
#include "commands/cmdfunctionimplementationupdate.h"
#include "ivarchetypereference.h"
#include "ivcore/abstractsystemchecks.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"

#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QMessageBox>
#include <QMetaEnum>
#include <algorithm>

namespace ive {

ArchetypesWidgetModel::ArchetypesWidgetModel(ivm::ArchetypeModel *archetypeModel, ivm::AbstractSystemChecks *checks,
        cmd::CommandsStack::Macro *macro, QObject *parent)
    : QAbstractItemModel(parent)
    , m_checks(checks)
    , m_cmdMacro(macro)
    , m_archetypeModel(archetypeModel)
{
}

void ArchetypesWidgetModel::setFunction(ivm::IVFunctionType *fn)
{
    beginResetModel();
    m_function = fn;
    endResetModel();
}

QVariant ArchetypesWidgetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Column::LibraryName:
            return tr("Library Name");
        case Column::FunctionName:
            return tr("Function Name");
        }
    }
    return QVariant();
}

QModelIndex ArchetypesWidgetModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ArchetypesWidgetModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    // no nesting levels -> no parents
    return QModelIndex();
}

int ArchetypesWidgetModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (!m_function) {
        return 0;
    }

    return m_archetypeReferences.size();
}

int ArchetypesWidgetModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    static const QMetaEnum &me = QMetaEnum::fromType<Column>();
    return me.keyCount();
}

QVariant ArchetypesWidgetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_function) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Column::LibraryName:
            return QVariant(m_archetypeReferences.value(index.row())->getLibraryName());
        case Column::FunctionName:
            return QVariant(m_archetypeReferences.value(index.row())->getFunctionName());
        }
        return QVariant();
    }

    if (role == Qt::EditRole) {
        QString libraryName = m_archetypeReferences.value(index.row())->getLibraryName();
        switch (index.column()) {
        case Column::LibraryName:
            return QVariant(m_archetypeModel->getLibrariesNames());
        case Column::FunctionName:
            return QVariant(m_archetypeModel->getFunctionsNamesByLibraryName(libraryName));
        }
        return QVariant();
    }

    return QVariant();
}

bool ArchetypesWidgetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || !m_function) {
        return false;
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole)) {
        switch (index.column()) {
        case Column::LibraryName:
            m_archetypeReferences.value(index.row())->setLibraryName(value.toString());
            m_archetypeReferences.value(index.row())->setFunctionName(QString());
            break;
        case Column::FunctionName:
            m_archetypeReferences.value(index.row())->setFunctionName(value.toString());
            break;
        default:
            return false;
        }

        Q_EMIT dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags ArchetypesWidgetModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool ArchetypesWidgetModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!m_function) {
        return false;
    }
    if (m_archetypeModel->getLibrariesNames().isEmpty()) {
        QMessageBox::warning(qApp->activeWindow(), tr("No Archetype libraries"),
                tr("It's not possible to set add new archetype implementations, no archetype libraries loaded"));
        return false;
    }

    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        ivm::IVArchetypeReference *reference = new ivm::IVArchetypeReference();
        const QString firstLibraryName = m_archetypeModel->getLibrariesNames().first();

        reference->setLibraryName(firstLibraryName);

        if(!m_archetypeModel->getFunctionsNamesByLibraryName(firstLibraryName).isEmpty())
        {
            reference->setFunctionName(m_archetypeModel->getFunctionsNamesByLibraryName(firstLibraryName).first());
        }

        m_archetypeReferences.append(reference);

        std::sort(m_archetypeReferences.begin(), m_archetypeReferences.end(),
                [](ivm::IVArchetypeReference *firstReference, ivm::IVArchetypeReference *secondReference) -> bool {
                    return firstReference->getLibraryName() < secondReference->getLibraryName();
                });
    }

    endInsertRows();
    return true;
}

bool ArchetypesWidgetModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!m_function) {
        return false;
    }
    const auto result = QMessageBox::question(qApp->activeWindow(), tr("Remove archetype implementations"),
            tr("Are you sure you want to remove selected archetype implementations?"));
    if (QMessageBox::StandardButton::Yes != result) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);

    m_archetypeReferences.remove(row, count);

    endRemoveRows();
    return true;
}

} // namespace ive
