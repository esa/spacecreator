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

#include "cmdifaceparamchange.h"

#include "commandids.h"

#include <aadlobjectsmodel.h>

namespace aadlinterface {
namespace cmd {

CmdIfaceParamChange::CmdIfaceParamChange(
        aadl::AADLObject *entity, const aadl::IfaceParameter &from, const aadl::IfaceParameter &to)
    : CmdIfaceParamBase(entity ? entity->as<aadl::AADLObjectIface *>() : nullptr)
    , m_newParam(to)
    , m_oldParam(from)
{
    setText(QObject::tr("Change Iface Parameter"));
}

void CmdIfaceParamChange::swapParam(const aadl::IfaceParameter &from, const aadl::IfaceParameter &to)
{
    if (!m_iface)
        return;

    auto params = m_iface->params();

    const int id = params.indexOf(from);
    if (id >= 0 && id < params.size()) {
        params.replace(id, to);
        m_iface->setParams(params);
    }
}

void CmdIfaceParamChange::redo()
{
    swapParam(m_oldParam, m_newParam);

    CmdIfaceParamBase::redo();
}

void CmdIfaceParamChange::undo()
{
    swapParam(m_newParam, m_oldParam);

    CmdIfaceParamBase::undo();
}

int CmdIfaceParamChange::id() const
{
    return ChangeIfaceParam;
}

}
}
