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

#include "cmddeleteentity.h"

#include "commandids.h"
#include "mscchart.h"
#include "msccomment.h"
#include "mscdocument.h"
#include "mscentity.h"
#include "mscinstance.h"
#include "mscinstanceevent.h"

namespace msc {
namespace cmd {

CmdDeleteEntity::CmdDeleteEntity(
        QVector<MscEntity *> items, msc::MscDocument *document, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(nullptr, layoutManager)
    , m_document(document)
{
    setText(QObject::tr("Delete"));

    initChartData(items);

    if (m_document && !items.empty()) {
        auto itemDocument = dynamic_cast<MscDocument *>(items[0]);
        if (itemDocument) {
            const int idx = m_document->documents().indexOf(itemDocument);
            m_entities[idx] = itemDocument;
        }
    }
}

CmdDeleteEntity::CmdDeleteEntity(QVector<MscEntity *> items, MscChart *chart)
    : ChartBaseCommand(nullptr, nullptr)
{
    m_chart = chart;
    setText(QObject::tr("Delete"));

    initChartData(items);
}

void CmdDeleteEntity::redo()
{
    if (m_chart) {
        for (auto event : qAsConst(m_deleteEvents)) {
            m_chart->removeInstanceEvent(event);
        }

        for (auto instance : qAsConst(m_entities)) {
            m_chart->removeInstance(dynamic_cast<MscInstance *>(instance));
        }
        checkVisualSorting();
    }

    if (m_document) {
        for (auto document : qAsConst(m_entities)) {
            m_document->removeDocument(dynamic_cast<MscDocument *>(document), false);
        }

        if (m_document->documents().empty() && m_document->charts().empty()) {
            m_document->addChart(new MscChart(m_document->name() + QObject::tr("_msc")));
        }
    }
}

void CmdDeleteEntity::undo()
{
    if (m_chart) {
        for (auto it = m_entities.cbegin(); it != m_entities.cend(); ++it) {
            const int idx = it.key();
            MscInstance *instance = dynamic_cast<MscInstance *>(it.value());
            m_chart->addInstance(instance, idx);
        }

        m_chart->setInstanceEvents(m_events, m_orphanEvents);
        undoVisualSorting();
    }

    if (m_document) {
        if (!m_document->charts().empty()) {
            m_document->blockSignals(true);
            m_document->clear();
            m_document->blockSignals(false);
        }

        for (auto it = m_entities.cbegin(); it != m_entities.cend(); ++it) {
            const int idx = it.key();
            MscDocument *document = dynamic_cast<MscDocument *>(it.value());
            m_document->addDocument(document, idx);
        }
    }
}

bool CmdDeleteEntity::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdDeleteEntity::id() const
{
    return msc::cmd::Id::DeleteEntity;
}

void CmdDeleteEntity::initChartData(const QVector<MscEntity *> &items)
{
    if (!m_chart) {
        return;
    }

    m_orphanEvents = m_chart->orphanEvents();
    m_events = m_chart->rawEvents();

    auto fetchRelatedComments = [this](MscEntity *entity) {
        if (entity) {
            if (MscInstanceEvent *commentEntity = entity->comment()) {
                m_deleteEvents.append(commentEntity);
            }
        }
    };

    for (auto item : items) {
        auto event = dynamic_cast<MscInstanceEvent *>(item);
        if (event) {
            m_deleteEvents.append(event);
        }
        auto instance = dynamic_cast<MscInstance *>(item);
        if (instance) {
            const int idx = m_chart->instances().indexOf(instance);
            m_entities[idx] = instance;
        }
        fetchRelatedComments(item);
    }

    // now add all events that depend on instances that are deleted
    for (auto entity : qAsConst(m_entities)) {
        auto instance = dynamic_cast<MscInstance *>(entity);
        m_chart->removeInstance(instance);
        for (auto event : m_chart->instanceEvents()) {
            if (event->relatesTo(instance)) {
                m_deleteEvents.append(event);
                fetchRelatedComments(event);
            }
        }
    }
}

} // namespace cmd
} // namespace msc
