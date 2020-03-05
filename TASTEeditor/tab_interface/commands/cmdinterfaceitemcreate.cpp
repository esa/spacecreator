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

#include "cmdinterfaceitemcreate.h"

#include "commandids.h"
#include "commandsfactory.h"

#include <QDebug>
#include <baseitems/common/utils.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

QVector<QUndoCommand *> fillCloneCommands(AADLObjectIface *iface, const AADLObjectIface::CreationInfo &creationInfo)
{
    QVector<QUndoCommand *> clones;

    if (!iface || !iface->parentObject())
        return clones;

    if (auto fnType = iface->parentObject()->as<const AADLObjectFunctionType *>())
        for (auto fn : fnType->instances()) {
            AADLObjectIface::CreationInfo clone = AADLObjectIface::CreationInfo::cloneIface(iface, fn);
            // the cloned iface has not been stored yet,
            // so it has invalid pointer to the model
            clone.model = creationInfo.model;
            if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, clone.toVarList()))
                clones.append(cmdRm);
        }

    return clones;
}

CmdInterfaceItemCreate::CmdInterfaceItemCreate(const AADLObjectIface::CreationInfo &creationInfo)
    : m_ifaceInfo(creationInfo)
    , m_entity(AADLObjectIface::createIface(m_ifaceInfo))
    , m_cmdClones(fillCloneCommands(m_entity, m_ifaceInfo))
{
    setText(m_ifaceInfo.type == AADLObjectIface::IfaceType::Provided ? QObject::tr("Create PI")
                                                                     : QObject::tr("Create RI"));
}

CmdInterfaceItemCreate::~CmdInterfaceItemCreate()
{
    qDeleteAll(m_cmdClones);
}

void CmdInterfaceItemCreate::redo()
{
    m_entity->setCoordinates(utils::coordinates(m_ifaceInfo.position));
    if (m_ifaceInfo.function)
        m_ifaceInfo.function->addInterface(m_entity);
    if (m_ifaceInfo.model)
        m_ifaceInfo.model->addObject(m_entity);

    for (QUndoCommand *clone : m_cmdClones)
        clone->redo();
}

void CmdInterfaceItemCreate::undo()
{
    if (m_ifaceInfo.function)
        m_ifaceInfo.function->removeInterface(m_entity);
    if (m_ifaceInfo.model)
        m_ifaceInfo.model->removeObject(m_entity);

    for (QUndoCommand *clone : m_cmdClones)
        clone->undo();
}

bool CmdInterfaceItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdInterfaceItemCreate::id() const
{
    return CreateInterfaceEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
