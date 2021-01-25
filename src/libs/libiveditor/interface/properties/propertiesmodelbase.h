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

#include <QStandardItemModel>

namespace ivm {
class AADLObject;
}

namespace ive {

class PropertiesModelBase : public QStandardItemModel
{
public:
    explicit PropertiesModelBase(QObject *parent = nullptr);
    ~PropertiesModelBase() override;

    virtual bool createProperty(const QString &propName) = 0;
    virtual bool removeProperty(const QModelIndex &index) = 0;

    virtual bool isAttr(const QModelIndex &id) const = 0;
    virtual bool isProp(const QModelIndex &id) const = 0;

    virtual const ivm::AADLObject *dataObject() const = 0;
};

}
