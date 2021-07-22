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

#include "dvmessagebindingsmodel.h"

#include "dvconnection.h"

#include <QDebug>

namespace dve {

DVMessageBindingsModel::DVMessageBindingsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void DVMessageBindingsModel::initModel(dvm::DVConnection *connection)
{
    beginResetModel();
    m_connection = connection;
    /// @todo fetch data from interface view
    endResetModel();
}

int DVMessageBindingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    /// @todo use data from interface view
    return 1;
}

QVariant DVMessageBindingsModel::data(const QModelIndex &index, int role) const
{
    /// @todo use data from interface view and messages from the connection
    switch (role) {
    case Qt::DisplayRole:
        return QVariant("Example F1.m -> F2.m");
    case Qt::CheckStateRole:
        return QVariant(true);
    }
    return {};
}

bool DVMessageBindingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        /// @todo run command to add/remove the message
        return true;
    }
    return false;
}

Qt::ItemFlags DVMessageBindingsModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

} // namespace dve
