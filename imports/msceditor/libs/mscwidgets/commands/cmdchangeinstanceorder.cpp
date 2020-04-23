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

#include "cmdchangeinstanceorder.h"

#include "mscchart.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdChangeInstanceOrder::CmdChangeInstanceOrder(msc::MscInstance *instance, int pos, MscChart *chart)
    : BaseCommand(instance)
    , m_instance(instance)
    , m_chart(chart)
    , m_posFrom(chart->instances().indexOf(m_instance))
    , m_posTo(pos)
{
    setText(QObject::tr("Move instance"));
}

void CmdChangeInstanceOrder::redo()
{
    if (m_chart && m_instance)
        m_chart->updateInstanceOrder(m_instance, m_posTo);
}

void CmdChangeInstanceOrder::undo()
{
    if (m_chart && m_instance)
        m_chart->updateInstanceOrder(m_instance, m_posFrom);
}

bool CmdChangeInstanceOrder::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdChangeInstanceOrder::id() const
{
    return msc::cmd::Id::ReorderInstance;
}

} // namespace cmd
} // namespace msc
