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

#include "aadlobjectconnection.h"
#include "aadlobjectsmodel.h"
#include "commandids.h"
#include "commandsfactory.h"

namespace ive {
namespace cmd {

QVector<QPointer<aadl::AADLObjectIface>> relatedIfaces(aadl::AADLObjectIface *iface)
{
    QVector<QPointer<aadl::AADLObjectIface>> ifaces;

    if (iface) {
        ifaces.append(iface);

        if (iface->isCloned())
            ifaces += iface->clones();
    }

    return ifaces;
}

QVector<aadl::AADLObjectConnection *> relatedConnections(aadl::AADLObjectIface *iface)
{
    QVector<aadl::AADLObjectConnection *> connections;

    if (iface && iface->objectsModel())
        for (aadl::AADLObjectIface *i : relatedIfaces(iface))
            connections += i->objectsModel()->getConnectionsForIface(i->id());

    return connections;
}

CmdIfaceParamBase::CmdIfaceParamBase(aadl::AADLObjectIface *iface)
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
    if (auto model = m_iface->objectsModel()) {
        for (auto connection : m_connections) {
            if (connection->sourceInterface() && connection->targetInterface()) {
                if (connection->sourceInterface()->params() != connection->targetInterface()->params()) {
                    const QVariantList params = { QVariant::fromValue(connection), QVariant::fromValue(model) };
                    if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                        m_cmdRmConnections.append(cmdRm);
                }
            }
        }
    }

    m_rmCommandsPrepared = true;
}

void CmdIfaceParamBase::redo()
{
    if (!m_rmCommandsPrepared)
        prepareRmCommands();

    for (QUndoCommand *rmCmd : m_cmdRmConnections)
        rmCmd->redo();
}

void CmdIfaceParamBase::undo()
{
    for (QUndoCommand *rmCmd : m_cmdRmConnections)
        rmCmd->undo();
}

bool CmdIfaceParamBase::mergeWith(const QUndoCommand * /*command*/)
{
    return false;
}

}
}
