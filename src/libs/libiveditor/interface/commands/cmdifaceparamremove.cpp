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

#include "cmdifaceparamremove.h"

#include "commandids.h"

#include <aadlobjectsmodel.h>
#include <aadlparameter.h>

namespace ive {
namespace cmd {

CmdIfaceParamRemove::CmdIfaceParamRemove(aadl::AADLObject *entity, const aadl::IfaceParameter &param)
    : CmdIfaceParamCreate(entity, param)
{
    setText(QObject::tr("Remove Iface Parameter"));
}

void CmdIfaceParamRemove::redo()
{
    if (!m_iface)
        return;

    QVector<aadl::IfaceParameter> currParams = m_iface->params();
    for (const aadl::IfaceParameter &param : m_targetParams)
        currParams.removeAll(param);
    m_iface->setParams(currParams);

    CmdIfaceParamBase::redo();
}

void CmdIfaceParamRemove::undo()
{
    if (!m_iface)
        return;

    m_iface->setParams(m_sourceParams);

    CmdIfaceParamBase::undo();
}

int CmdIfaceParamRemove::id() const
{
    return RemoveIfaceParam;
}

}
}
