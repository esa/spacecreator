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

#include "parameter.h"
#include "commandsstack.h"
#include "propertiesmodelbase.h"

#include <QPointer>
#include <QSharedPointer>

namespace Asn1Acn {
class File;
}

namespace ivm {
class IVObject;
class ContextParameter;
}

namespace ive {

class ContextParametersModel : public PropertiesModelBase
{
    Q_OBJECT

public:
    enum Column
    {
        Name = 0,
        Type,
        Value,
    };
    Q_ENUM(Column)

    explicit ContextParametersModel(cmd::CommandsStack::Macro *macro, QObject *parent = nullptr);
    ~ContextParametersModel() override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDataObject(ivm::IVObject *obj) override;
    void setDataTypes(const QSharedPointer<Asn1Acn::File> &dataTypes);

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
    ivm::IVObject *m_dataObject { nullptr };
    QVector<ivm::ContextParameter> m_params;
    QSharedPointer<Asn1Acn::File> m_dataTypes;
    QStringList m_asn1Names;

    void createNewRow(const ivm::ContextParameter &param, int row);
};

}
