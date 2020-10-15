/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"

#include <QAbstractListModel>
#include <QSet>

namespace aadl {
class AADLObjectConnection;
class AADLObjectConnectionGroup;
}

namespace aadlinterface {

class AADLConnectionGroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AADLConnectionGroupModel(aadl::AADLObjectConnectionGroup *connectionGroup, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    bool isEnabled(const QModelIndex &index) const;

private:
    aadl::AADLObjectConnectionGroup *m_connectionGroup;
    QList<QPointer<aadl::AADLObjectConnection>> m_allConnections;
    QSet<shared::Id> m_groupedConnetions;
};

} // namespace aadlinterface
