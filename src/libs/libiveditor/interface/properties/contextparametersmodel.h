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

#include "aadlparameter.h"
#include "commandsstack.h"
#include "propertiesmodelbase.h"

#include <QPointer>
#include <QSharedPointer>

namespace Asn1Acn {
class File;
}

namespace aadl {
class AADLObject;
class ContextParameter;
}

namespace aadlinterface {

class ContextParametersModel : public PropertiesModelBase
{
    Q_OBJECT

public:
    static const int ItemTypeRole { Qt::UserRole + 2 };
    static const int ColumnName { 0 };
    static const int ColumnType { 1 };
    static const int ColumnValue { 2 };

    explicit ContextParametersModel(cmd::CommandsStack::Macro *macro, QObject *parent = nullptr);
    ~ContextParametersModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDataObject(aadl::AADLObject *obj);
    const aadl::AADLObject *dataObject() const override;

    void setDataTypes(const QSharedPointer<Asn1Acn::File> &dataTypes);

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
    aadl::AADLObject *m_dataObject { nullptr };
    QVector<aadl::ContextParameter> m_params;
    QSharedPointer<Asn1Acn::File> m_dataTypes;

    void createNewRow(const aadl::ContextParameter &param, int row);
};

}
