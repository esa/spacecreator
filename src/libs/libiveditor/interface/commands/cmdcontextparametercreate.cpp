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

#include "cmdcontextparametercreate.h"

#include "commandids.h"

#include <QDebug>
#include <aadlobjectsmodel.h>

namespace aadlinterface {
namespace cmd {

CmdContextParameterCreate::CmdContextParameterCreate(
        aadl::AADLObjectFunctionType *entity, const aadl::ContextParameter &prop)
    : QUndoCommand()
    , m_entity(entity)
    , m_newProps({ prop })
    , m_oldProps(entity->contextParams())
{
    setText(QObject::tr("Create Context Parameter"));
}

void CmdContextParameterCreate::redo()
{
    if (!m_entity)
        return;

    QVector<aadl::ContextParameter> params = m_oldProps;
    for (const aadl::ContextParameter &param : m_newProps)
        params.append(param);
    m_entity->setContextParams(params);
}

void CmdContextParameterCreate::undo()
{
    m_entity->setContextParams(m_oldProps);
}

bool CmdContextParameterCreate::mergeWith(const QUndoCommand *command)
{
    if (command->id() == id()) {
        m_newProps += static_cast<const CmdContextParameterCreate *>(command)->m_newProps;
        return true;
    }
    return false;
}

int CmdContextParameterCreate::id() const
{
    return CreateContextParameter;
}

}
}
