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

#include "cmdentitypropertyrename.h"

#include "commandids.h"

#include <aadlobjectsmodel.h>

namespace aadlinterface {
namespace cmd {

CmdEntityPropertyRename::CmdEntityPropertyRename(aadl::AADLObject *entity, const QHash<QString, QString> &props)
    : QUndoCommand()
    , m_entity(entity)
    , m_newProps(props)
{
    setText(QObject::tr("Rename Property"));
}

void CmdEntityPropertyRename::redo()
{
    QVariantHash props = m_entity->props();
    for (auto it = m_newProps.constBegin(); it != m_newProps.constEnd(); ++it) {
        const QString &from = it.key();
        const QString &to = it.value();
        const QVariant &value = props.take(from);
        props.insert(to, value);
    }
    m_entity->setProps(props);
}

void CmdEntityPropertyRename::undo()
{
    QVariantHash props = m_entity->props();
    for (auto it = m_newProps.constBegin(); it != m_newProps.constEnd(); ++it) {
        const QString &from = it.value();
        const QString &to = it.key();
        const QVariant &value = props.take(from);
        props.insert(to, value);
    }
    m_entity->setProps(props);
}

bool CmdEntityPropertyRename::mergeWith(const QUndoCommand *)
{
    return false;
}

int CmdEntityPropertyRename::id() const
{
    return RenameEntityProperty;
}

}
}
