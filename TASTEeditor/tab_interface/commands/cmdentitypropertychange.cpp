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

#include "cmdentitypropertychange.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

static inline QVariantHash getCurrentProperties(AADLObject *entity, const QVariantHash &props)
{
    QVariantHash result;
    for (auto it = props.constBegin(); it != props.constEnd(); ++it)
        result.insert(it.key(), entity->prop(it.key()));
    return result;
}

CmdEntityPropertyChange::CmdEntityPropertyChange(AADLObject *entity, const QVariantHash &props)
    : QUndoCommand()
    , m_entity(entity)
    , m_newProps(props)
    , m_oldProps(getCurrentProperties(entity, props))
{
}

void CmdEntityPropertyChange::redo()
{
    for (auto it = m_newProps.constBegin(); it != m_newProps.constEnd(); ++it)
        m_entity->setProp(it.key(), it.value());
}

void CmdEntityPropertyChange::undo()
{
    for (auto it = m_oldProps.constBegin(); it != m_oldProps.constEnd(); ++it)
        m_entity->setProp(it.key(), it.value());
}

bool CmdEntityPropertyChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntityPropertyChange::id() const
{
    return ChangeEntityProperties;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
