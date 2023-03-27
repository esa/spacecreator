/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdentitiesreference.h"

#include "commands/commandids.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"

namespace ive {
namespace cmd {

CmdEntitiesReference::CmdEntitiesReference(
        ivm::IVFunctionType *entity, ivm::IVFunctionType *parent, ivm::IVModel *model, const QPointF &pos)
    : QUndoCommand()
    , m_parent(parent)
    , m_model(model)
    , m_referencedEntity(entity)
    , m_pos(pos)
    , m_ref(shared::createId())
{
    Q_ASSERT(entity);
}

CmdEntitiesReference::~CmdEntitiesReference() { }

void CmdEntitiesReference::redo()
{
    m_model->import(ivm::IVModel::ImportType::Reference, m_referencedEntity, m_parent, m_ref, m_pos);
}

void CmdEntitiesReference::undo()
{
    if (auto ref = m_model->getObject(m_referencedEntity->id())) {
        m_model->removeObject(ref);
        ref->deleteLater();
    }
}

bool CmdEntitiesReference::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntitiesReference::id() const
{
    return ReferenceEntities;
}

} // namespace cmd
} // namespace ive
