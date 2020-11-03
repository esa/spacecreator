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

#include "cmdentitiesinstantiate.h"

#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "cmdentityattributechange.h"
#include "commandids.h"

namespace aadlinterface {
namespace cmd {

CmdEntitiesInstantiate::CmdEntitiesInstantiate(aadl::AADLObjectFunctionType *entity,
        aadl::AADLObjectFunctionType *parent, aadl::AADLObjectsModel *model, const QPointF &pos)
    : QUndoCommand()
    , m_parent(parent)
    , m_model(model)

{
    Q_ASSERT(entity);
    m_instantiatedEntity = new aadl::AADLObjectFunction(entity->title() + QLatin1String("Instance"));
    QRectF typeGeometry = aadlinterface::rect(entity->coordinates());
    typeGeometry.moveTopLeft(pos);
    m_instantiatedEntity->setCoordinates(aadlinterface::coordinates(typeGeometry));
    const QString nameKey = aadl::meta::Props::token(aadl::meta::Props::Token::instance_of);
    m_subCmd = new CmdEntityAttributeChange(m_instantiatedEntity, { { nameKey, entity->title() } });
}

CmdEntitiesInstantiate::~CmdEntitiesInstantiate()
{
    delete m_subCmd;
    m_subCmd = nullptr;

    if (m_instantiatedEntity && !m_instantiatedEntity->parent())
        delete m_instantiatedEntity;
}

void CmdEntitiesInstantiate::redo()
{
    if (!m_instantiatedEntity.isNull()) {
        if (m_parent) {
            m_parent->addChild(m_instantiatedEntity);
        }
        m_model->addObject(m_instantiatedEntity);
        m_subCmd->redo();
    }
}

void CmdEntitiesInstantiate::undo()
{
    if (!m_instantiatedEntity.isNull()) {
        m_subCmd->undo();
        m_model->removeObject(m_instantiatedEntity);
        if (m_parent) {
            m_parent->removeChild(m_instantiatedEntity);
        } else {
            m_instantiatedEntity->setParentObject(nullptr);
        }
    }
}

bool CmdEntitiesInstantiate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntitiesInstantiate::id() const
{
    return InstantiateEntities;
}

} // namespace aadlinterface
} // namespace cmd
