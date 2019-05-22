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

#include "common/commandids.h"
#include "mscchart.h"
#include "msccomment.h"
#include "mscdocument.h"
#include "mscentity.h"
#include "mscinstance.h"
#include "mscinstanceevent.h"

namespace msc {
namespace cmd {

CmdDeleteEntity::CmdDeleteEntity(QVector<MscEntity *> items, msc::MscChart *chart, msc::MscDocument *document)
    : QUndoCommand()
    , m_chart(chart)
    , m_document(document)
{
    setText(QObject::tr("Delete"));

    if (m_chart) {
        auto fetchRelatedComments = [this](MscEntity *entity) {
            if (MscInstanceEvent *commentEntity = entity->comment()) {
                const int idx = m_chart->instanceEvents().indexOf(commentEntity);
                m_events[idx] = commentEntity;
            }
        };

        for (auto item : items) {
            auto event = dynamic_cast<MscInstanceEvent *>(item);
            if (event) {
                const int idx = m_chart->instanceEvents().indexOf(event);
                m_events[idx] = event;
            }
            auto instance = dynamic_cast<MscInstance *>(item);
            if (instance) {
                const int idx = m_chart->instances().indexOf(instance);
                m_entities[idx] = instance;
            }
            fetchRelatedComments(item);
        }

        // now add all events that depend on instances that are deleted
        for (auto entity : m_entities) {
            auto instance = dynamic_cast<MscInstance *>(entity);
            m_chart->removeInstance(instance);
            for (auto event : m_chart->instanceEvents()) {
                if (event->relatesTo(instance)) {
                    const int idx = m_chart->instanceEvents().indexOf(event);
                    m_events[idx] = event;
                    fetchRelatedComments(event);
                }
            }
        }

    } else if (m_document && !items.empty()) {
        auto document = dynamic_cast<MscDocument *>(items[0]);
        if (document) {
            const int idx = m_document->documents().indexOf(document);
            m_entities[idx] = document;
        }
    }
}

void CmdDeleteEntity::redo()
{
    if (m_chart) {
        for (auto event : m_events) {
            m_chart->removeInstanceEvent(event);
        }

        for (auto instance : m_entities) {
            m_chart->removeInstance(dynamic_cast<MscInstance *>(instance));
        }
    } else if (m_document) {
        for (auto document : m_entities) {
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

        for (auto it = m_events.cbegin(); it != m_events.cend(); ++it) {
            const int idx = it.key();
            MscInstanceEvent *event = it.value();
            m_chart->addInstanceEvent(event, idx);
        }
    } else if (m_document) {
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
    Q_UNUSED(command);
    return false;
}

int CmdDeleteEntity::id() const
{
    return msc::cmd::Id::DeleteEntity;
}

} // namespace cmd
} // namespace msc
