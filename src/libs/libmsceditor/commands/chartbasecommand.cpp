/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "chartbasecommand.h"

#include "chartlayoutmanager.h"
#include "mscchart.h"
#include "mscinstanceevent.h"

namespace msc {
namespace cmd {

ChartBaseCommand::ChartBaseCommand(
        MscEntity *item, MscChart *chart, ChartLayoutManager *layoutManager, QUndoCommand *parent)
    : BaseCommand(item, parent)
    , m_chart(chart)
    , m_layoutManager(layoutManager)
    , m_oldSortedEvents(m_chart ? m_chart->instanceEvents() : QVector<msc::MscInstanceEvent *>())
{
}

/*!
   Checks if the visual sorting/order of events got changed (might havven as messages are fixed to theis cif geometry)
 */
void ChartBaseCommand::checkVisualSorting()
{
    if (m_newSortedEvents.isEmpty() && m_layoutManager) {
        m_layoutManager->doLayout();
        m_newSortedEvents = m_layoutManager->visuallySortedEvents();
    }
    if (m_chart && (m_newSortedEvents != m_oldSortedEvents)) {
        m_chart->rearrangeEvents(m_newSortedEvents);
    }
}

/*!
   Undos any change done in \sa checkVisualSorting()
 */
void ChartBaseCommand::undoVisualSorting()
{
    if (m_chart && (m_newSortedEvents != m_oldSortedEvents)) {
        m_chart->rearrangeEvents(m_oldSortedEvents);
    }
}

} // namespace cmd
} // namespace msc
