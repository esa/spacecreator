/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmddeleteentity.h"

#include "common/commandids.h"
#include "mscchart.h"
#include "mscentity.h"
#include "mscinstanceevent.h"

namespace msc {
namespace cmd {

CmdDeleteEntity::CmdDeleteEntity(QVector<MscEntity *> items, msc::MscChart *chart)
    : QUndoCommand()
    , m_chart(chart)
{
    Q_ASSERT(m_chart);

    setText(QObject::tr("Delete"));

    for (auto item : items) {
        auto event = dynamic_cast<MscInstanceEvent *>(item);
        if (event) {
            const int idx = m_chart->instanceEvents().indexOf(event);
            m_items[idx] = event;
        }
    }
}

void CmdDeleteEntity::redo()
{
    Q_ASSERT(m_chart);
    for (auto event : m_items) {
        m_chart->removeInstanceEvent(event);
    }
}

void CmdDeleteEntity::undo()
{
    Q_ASSERT(m_chart);
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        const int idx = it.key();
        MscInstanceEvent *event = it.value();
        m_chart->addInstanceEvent(event, idx);
    }
}

bool CmdDeleteEntity::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdDeleteEntity::id() const
{
    return msc::cmd::Id::DeleteEntity;
}

} // namespace cmd
} // namespace msc
