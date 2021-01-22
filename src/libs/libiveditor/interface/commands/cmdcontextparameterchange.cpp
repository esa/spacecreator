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

#include "cmdcontextparameterchange.h"

#include "commandids.h"

#include <QDebug>
#include <aadlobjectfunctiontype.h>
#include <aadlobjectsmodel.h>

namespace ive {
namespace cmd {

CmdContextParameterChange::CmdContextParameterChange(aadl::AADLObjectFunctionType *entity,
        const aadl::ContextParameter &oldParam, const aadl::ContextParameter &newParam)
    : QUndoCommand()
    , m_entity(entity)
    , m_newParam(newParam)
    , m_oldParam(oldParam)
{
    setText(QObject::tr("Change Context Parameter"));
}

void CmdContextParameterChange::swapParam(const aadl::ContextParameter &from, const aadl::ContextParameter &to)
{
    if (!m_entity)
        return;

    QVector<aadl::ContextParameter> params = m_entity->contextParams();

    const int id = params.indexOf(from);
    if (id >= 0 && id < params.size()) {
        params.replace(id, to);
        m_entity->setContextParams(params);
    }
}

void CmdContextParameterChange::redo()
{
    swapParam(m_oldParam, m_newParam);
}

void CmdContextParameterChange::undo()
{
    swapParam(m_newParam, m_oldParam);
}

bool CmdContextParameterChange::mergeWith(const QUndoCommand * /*command*/)
{
    return false;
}

int CmdContextParameterChange::id() const
{
    return ChangeContextParameter;
}

}
}
