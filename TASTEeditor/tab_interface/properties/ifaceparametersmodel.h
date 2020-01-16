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

#include "propertiesmodelbase.h"
#include "tab_aadl/aadlparameter.h"

namespace taste3 {
namespace aadl {

class AADLObject;
struct IfaceParameter;
class IfaceParametersModel : public PropertiesModelBase
{
    Q_OBJECT
public:
    static const int ItemTypeRole { Qt::UserRole + 2 };
    static const int ColumnName { 0 };
    static const int ColumnType { 1 };
    static const int ColumnEncoding { 2 };
    static const int ColumnDirection { 3 };

    explicit IfaceParametersModel(QObject *parent = nullptr);
    ~IfaceParametersModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDataObject(AADLObject *obj);

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    AADLObject *m_dataObject { nullptr };
    QVector<IfaceParameter> m_params;

    void createNewRow(const IfaceParameter &param, int row);
};

} // namespace aadl
} // namespace taste3
