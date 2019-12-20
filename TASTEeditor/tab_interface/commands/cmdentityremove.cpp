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

#include "cmdentityremove.h"

#include "commandids.h"

#include <tab_aadl/aadlobject.h>
#include <tab_aadl/aadlobjectconnection.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdEntityRemove::CmdEntityRemove(AADLObject *entity, AADLObjectsModel *model)
    : QUndoCommand()
    , m_model(model)
    , m_entity(entity)
{
    if (auto container = qobject_cast<AADLObjectFunctionType *>(m_entity)) {
        for (auto ri : container->ris()) {
            if (auto connection = m_model->getConnectionForIface(ri->id()))
                m_linkedEntities.append(connection);
            m_linkedEntities.append(ri);
        }
        for (auto pi : container->pis()) {
            if (auto connection = m_model->getConnectionForIface(pi->id()))
                m_linkedEntities.append(connection);
            m_linkedEntities.append(pi);
        }
        for (auto child : container->children()) {
            if (auto connection = m_model->getFunctionType(child->id()))
                m_linkedEntities.append(connection);
            m_linkedEntities.append(child);
        }
    } else if (auto iface = qobject_cast<AADLObjectIface *>(m_entity)) {
        if (auto connection = m_model->getConnectionForIface(iface->id()))
            m_linkedEntities.append(connection);
    }
    setText(QObject::tr("Remove"));
}

void CmdEntityRemove::redo()
{
    if (!m_model)
        return;

    for (auto it = m_linkedEntities.cbegin(); it != m_linkedEntities.cend(); ++it)
        m_model->removeObject(*it);

    if (m_entity)
        m_model->removeObject(m_entity);
}

void CmdEntityRemove::undo()
{
    if (!m_model)
        return;

    if (m_entity)
        m_model->addObject(m_entity);

    for (auto it = m_linkedEntities.crbegin(); it != m_linkedEntities.crend(); ++it)
        m_model->addObject(*it);
}

bool CmdEntityRemove::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdEntityRemove::id() const
{
    return RemoveEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
