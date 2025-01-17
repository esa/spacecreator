/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstackbase.h"

#include <QAbstractListModel>
#include <QPointer>

namespace dvm {
class AbstractSystemChecks;
class DVConnection;
}

namespace dve {
class DVMessageBindingsModel;

class DVMessageBindingsModel : public QAbstractListModel
{
public:
    explicit DVMessageBindingsModel(shared::cmd::CommandsStackBase::Macro *macro, QObject *parent = nullptr);

    void initModel(dvm::DVConnection *connection, dvm::AbstractSystemChecks *systemChecker);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    bool isEditable(const QModelIndex &index) const;

private:
    struct DataItem {
        QString m_fromFunction;
        QString m_fromInterface;
        QString m_toFunction;
        QString m_toInterface;
    };

    QPointer<dvm::DVConnection> m_connection;
    QList<DataItem> m_messages;
    shared::cmd::CommandsStackBase::Macro *m_cmdMacro { nullptr };
};

} // namespace dve
