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

#include "cmdifaceparambase.h"

#include "cmdentitiesremove.h"
#include "ivconnection.h"
#include "ivmodel.h"

namespace ive {
namespace cmd {

QVector<QPointer<ivm::IVInterface>> relatedIfaces(ivm::IVInterface *iface)
{
    QVector<QPointer<ivm::IVInterface>> ifaces;

    if (iface) {
        ifaces.append(iface);

        if (iface->isCloned())
            ifaces += iface->clones();
    }

    return ifaces;
}

QVector<ivm::IVConnection *> relatedConnections(ivm::IVInterface *iface)
{
    QVector<ivm::IVConnection *> connections;

    if (iface && iface->model())
        for (ivm::IVInterface *i : relatedIfaces(iface))
            connections += i->model()->getConnectionsForIface(i->id());

    return connections;
}

CmdIfaceParamBase::CmdIfaceParamBase(ivm::IVInterface *iface)
    : QUndoCommand()
    , m_iface(iface)
    , m_connections(relatedConnections(m_iface))
{
}

CmdIfaceParamBase::~CmdIfaceParamBase()
{
    qDeleteAll(m_cmdRmConnections);
}

void CmdIfaceParamBase::prepareRmCommands()
{
    QList<QPointer<ivm::IVObject>> entities;
    if (auto model = m_iface->model()) {
        for (auto connection : qAsConst(m_connections)) {
            if (connection->sourceInterface() && connection->targetInterface()) {
                if (connection->sourceInterface()->params() != connection->targetInterface()->params()) {
                    entities.append(connection);
                }
            }
        }
        if (!entities.isEmpty()) {
            auto cmdRm = new cmd::CmdEntitiesRemove(entities, model);
            m_cmdRmConnections.append(cmdRm);
        }
    }

    m_rmCommandsPrepared = true;
}

void CmdIfaceParamBase::redo()
{
    if (!m_rmCommandsPrepared)
        prepareRmCommands();

    for (QUndoCommand *rmCmd : qAsConst(m_cmdRmConnections))
        rmCmd->redo();
}

void CmdIfaceParamBase::undo()
{
    for (QUndoCommand *rmCmd : qAsConst(m_cmdRmConnections))
        rmCmd->undo();
}

bool CmdIfaceParamBase::mergeWith(const QUndoCommand * /*command*/)
{
    return false;
}

}
}
