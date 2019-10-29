/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cmdentityattributechange.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

static inline QVariantHash getCurrentAttributes(AADLObject *entity, const QVariantHash &attrs)
{
    QVariantHash result;
    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
        result.insert(it.key(), entity->attr(it.key()));
    return result;
}

CmdEntityAttributeChange::CmdEntityAttributeChange(AADLObject *entity, const QVariantHash &attrs)
    : QUndoCommand()
    , m_entity(entity)
    , m_newAttrs(attrs)
    , m_oldAttrs(getCurrentAttributes(entity, attrs))
{
}

void CmdEntityAttributeChange::redo()
{
    for (auto it = m_newAttrs.constBegin(); it != m_newAttrs.constEnd(); ++it)
        m_entity->setAttr(it.key(), it.value());
}

void CmdEntityAttributeChange::undo()
{
    for (auto it = m_oldAttrs.constBegin(); it != m_oldAttrs.constEnd(); ++it)
        m_entity->setAttr(it.key(), it.value());
}

bool CmdEntityAttributeChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntityAttributeChange::id() const
{
    return ChangeEntityAttributes;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
