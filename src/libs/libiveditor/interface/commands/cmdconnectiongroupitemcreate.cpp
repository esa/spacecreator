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

#include "cmdconnectiongroupitemcreate.h"

#include "aadlconnectiongroup.h"
#include "aadlfunction.h"
#include "aadlifacegroup.h"
#include "aadlmodel.h"
#include "cmdconnectiongroupitemchange.h"
#include "commandids.h"

namespace ive {
namespace cmd {

CmdConnectionGroupItemCreate::CmdConnectionGroupItemCreate(
        const ivm::AADLConnectionGroup::CreationInfo &creationInfo)
    : CmdEntityGeometryChange({}, QObject::tr("Create Connection Group"))
    , m_groupName(creationInfo.name)
    , m_model(creationInfo.model)
    , m_parent(qobject_cast<ivm::AADLFunction *>(creationInfo.parentObject))
    , m_sourceIfaceParent(qobject_cast<ivm::AADLFunction *>(creationInfo.sourceObject))
    , m_targetIfaceParent(qobject_cast<ivm::AADLFunction *>(creationInfo.targetObject))
{
    Q_ASSERT(creationInfo.model);
    Q_ASSERT(creationInfo.sourceObject);
    Q_ASSERT(creationInfo.targetObject);
    Q_ASSERT(!creationInfo.connections.isEmpty());

    ivm::AADLIface::CreationInfo sourceInfo;
    sourceInfo.model = m_model;
    sourceInfo.function = m_sourceIfaceParent;
    m_sourceIface = new ivm::AADLIfaceGroup(sourceInfo);

    ivm::AADLIface::CreationInfo targetInfo;
    targetInfo.model = m_model;
    targetInfo.function = m_targetIfaceParent;
    m_targetIface = new ivm::AADLIfaceGroup(targetInfo);

    m_entity = new ivm::AADLConnectionGroup(
            creationInfo.name, m_sourceIface, m_targetIface, {}, creationInfo.parentObject);
    prepareData({ qMakePair(m_entity, creationInfo.points) });

    for (auto conn : creationInfo.connections) {
        if (conn) {
            m_subCommands.append(new CmdConnectionGroupItemChange(m_entity, conn, true));
        }
    }
}

CmdConnectionGroupItemCreate::~CmdConnectionGroupItemCreate()
{
    if (m_sourceIface && !m_sourceIface->parent()) {
        delete m_sourceIface;
    }
    if (m_targetIface && !m_targetIface->parent()) {
        delete m_targetIface;
    }
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }

    qDeleteAll(m_subCommands);
    m_subCommands.clear();
}

void CmdConnectionGroupItemCreate::redo()
{
    CmdEntityGeometryChange::redo();

    if (m_sourceIfaceParent) {
        m_sourceIfaceParent->addChild(m_sourceIface);
    }
    if (m_targetIfaceParent) {
        m_targetIfaceParent->addChild(m_targetIface);
    }
    if (m_parent) {
        m_parent->addChild(m_entity);
    }

    m_model->addObject(m_sourceIface);
    m_model->addObject(m_targetIface);
    m_model->addObject(m_entity);

    for (auto it = m_subCommands.begin(); it != m_subCommands.end(); ++it) {
        (*it)->redo();
    }
}

void CmdConnectionGroupItemCreate::undo()
{
    for (auto it = m_subCommands.rbegin(); it != m_subCommands.rend(); ++it) {
        (*it)->undo();
    }

    CmdEntityGeometryChange::undo();

    m_model->removeObject(m_entity);
    m_model->removeObject(m_targetIface);
    m_model->removeObject(m_sourceIface);

    if (m_parent) {
        m_parent->removeChild(m_entity);
    }
    if (m_targetIfaceParent) {
        m_targetIfaceParent->removeChild(m_targetIface);
    }
    if (m_sourceIfaceParent) {
        m_sourceIfaceParent->removeChild(m_sourceIface);
    }
}

int CmdConnectionGroupItemCreate::id() const
{
    return CreateConnectionGroupEntity;
}

} // namespace ive
} // namespace cmd
