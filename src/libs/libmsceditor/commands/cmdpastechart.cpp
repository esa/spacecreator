/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdpastechart.h"

#include "commandids.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmodel.h"
#include "mscreader.h"

namespace msc {
namespace cmd {

CmdPasteChart::CmdPasteChart(msc::MscDocument *document, const QString &copyChartText)
    : BaseCommand(document)
    , m_document(document)
{
    Q_ASSERT(m_document.data());

    setText(QObject::tr("Paste chart"));

    m_copyModel = MscReader().parseText(copyChartText);

    if (m_document && m_document->charts().size() == 1) {
        m_chart = m_document->charts()[0];
    }
}

void CmdPasteChart::redo()
{
    if (m_document && m_copyModel && m_copyModel->charts().size() == 1) {
        const auto mscChart = m_copyModel->charts()[0];

        if (m_document->documents().size()) {
            if (!m_leafDoument) {
                // create leaf type document
                m_leafDoument = new msc::MscDocument(mscChart->name());
                m_leafDoument->setHierarchyType(MscDocument::HierarchyLeaf);
                m_leafDoument->addChart(mscChart);
            }

            m_document->addDocument(m_leafDoument);
        } else if (m_chart) {
            for (auto instance : mscChart->instances()) {
                m_chart->addInstance(instance);
            }

            for (auto instanceEvent : mscChart->instanceEvents()) {
                m_chart->addInstanceEvent(instanceEvent);
            }

            for (auto gate : mscChart->gates()) {
                m_chart->addGate(gate);
            }
        }
    }
}

void CmdPasteChart::undo()
{
    if (m_document && m_copyModel && m_copyModel->charts().size() == 1) {
        if (m_document->documents().size()) {
            m_document->removeDocument(m_leafDoument, false);
        } else if (m_chart) {
            const auto mscChart = m_copyModel->charts()[0];

            for (auto instance : mscChart->instances()) {
                m_chart->removeInstance(instance);
            }

            for (auto instanceEvent : mscChart->instanceEvents()) {
                m_chart->removeInstanceEvent(instanceEvent);
            }

            for (auto gate : mscChart->gates()) {
                m_chart->removeGate(gate);
            }
        }
    }
}

bool CmdPasteChart::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdPasteChart::id() const
{
    return msc::cmd::Id::PasteChart;
}

} // namespace cmd
} // namespace msc
