/*
   Copyright (C) 2022-2023 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "commandsstackbase.h" // Can't forward declare nested class Macro

#include <QAbstractTableModel>

namespace ivm { class IVModel; }

namespace ive {

/**
 * \brief Model to read/write attributes of functions and interfaces.
 */
class IVEditAttributesModel : public QAbstractTableModel
{
public:
    enum ObjectType { Interface, Function };
    // Custom roles that the delegate requires us to support.
    enum { DataRole = Qt::UserRole+2, EditRole, ValidatorRole };

    explicit IVEditAttributesModel(ObjectType objectType, ivm::IVModel *model, shared::cmd::CommandsStackBase::Macro *macro = nullptr);
    ~IVEditAttributesModel();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    struct Private;
    Private* d;
};

}
