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

#include "chartindex.h"
#include "eventmovebasecommand.h"

#include <QHash>
#include <QPoint>
#include <QPointer>
#include <QVector>

namespace msc {

class MscChart;
class MscInstance;
class MscMessage;

namespace cmd {

class CmdMessagePointsEdit : public EventMoveBaseCommand
{
public:
    CmdMessagePointsEdit(MscMessage *message, const QVector<QPoint> &cifPointsOld, const QVector<QPoint> &cifPointsNew,
            ChartIndexList indices, ChartLayoutManager *layoutManager);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<MscMessage> m_message;
    QVector<QPoint> m_newCif;
    const QVector<QPoint> m_oldCif;
    const ChartIndexList m_newIndexes;
    const ChartIndexList m_oldIndexes;
};

} // ns cmd
} // ns msc
