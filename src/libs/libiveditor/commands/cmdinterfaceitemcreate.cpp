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
#include "itemeditor/common/ivutils.h"
#include "ivfunction.h"
#include "ivmodel.h"

#include <QDebug>

namespace ive {
namespace cmd {

QVector<QUndoCommand *> fillCloneCommands(ivm::IVInterface *iface, const ivm::IVInterface::CreationInfo &creationInfo)
{
    QVector<QUndoCommand *> clones;

    if (!iface || !iface->parentObject())
        return clones;

    if (auto fnType = iface->parentObject()->as<const ivm::IVFunctionType *>())
        for (const auto &fn : fnType->instances()) {
            ivm::IVInterface::CreationInfo clone = ivm::IVInterface::CreationInfo::cloneIface(iface, fn);
            // the cloned iface has not been stored yet,
            // so it has invalid pointer to the model
            clone.model = creationInfo.model;
            auto cmd = new cmd::CmdInterfaceItemCreate(clone);
            clones.append(cmd);
        }

    return clones;
}

CmdInterfaceItemCreate::CmdInterfaceItemCreate(const ivm::IVInterface::CreationInfo &creationInfo)
    : shared::cmd::CmdEntityGeometryChange({},
            creationInfo.type == ivm::IVInterface::InterfaceType::Provided ? QObject::tr("Create PI")
                                                                           : QObject::tr("Create RI"))
    , m_ifaceInfo(creationInfo)
    , m_entity(ivm::IVInterface::createIface(m_ifaceInfo))
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
    shared::cmd::CmdEntityGeometryChange::redo();

    if (m_ifaceInfo.function)
        m_ifaceInfo.function->addChild(m_entity);
    if (m_ifaceInfo.model)
        m_ifaceInfo.model->addObject(m_entity);

    for (QUndoCommand *clone : m_cmdClones)
        clone->redo();
}

void CmdInterfaceItemCreate::undo()
{
    shared::cmd::CmdEntityGeometryChange::undo();

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

ivm::IVInterface *CmdInterfaceItemCreate::createdInterface() const
{
    return m_entity;
}

}
}
