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

#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "commandids.h"
#include "commandsfactory.h"

#include <QDebug>

namespace aadlinterface {
namespace cmd {

QVector<QUndoCommand *> fillCloneCommands(
        aadl::AADLObjectIface *iface, const aadl::AADLObjectIface::CreationInfo &creationInfo)
{
    QVector<QUndoCommand *> clones;

    if (!iface || !iface->parentObject())
        return clones;

    if (auto fnType = iface->parentObject()->as<const aadl::AADLObjectFunctionType *>())
        for (const auto &fn : fnType->instances()) {
            aadl::AADLObjectIface::CreationInfo clone = aadl::AADLObjectIface::CreationInfo::cloneIface(iface, fn);
            // the cloned iface has not been stored yet,
            // so it has invalid pointer to the model
            clone.model = creationInfo.model;
            if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, clone.toVarList()))
                clones.append(cmdRm);
        }

    return clones;
}

CmdInterfaceItemCreate::CmdInterfaceItemCreate(const aadl::AADLObjectIface::CreationInfo &creationInfo)
    : CmdEntityGeometryChange({},
            creationInfo.type == aadl::AADLObjectIface::IfaceType::Provided ? QObject::tr("Create PI")
                                                                            : QObject::tr("Create RI"))
    , m_ifaceInfo(creationInfo)
    , m_entity(aadl::AADLObjectIface::createIface(m_ifaceInfo))
    , m_cmdClones(fillCloneCommands(m_entity, m_ifaceInfo))
{
    prepareData({ qMakePair(m_entity, QVector<QPointF> { m_ifaceInfo.position }) });
}

CmdInterfaceItemCreate::~CmdInterfaceItemCreate()
{
    qDeleteAll(m_cmdClones);
}

void CmdInterfaceItemCreate::redo()
{
    CmdEntityGeometryChange::redo();

    if (m_ifaceInfo.function)
        m_ifaceInfo.function->addChild(m_entity);
    if (m_ifaceInfo.model)
        m_ifaceInfo.model->addObject(m_entity);

    for (QUndoCommand *clone : m_cmdClones)
        clone->redo();
}

void CmdInterfaceItemCreate::undo()
{
    CmdEntityGeometryChange::undo();

    if (m_ifaceInfo.function)
        m_ifaceInfo.function->removeChild(m_entity);
    if (m_ifaceInfo.model)
        m_ifaceInfo.model->removeObject(m_entity);

    for (QUndoCommand *clone : m_cmdClones)
        clone->undo();
}

int CmdInterfaceItemCreate::id() const
{
    return CreateInterfaceEntity;
}

}
}
