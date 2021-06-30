/*
  Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdentitypropertyrename.h"

#include "commandids.h"
#include "veobject.h"

namespace shared {
namespace cmd {

CmdEntityPropertyRename::CmdEntityPropertyRename(VEObject *entity, const QHash<QString, QString> &props)
    : QUndoCommand()
    , m_entity(entity)
    , m_newProps(props)
{
    setText(QObject::tr("Rename Property"));
}

void CmdEntityPropertyRename::redo()
{
    EntityAttributes props = m_entity->entityAttributes();
    for (auto it = m_newProps.constBegin(); it != m_newProps.constEnd(); ++it) {
        const QString &from = it.key();
        const QString &to = it.value();
        const EntityAttribute &attr = props.take(from);
        props.insert(to, EntityAttribute(to, attr.value(), attr.type()));
    }
    m_entity->setEntityAttributes(props);
}

void CmdEntityPropertyRename::undo()
{
    EntityAttributes props = m_entity->entityAttributes();
    for (auto it = m_newProps.constBegin(); it != m_newProps.constEnd(); ++it) {
        const QString &from = it.value();
        const QString &to = it.key();
        const EntityAttribute &attr = props.take(from);
        props.insert(to, EntityAttribute(to, attr.value(), attr.type()));
    }
    m_entity->setEntityAttributes(props);
}

int CmdEntityPropertyRename::id() const
{
    return RenameEntityProperty;
}

} // namespace cmd
} // namespace shared
