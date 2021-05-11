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

#include "cmdentitynamechange.h"

#include "commandids.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscentity.h"

namespace msc {
namespace cmd {

CmdEntityNameChange::CmdEntityNameChange(MscEntity *item, const QString &newName, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(item, layoutManager)
    , m_oldName(item->name())
    , m_newName(newName)
{
}

void CmdEntityNameChange::redo()
{
    if (m_modelItem) {
        m_modelItem->setName(m_newName);
        setChartDocumentName(m_newName);
        Q_EMIT nameChanged(m_modelItem, m_oldName, this);
    }

    m_firstRedo = false;
}

void CmdEntityNameChange::undo()
{
    if (m_modelItem) {
        m_modelItem->setName(m_oldName);
        setChartDocumentName(m_oldName);
        Q_EMIT nameChanged(m_modelItem, m_newName, this);
    }
}

bool CmdEntityNameChange::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdEntityNameChange *>(command);
    if (canMergeWith(other)) {
        m_newName = other->m_newName;
        return true;
    }

    return false;
}

int CmdEntityNameChange::id() const
{
    return msc::cmd::Id::RenameEntity;
}

void CmdEntityNameChange::setChartDocumentName(const QString &name)
{
    if (auto document = dynamic_cast<MscDocument *>(m_modelItem.data())) {
        if (!document->charts().empty()) {
            document->charts()[0]->setName(name);
        }
    } else if (auto chart = dynamic_cast<MscChart *>(m_modelItem.data())) {
        if (chart->parentDocument()) {
            chart->parentDocument()->setName(name);
        }
    }
}

} // namespace cmd
} // namespace msc
