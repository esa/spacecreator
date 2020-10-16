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

#include "chartbasecommand.h"
#include "mscmessage.h"

#include <QPointer>

namespace msc {

class MscChart;
class MscInstance;

namespace cmd {

class CmdMessageItemResize : public ChartBaseCommand
{
public:
    CmdMessageItemResize(MscMessage *message, int newPos, msc::MscInstance *newInsance,
            msc::MscMessage::EndType endType, ChartLayoutManager *layoutManager);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<MscMessage> m_message;
    int m_oldIndex = -1;
    int m_newIndex = -1;
    QPointer<msc::MscInstance> m_oldInstance;
    QPointer<msc::MscInstance> m_newInstance;
    msc::MscMessage::EndType m_endType = msc::MscMessage::EndType::SOURCE_TAIL;
};

} // ns cmd
} // ns msc
