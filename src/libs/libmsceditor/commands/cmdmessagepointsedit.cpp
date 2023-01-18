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

#include "cmdmessagepointsedit.h"

#include "commandids.h"
#include "mscchart.h"
#include "mscmessage.h"

#include <QDebug>

namespace msc {
namespace cmd {

CmdMessagePointsEdit::CmdMessagePointsEdit(MscMessage *message, const QVector<QPoint> &cifPointsOld,
        const QVector<QPoint> &cifPointsNew, ChartIndexList indices, MscChart *chart)
    : EventMoveBaseCommand(message, chart)
    , m_message(message)
    , m_newCifData(cifPointsNew)
    , m_oldCifData(cifPointsOld)
    , m_newIndexes(indices)
    , m_oldIndexes(m_chart->indicesOfEvent(m_message))
{
    setText(QObject::tr("Edit message trajectory"));
}

void CmdMessagePointsEdit::redo()
{
    if (m_chart) {
        m_chart->moveEvent(m_message, m_newIndexes);
    }

    storeGeometries();
    if (m_message) {
        m_message->setCifPoints(m_newCifData);
    }
}

void CmdMessagePointsEdit::undo()
{
    if (m_chart) {
        m_chart->moveEvent(m_message, m_oldIndexes);
    }

    restoreGeometries();
    if (m_message) {
        m_message->setCifPoints(m_oldCifData);
    }
}

bool CmdMessagePointsEdit::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdMessagePointsEdit::id() const
{
    return msc::cmd::Id::EditMessagePoints;
}

} // ns cmd
} // ns msc
