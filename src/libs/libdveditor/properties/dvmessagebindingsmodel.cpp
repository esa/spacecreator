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

#include "abstractsystemchecks.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvpartition.h"

#include <QDebug>

namespace dve {

DVMessageBindingsModel::DVMessageBindingsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void DVMessageBindingsModel::initModel(dvm::DVConnection *connection, AbstractSystemChecks *systemChecker)
{
    Q_ASSERT(connection);
    Q_ASSERT(connection->model());
    dvm::DVModel *model = connection->model();

    dvm::DVNode *sourceNode = connection->sourceNode();
    dvm::DVNode *targetNode = connection->targetNode();

    beginResetModel();
    m_messages.clear();
    m_connection = connection;

    if (!systemChecker || !sourceNode || !targetNode) {
        endResetModel();
        return;
    }

    QList<dvm::DVFunction *> sourceFunctions = model->functions(sourceNode);
    QStringList sourceFunctionNames;
    for (dvm::DVFunction *fn : qAsConst(sourceFunctions)) {
        if (fn) {
            sourceFunctionNames.append(fn->title());
        }
    }
    QList<dvm::DVFunction *> targetFunctions = model->functions(targetNode);
    QStringList targetFunctionNames;
    for (dvm::DVFunction *fn : qAsConst(targetFunctions)) {
        if (fn) {
            targetFunctionNames.append(fn->title());
        }
    }

    for (const QString &sourceName : sourceFunctionNames) {
        for (const QString &targetName : targetFunctionNames) {
            if (sourceName == targetName) {
                continue;
            }
            QStringList names = systemChecker->messages(sourceName, targetName);
            for (const QString &name : qAsConst(names)) {
                m_messages.append({ name, sourceName, targetName, false });
            }
            names = systemChecker->messages(targetName, sourceName);
            for (const QString &name : qAsConst(names)) {
                m_messages.append({ name, targetName, sourceName, false });
            }
        }
    }

    endResetModel();
}

int DVMessageBindingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_messages.size();
}

QVariant DVMessageBindingsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= m_messages.size()) {
        return {};
    }

    switch (role) {
    case Qt::DisplayRole: {
        const DataItem &item = m_messages.at(index.row());
        return QVariant(QString("%1.%2 -> %3.%2").arg(item.m_from, item.m_name, item.m_to));
    }
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
