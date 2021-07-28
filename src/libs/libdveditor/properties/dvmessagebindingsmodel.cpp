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
#include "commands/cmdentitiesremove.h"
#include "commands/cmdmessagenetitycreate.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvpartition.h"

#include <QDebug>

namespace dve {

DVMessageBindingsModel::DVMessageBindingsModel(shared::cmd::CommandsStackBase::Macro *macro, QObject *parent)
    : QAbstractListModel(parent)
    , m_cmdMacro(macro)
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
            QList<QPair<QString, QString>> names = systemChecker->messages(sourceName, targetName);
            for (const QPair<QString, QString> &name : qAsConst(names)) {
                m_messages.append({ sourceName, name.first, targetName, name.second });
            }
            names = systemChecker->messages(targetName, sourceName);
            for (const QPair<QString, QString> &name : qAsConst(names)) {
                m_messages.append({ targetName, name.first, sourceName, name.second });
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

    const DataItem &item = m_messages.at(index.row());
    switch (role) {
    case Qt::DisplayRole: {
        return QVariant(QString("%1.%2 -> %3.%4")
                                .arg(item.m_fromFunction, item.m_fromInterface, item.m_toFunction, item.m_toInterface));
    }
    case Qt::CheckStateRole:
        return m_connection->hasMessage(
                       item.m_fromFunction, item.m_fromInterface, item.m_toFunction, item.m_toInterface)
                ? QVariant(Qt::Checked)
                : QVariant(Qt::Unchecked);
    }
    return {};
}

bool DVMessageBindingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= m_messages.size()) {
        return {};
    }

    if (role == Qt::CheckStateRole) {
        DataItem &item = m_messages[index.row()];
        if ((Qt::CheckState)value.toInt() == Qt::Checked) {
            auto cmd = new cmd::CmdMessageEntityCreate(
                    m_connection, item.m_fromFunction, item.m_fromInterface, item.m_toFunction, item.m_toInterface);
            m_cmdMacro->push(cmd);
        } else {
            dvm::DVMessage *message = m_connection->message(
                    item.m_fromFunction, item.m_fromInterface, item.m_toFunction, item.m_toInterface);
            if (message) {
                auto cmd = new cmd::CmdEntitiesRemove({ message }, message->model());
                m_cmdMacro->push(cmd);
            }
        }
        return true;
    }
    return false;
}

Qt::ItemFlags DVMessageBindingsModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

} // namespace dve
