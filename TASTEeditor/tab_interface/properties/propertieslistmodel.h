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

namespace taste3 {
namespace aadl {

class AADLObject;
class PropertiesListModel : public PropertiesModelBase
{
    Q_OBJECT

public:
    static const int ItemTypeRole { Qt::UserRole + 2 };
    static const int ColumnTitle { 0 };
    static const int ColumnValue { 1 };

    enum ItemType
    {
        Attribute = 0,
        Property
    };

    explicit PropertiesListModel(QObject *parent = nullptr);
    ~PropertiesListModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDataObject(AADLObject *obj);
    const AADLObject *dataObject() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

private:
    AADLObject *m_dataObject { nullptr };
    QVector<QString> m_names;

    void createNewRow(const QString &title, const QVariant &value, ItemType type, int row);

    bool isEditableCellFunction(const QModelIndex &index) const;
    bool isEditableCellIface(const QModelIndex &index) const;
};

} // namespace aadl
} // namespace taste3
