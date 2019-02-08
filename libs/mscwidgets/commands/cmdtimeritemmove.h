/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef CMDTIMERITEMMOVE_H
#define CMDTIMERITEMMOVE_H

#include "basecommand.h"

#include <QPointer>

namespace msc {

class MscTimer;
class MscChart;
class MscInstance;

namespace cmd {

class CmdTimerItemMove : public BaseCommand
{
public:
    CmdTimerItemMove(msc::MscTimer *timer, int newPos, msc::MscInstance *newInsance, MscChart *chart);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<msc::MscTimer> m_timer;
    int m_oldIndex = -1;
    int m_newIndex = -1;
    QPointer<msc::MscInstance> m_oldInstance;
    QPointer<msc::MscInstance> m_newInstance;
    QPointer<msc::MscChart> m_chart;
};

} // namespace cmd
} // namespace msc

#endif // CMDTIMERITEMMOVE_H
