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

#include "cmddocumentmove.h"

#include "mscchart.h"
#include "mscdocument.h"

namespace msc {
namespace cmd {

CmdDocumentMove::CmdDocumentMove(msc::MscDocument *document, MscDocument *parentDocument)
    : BaseCommand(document)
    , m_document(document)
    , m_newParentDocument(parentDocument)
    , m_oldParentDocument(m_document->parentDocument())
    , m_newParentCharts(m_newParentDocument->charts())
    , m_documentIndex(m_oldParentDocument->documents().indexOf(m_document))
{
    Q_ASSERT(m_newParentDocument.data());

    setText(QObject::tr("Move document"));
}

void CmdDocumentMove::redo()
{
    if (m_document && m_newParentDocument) {
        clearDocumentCharts(m_newParentDocument);

        if (m_newParentDocument->addDocument(m_document)) {
            m_oldParentDocument->removeDocument(m_document, false);

            addDocumentChart(m_oldParentDocument);
        }
    }
}

void CmdDocumentMove::undo()
{
    Q_ASSERT(m_oldParentDocument.data());

    if (m_newParentDocument && m_oldParentDocument) {
        m_newParentDocument->removeDocument(m_document, false);
        // restore charts
        for (auto chart : m_newParentCharts)
            m_newParentDocument->addChart(chart);

        clearDocumentCharts(m_oldParentDocument);
        m_oldParentDocument->addDocument(m_document, m_documentIndex);
    }
}

bool CmdDocumentMove::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdDocumentMove::id() const
{
    return msc::cmd::Id::MoveDocument;
}

void CmdDocumentMove::clearDocumentCharts(MscDocument *document)
{
    if (!document->charts().empty()) {
        const QSignalBlocker blocker(document);
        document->clear();
    }
}

void CmdDocumentMove::addDocumentChart(MscDocument *document)
{
    if (document->documents().empty() && document->charts().empty()
            && document->hierarchyType() == MscDocument::HierarchyLeaf) {
        document->addChart(new MscChart(tr("msc")));
    }
}

} // namespace cmd
} // namespace msc
