/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "eventmovebasecommand.h"

#include "baseitems/interactiveobject.h"
#include "chartlayoutmanager.h"
#include "cif/cifparser.h"
#include "mscinstanceevent.h"

namespace msc {
namespace cmd {

EventMoveBaseCommand::EventMoveBaseCommand(
        MscInstanceEvent *event, ChartLayoutManager *layoutManager, QUndoCommand *parent)
    : ChartBaseCommand(event, layoutManager, parent)
    , m_event(event)
{
}

void EventMoveBaseCommand::storeGeometries()
{
    if (m_layoutManager.isNull()) {
        return;
    }

    m_eventGeometries.clear();
    for (InteractiveObject *item : m_layoutManager->instanceEventItems()) {
        MscEntity *entity = item->modelEntity();
        m_eventGeometries.insert(entity, entity->cifText());
    }
}

void EventMoveBaseCommand::restoreGeometries()
{
    for (auto it = m_eventGeometries.begin(); it != m_eventGeometries.end(); ++it) {
        restoreCif(it.key(), it.value());
    }
}

void EventMoveBaseCommand::applyNewPos()
{
    if (m_newCif.isEmpty()) {
        InteractiveObject *item = m_layoutManager->itemForEntity(m_event);
        item->updateCif();
        m_newCif = m_event->cifText();
    } else {
        restoreCif(m_event, m_newCif);
    }
}

void EventMoveBaseCommand::restoreCif(MscEntity *entity, const QString &cifText) const
{
    QStringList lines = cifText.split("\n");
    for (QString &line : lines) {
        if (line.startsWith("/*")) {
            line.remove(0, 2);
        }
        if (line.endsWith("*/")) {
            line.remove(line.length() - 2, 2);
        }
        line = line.trimmed();
    }
    cif::CifParser parser;
    QVector<cif::CifBlockShared> cifs = parser.readCifBlocks(lines);
    entity->setCifs(cifs);
}

} // namespace cmd
} // namespace msc
