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

#include "cmdinstanceitemmove.h"

#include "mscchart.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdInstanceItemMove::CmdInstanceItemMove(msc::MscInstance *instance, int pos, MscChart *chart)
    : BaseCommand(instance)
    , m_instance(instance)
    , m_chart(chart)
    , m_posFrom(chart->instances().indexOf(m_instance))
    , m_posTo(pos)
{
    setText(QObject::tr("Move instance"));
}

void CmdInstanceItemMove::redo()
{
    if (m_chart && m_instance)
        m_chart->updateInstancePos(m_instance, m_posTo);
}

void CmdInstanceItemMove::undo()
{
    if (m_chart && m_instance)
        m_chart->updateInstancePos(m_instance, m_posFrom);
}

bool CmdInstanceItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdInstanceItemMove *other = static_cast<const CmdInstanceItemMove *>(command);
    if (canMergeWith(other)) {
        m_posTo = other->m_posTo;
        return true;
    }

    return false;
}

int CmdInstanceItemMove::id() const
{
    return msc::cmd::Id::MoveInstance;
}

} // namespace cmd
} // namespace msc
