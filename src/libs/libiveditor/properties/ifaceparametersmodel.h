/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstack.h"
#include "ivobject.h"
#include "parameter.h"
#include "propertiesmodelbase.h"

#include <QVector>

namespace ivm {
class InterfaceParameter;
}

namespace ive {

class IfaceParametersModel : public shared::PropertiesModelBase
{
    Q_OBJECT
public:
    enum Column
    {
        Name = 0,
        Type,
        Encoding,
        Direction,
    };
    Q_ENUM(Column)

    explicit IfaceParametersModel(
            cmd::CommandsStack::Macro *macro, const QStringList &asn1Names, QObject *parent = nullptr);
    ~IfaceParametersModel() override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setDataObject(shared::VEObject *obj) override;

    int columnCount(const QModelIndex &) const override;

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    ivm::IVObject *entity() const override;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
            const QModelIndex &destinationParent, int destinationChild) override;
private:
    void createNewRow(const ivm::InterfaceParameter &param, int row);

private:
    const QStringList m_asn1Names;
    QVector<ivm::InterfaceParameter> m_params;
    cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
};

}
