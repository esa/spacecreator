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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cmdinstanceitemcreate.h"

#include "common/commandids.h"
#include "mscchart.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdInstanceItemCreate::CmdInstanceItemCreate(msc::MscInstance *instance, msc::MscChart *chart, int orderId)
    : BaseCommand(instance)
    , m_instance(instance)
    , m_chart(chart)
    , m_instanceOrderNum(orderId)
{
    Q_ASSERT(m_chart.data());

    setText(tr("Add instance"));
}

void CmdInstanceItemCreate::redo()
{
    Q_ASSERT(m_chart.data());

    if (!m_instance) {
        m_instance = m_chart->makeInstance();
        m_modelItem = m_instance;
    }

    // The chart takes over parent-/owner-ship
    m_chart->addInstance(m_instance, m_instanceOrderNum);
}

void CmdInstanceItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstance(m_instance);

    // this command takes over ownership
    m_instance->setParent(this);
}

bool CmdInstanceItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdInstanceItemCreate::id() const
{
    return msc::cmd::Id::CreateInstance;
}

} // ns cmd
} // ns msc
