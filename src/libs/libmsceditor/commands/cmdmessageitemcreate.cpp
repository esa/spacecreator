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
#include "commandids.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmessage.h"

namespace msc {
namespace cmd {

CmdMessageItemCreate::InstanceGeometry CmdMessageItemCreate::initGeometryHolder(msc::MscInstance *from)
{
    InstanceGeometry geometry;
    if (from) {
        geometry.m_cif = from->cifGeometry();
    }

    return geometry;
}

CmdMessageItemCreate::CmdMessageItemCreate(
        msc::MscMessage *message, const ChartIndexList &instanceIndexes, MscChart *chart, const QVector<QPoint> &points)
    : ChartBaseCommand(message, chart)
    , m_message(message)
    , m_instanceIndexes(instanceIndexes)
    , m_msgPoints(points)
    , m_sourceGeometryPrev(initGeometryHolder(m_message ? m_message->sourceInstance() : nullptr))
    , m_targetGeometryPrev(initGeometryHolder(m_message ? m_message->targetInstance() : nullptr))
{
    Q_ASSERT(m_message);
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add message"));
}

void CmdMessageItemCreate::redo()
{
    Q_ASSERT(m_chart.data());

    if (m_message->messageType() == MscMessage::MessageType::Create && m_message->targetInstance())
        m_message->targetInstance()->setExplicitCreator(m_message->sourceInstance());

    if (!m_msgPoints.isEmpty())
        if (cif::CifBlockShared msgCif = cif::CifBlockFactory::createBlockMessage()) {
            msgCif->setLines({ cif::CifLineShared(new cif::CifLineMessage) });
            msgCif->setPayload(QVariant::fromValue(m_msgPoints), cif::CifLine::CifType::Message);
            m_modelItem->setCifs({ msgCif });
        }

    // The chart takes over parent-/owner-ship
    m_chart->addInstanceEvent(m_message, m_instanceIndexes);
}

void CmdMessageItemCreate::undo()
{
    Q_ASSERT(m_chart.data());

    auto restoreInstanceGeometry = [this](MscInstance *instance, const InstanceGeometry &geometry) {
        if (!instance) {
            return;
        }

        QSignalBlocker silently(instance);
        if (!geometry.m_cif.isEmpty()) {
            instance->setCifGeometry(geometry.m_cif);
        }
    };

    restoreInstanceGeometry(m_message->sourceInstance(), m_sourceGeometryPrev);
    restoreInstanceGeometry(m_message->targetInstance(), m_targetGeometryPrev);

    m_chart->removeInstanceEvent(m_message);

    // this command takes over ownership
    m_message->setParent(this);
    if (m_message->messageType() == MscMessage::MessageType::Create && m_message->targetInstance()) {
        m_message->targetInstance()->setExplicitCreator(nullptr);
    }
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
