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

#include "cmdhierarchytypechange.h"

#include "mscchart.h"

namespace msc {
namespace cmd {

CmdHierarchyTypeChange::CmdHierarchyTypeChange(MscDocument *document, const MscDocument::HierarchyType newType)
    : BaseCommand(document)
    , m_document(document)
    , m_oldType(document->hierarchyType())
    , m_newType(newType)
{
}

void CmdHierarchyTypeChange::redo()
{
    if (m_document) {
        m_document->setHierarchyType(m_newType);

        if (m_newType == MscDocument::HierarchyLeaf && m_document->documents().empty()
                && m_document->charts().empty()) {
            m_document->addChart(new MscChart(m_document->name() + QObject::tr("_msc")));
            m_newChart = true;
        }
    }
}

void CmdHierarchyTypeChange::undo()
{
    if (m_document) {
        m_document->setHierarchyType(m_oldType);

        if (m_newChart) {
            m_document->clearCharts();
        }
    }
}

bool CmdHierarchyTypeChange::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdHierarchyTypeChange *>(command);
    if (canMergeWith(other)) {
        m_newType = other->m_newType;
        return true;
    }

    return false;
}

int CmdHierarchyTypeChange::id() const
{
    return msc::cmd::Id::HierarchyType;
}

} // namespace cmd
} // namespace msc
