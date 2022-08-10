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

#pragma once

#include "commandsstack.h"

#include <QAbstractItemModel>
#include <QPointer>

namespace ivm {
class AbstractSystemChecks;
class IVFunctionType;
class ArchetypeModel;
class IVArchetypeReference;
}

namespace ive {

class ArchetypesWidgetModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Column
    {
        LibraryName = 0,
        FunctionName,
    };
    Q_ENUM(Column)

    explicit ArchetypesWidgetModel(ivm::ArchetypeModel *archetypeModel, ivm::AbstractSystemChecks *checks,
            cmd::CommandsStack::Macro *macro, QObject *parent = nullptr);

    ivm::IVFunctionType *getFunction();
    void setFunction(ivm::IVFunctionType *fn);

    QVector<ivm::IVArchetypeReference *> getArchetypeReferences();
    bool areArchetypesModified();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    bool m_areArchetypesModified;
    QPointer<ivm::IVFunctionType> m_function;
    QPointer<ivm::AbstractSystemChecks> m_checks;
    cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
    QPointer<ivm::ArchetypeModel> m_archetypeModel;
    QVector<ivm::IVArchetypeReference *> m_archetypeReferences;
};

} // namespace ive
