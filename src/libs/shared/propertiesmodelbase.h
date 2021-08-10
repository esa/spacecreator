/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QStandardItemModel>

namespace shared {
class VEObject;

class PropertiesModelBase : public QStandardItemModel
{
    Q_OBJECT
public:
    enum Roles
    {
        DataRole = Qt::UserRole + 2,
        EditRole,
        ValidatorRole,
        InfoRole,
    };

    explicit PropertiesModelBase(QObject *parent = nullptr);
    ~PropertiesModelBase() override = default;

    virtual void setDataObject(VEObject *obj) = 0;

    virtual bool createProperty(const QString &propName) = 0;
    virtual bool removeProperty(const QModelIndex &index) = 0;

    virtual bool isAttr(const QModelIndex &id) const = 0;
    virtual bool isProp(const QModelIndex &id) const = 0;

    virtual VEObject *entity() const;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
            const QModelIndex &destinationParent, int destinationChild) override;

protected:
    VEObject *m_dataObject { nullptr };
};

} // namespace shared
