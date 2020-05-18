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

#include "cmddocumentcreate.h"

#include "common/commandids.h"
#include "hierarchyviewmodel.h"
#include "mscchart.h"
#include "mscdocument.h"

namespace msc {
namespace cmd {

CmdDocumentCreate::CmdDocumentCreate(msc::MscDocument *document, MscDocument *parentDocument)
    : BaseCommand(document)
    , m_document(document)
    , m_parentDocument(parentDocument)
    , m_parentCharts(m_parentDocument->charts())
{
    Q_ASSERT(m_parentDocument.data());

    setText(QObject::tr("Add document"));
}

void CmdDocumentCreate::redo()
{
    if (!m_document) {
        m_document = new MscDocument(QObject::tr("Document_%1").arg(m_parentDocument->documents().size()));
        m_modelItem = m_document;
    }

    if (m_parentDocument) {
        if (!m_parentDocument->charts().empty()) {
            QSignalBlocker blocker(m_parentDocument);
            m_parentDocument->clear();
        }

        if (m_document->charts().empty() && m_document->hierarchyType() == MscDocument::HierarchyLeaf) {
            m_document->addChart(new MscChart(m_document->name() + QObject::tr("_msc")));
        }

        m_parentDocument->addDocument(m_document);
    }
}

void CmdDocumentCreate::undo()
{
    Q_ASSERT(m_parentDocument.data());

    if (m_parentDocument) {
        m_parentDocument->removeDocument(m_document);

        if (m_parentDocument->documents().empty() && m_parentDocument->charts().empty()) {
            for (auto chart : m_parentCharts)
                m_parentDocument->addChart(chart);
        }
    }
}

bool CmdDocumentCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdDocumentCreate::id() const
{
    return msc::cmd::Id::CreateDocument;
}

} // namespace cmd
} // namespace msc
