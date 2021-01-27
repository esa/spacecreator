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

#include "aadlfunction.h"
#include "aadlmodel.h"
#include "baseitems/common/aadlutils.h"
#include "cmdinterfaceitemcreate.h"
#include "commandids.h"

#include <QDebug>

namespace ive {
namespace cmd {

QVector<QUndoCommand *> fillCloneCommands(ivm::AADLIface *iface, const ivm::AADLIface::CreationInfo &creationInfo)
{
    QVector<QUndoCommand *> clones;

    if (!iface || !iface->parentObject())
        return clones;

    if (auto fnType = iface->parentObject()->as<const ivm::AADLFunctionType *>())
        for (const auto &fn : fnType->instances()) {
            ivm::AADLIface::CreationInfo clone = ivm::AADLIface::CreationInfo::cloneIface(iface, fn);
            // the cloned iface has not been stored yet,
            // so it has invalid pointer to the model
            clone.model = creationInfo.model;
            auto cmd = new cmd::CmdInterfaceItemCreate(clone);
            clones.append(cmd);
        }

    return clones;
}

CmdInterfaceItemCreate::CmdInterfaceItemCreate(const ivm::AADLIface::CreationInfo &creationInfo)
    : CmdEntityGeometryChange({},
            creationInfo.type == ivm::AADLIface::IfaceType::Provided ? QObject::tr("Create PI")
                                                                     : QObject::tr("Create RI"))
    , m_ifaceInfo(creationInfo)
    , m_entity(ivm::AADLIface::createIface(m_ifaceInfo))
    , m_cmdClones(fillCloneCommands(m_entity, m_ifaceInfo))
{
    prepareData({ qMakePair(m_entity, QVector<QPointF> { m_ifaceInfo.position }) });
}

CmdInterfaceItemCreate::~CmdInterfaceItemCreate()
{
    qDeleteAll(m_cmdClones);
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }
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

ivm::AADLIface *CmdInterfaceItemCreate::createdInterface() const
{
    return m_entity;
}

}
}
