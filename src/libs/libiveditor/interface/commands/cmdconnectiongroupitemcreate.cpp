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

#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectifacegroup.h"
#include "aadlobjectsmodel.h"
#include "commandids.h"

namespace aadlinterface {
namespace cmd {

CmdConnectionGroupItemCreate::CmdConnectionGroupItemCreate(
        const aadl::AADLObjectConnectionGroup::CreationInfo &creationInfo)
    : CmdEntityGeometryChange({}, QObject::tr("Create Connection Group"))
    , m_groupName(creationInfo.name)
    , m_model(creationInfo.model)
    , m_parent(qobject_cast<aadl::AADLObjectFunction *>(creationInfo.parentObject))
    , m_connections(creationInfo.connections)
{
    Q_ASSERT(creationInfo.model);
    Q_ASSERT(creationInfo.sourceObject);
    Q_ASSERT(creationInfo.targetObject);
    Q_ASSERT(!creationInfo.connections.isEmpty());

    aadl::AADLObjectIface::CreationInfo sourceInfo;
    sourceInfo.model = m_model;
    sourceInfo.function = qobject_cast<aadl::AADLObjectFunction *>(creationInfo.sourceObject);
    m_sourceIface = new aadl::AADLObjectIfaceGroup(sourceInfo);

    aadl::AADLObjectIface::CreationInfo targetInfo;
    targetInfo.model = m_model;
    targetInfo.function = qobject_cast<aadl::AADLObjectFunction *>(creationInfo.targetObject);
    m_targetIface = new aadl::AADLObjectIfaceGroup(targetInfo);

    m_entity = new aadl::AADLObjectConnectionGroup(
            creationInfo.name, m_sourceIface, m_targetIface, m_connections, creationInfo.parentObject);
    prepareData({ qMakePair(m_entity, creationInfo.points) });
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
}

void CmdConnectionGroupItemCreate::redo()
{
    CmdEntityGeometryChange::redo();

    if (m_parent) {
        m_parent->addChild(m_sourceIface);
        m_parent->addChild(m_targetIface);
        m_parent->addChild(m_entity);
    }

    m_model->addObject(m_sourceIface);
    m_model->addObject(m_targetIface);
    m_model->addObject(m_entity);
}

void CmdConnectionGroupItemCreate::undo()
{
    CmdEntityGeometryChange::undo();

    m_model->removeObject(m_entity);
    m_model->removeObject(m_targetIface);
    m_model->removeObject(m_sourceIface);

    if (m_parent) {
        m_parent->removeChild(m_entity);
        m_parent->removeChild(m_targetIface);
        m_parent->removeChild(m_sourceIface);
    }
}

int CmdConnectionGroupItemCreate::id() const
{
    return CreateConnectionGroupEntity;
}

} // namespace aadlinterface
} // namespace cmd
