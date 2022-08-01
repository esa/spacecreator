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

#include "archetypeswidgetmodel.h"

#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdfunctionimplementationdefaultchange.h"
#include "commands/cmdfunctionimplementationinsert.h"
#include "commands/cmdfunctionimplementationremove.h"
#include "commands/cmdfunctionimplementationupdate.h"
#include "ivcore/abstractsystemchecks.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "archetypes/archetypemodel.h"
#include "ivpropertytemplateconfig.h"

#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QMessageBox>
#include <QMetaEnum>

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

    return m_function->archetypeReferences().size();
}

int ArchetypesWidgetModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    static const QMetaEnum &me = QMetaEnum::fromType<Column>();
    return me.keyCount();
}

QVariant ArchetypesWidgetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_function || m_function->archetypeReferences().size() <= index.row()) {
        return QVariant();
    }

    return QVariant();
}

bool ArchetypesWidgetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || !m_function || m_function->archetypeReferences().size() <= index.row()) {
        return false;
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
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
    return true;
}

bool ArchetypesWidgetModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!m_function) {
        return false;
    }
    const auto result = QMessageBox::question(qApp->activeWindow(), tr("Remove implementations"),
            tr("Are you sure you want to remove selected implementations?"));
    if (QMessageBox::StandardButton::Yes != result) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);

    endRemoveRows();
    return true;
}

} // namespace ive
