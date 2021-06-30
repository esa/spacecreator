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
#include "veobject.h"

namespace shared {
namespace cmd {

static inline QVariantHash getCurrentProperties(VEObject *entity, const QVariantHash &props)
{
    QVariantHash result;
    for (auto it = props.constBegin(); it != props.constEnd(); ++it)
        result.insert(it.key(), entity->entityAttributeValue(it.key()));
    return result;
}

CmdEntityPropertyChange::CmdEntityPropertyChange(VEObject *entity, const QVariantHash &props)
    : QUndoCommand()
    , m_entity(entity)
    , m_newProps(props)
    , m_oldProps(getCurrentProperties(entity, props))
{
    setText(QObject::tr("Change Property"));
}

void CmdEntityPropertyChange::redo()
{
    for (auto it = m_newProps.constBegin(); it != m_newProps.constEnd(); ++it)
        m_entity->setEntityProperty(it.key(), it.value());
}

void CmdEntityPropertyChange::undo()
{
    for (auto it = m_oldProps.constBegin(); it != m_oldProps.constEnd(); ++it)
        m_entity->setEntityProperty(it.key(), it.value());
}

int CmdEntityPropertyChange::id() const
{
    return ChangeEntityProperty;
}

} // namespace cmd
} // namespace shared
