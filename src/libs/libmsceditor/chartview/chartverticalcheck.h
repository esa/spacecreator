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

#pragma once

#include <QList>
#include <QMap>
#include <QPointer>
#include <QSet>

namespace msc {
class ChartLayoutManager;
class CoregionItem;
class EventItem;
class MessageItem;
class MscChart;
class MscInstance;
class MscInstanceEvent;

/*!
   Class to check and correct the vertical position of events
 */
class ChartVerticalCheck
{
public:
    ChartVerticalCheck();

    void reset(msc::ChartLayoutManager *manager, msc::MscChart *chart);

    void checkVerticalConstraints();
    void checkEvent(msc::MscInstanceEvent *event);

private:
    void checkMessageHead(MessageItem *messageItem, bool isCrossing);
    void checkMessageTail(MessageItem *messageItem, bool isCrossing);
    void checkPrecedingEvents(msc::MscInstanceEvent *event, MscInstance *instance);

    const qreal m_minSpace = 3.;
    const int m_messageOffset = 8;

    QPointer<msc::ChartLayoutManager> m_manager;
    QPointer<msc::MscChart> m_currentChart;
    QMap<MscInstance *, qreal> minYperInstance;
    QList<msc::EventItem *> m_overlapEvents;
    QSet<msc::MscInstanceEvent *> m_processedEvents;
    CoregionItem *m_activeCoregionItem = nullptr;
};

}
