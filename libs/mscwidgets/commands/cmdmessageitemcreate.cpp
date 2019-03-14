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

#include "cmdmessageitemcreate.h"

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmessage.h"

namespace msc {
namespace cmd {

CmdMessageItemCreate::CmdMessageItemCreate(msc::MscMessage *message, msc::MscChart *chart, int eventIndex,
                                           const QVector<QPointF> &points)
    : BaseCommand(message)
    , m_message(message)
    , m_chart(chart)
    , m_eventIndex(eventIndex)
    , m_msgPoints(points)
{
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add message"));
}

void CmdMessageItemCreate::redo()
{
    Q_ASSERT(m_chart.data());

    if (!m_message) {
        m_message = new MscMessage(QObject::tr("Message_%1").arg(m_chart->instanceEvents().size()));
        m_modelItem = m_message;
    }
    if (m_message->messageType() == MscMessage::MessageType::Create && m_message->targetInstance())
        m_message->targetInstance()->setExplicitCreator(m_message->sourceInstance());

    if (!m_msgPoints.isEmpty())
        if (cif::CifBlockShared msgCif = cif::CifBlockFactory::createBlockMessage()) {
            QVector<cif::CifLineShared> lines = { cif::CifLineShared(new cif::CifLineMessage) };
            msgCif->setLines(lines);
            msgCif->setPayload(QVariant::fromValue(m_msgPoints), cif::CifLine::CifType::Message);
            m_modelItem->setCifs({ msgCif });
        }

    // The chart takes over parent-/owner-ship
    m_chart->addInstanceEvent(m_message, m_eventIndex);
}

void CmdMessageItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_message);

    // this command takes over ownership
    m_message->setParent(this);
    if (m_message->messageType() == MscMessage::MessageType::Create && m_message->targetInstance())
        m_message->targetInstance()->setExplicitCreator(nullptr);
}

bool CmdMessageItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdMessageItemCreate::id() const
{
    return msc::cmd::Id::CreateMessage;
}

} // ns cmd
} // ns msc
