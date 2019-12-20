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

#include "cmdifaceparamcreate.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdIfaceParamCreate::CmdIfaceParamCreate(AADLObject *entity, const IfaceParameter &param)
    : QUndoCommand()
    , m_iface(qobject_cast<AADLObjectIface *>(entity))
    , m_targetParams({ param })
    , m_sourceParams(m_iface ? m_iface->params() : QVector<IfaceParameter>())
{
    setText(QObject::tr("Create Iface Parameter"));
}

void CmdIfaceParamCreate::redo()
{
    if (!m_iface)
        return;

    QVector<IfaceParameter> currParams = m_iface->params();
    currParams.append(m_targetParams);
    m_iface->setParams(currParams);
}

void CmdIfaceParamCreate::undo()
{
    if (!m_iface)
        return;

    m_iface->setParams(m_sourceParams);
}

bool CmdIfaceParamCreate::mergeWith(const QUndoCommand *command)
{
    if (command->id() == id()) {
        const QVector<IfaceParameter> &update = static_cast<const CmdIfaceParamCreate *>(command)->m_targetParams;
        m_targetParams.append(update);
        return true;
    }
    return false;
}

int CmdIfaceParamCreate::id() const
{
    return CreateIfaceParam;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
