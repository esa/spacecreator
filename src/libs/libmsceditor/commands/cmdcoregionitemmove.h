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

#pragma once

#include "basecommand.h"

#include <QPointer>

namespace msc {

class MscCoregion;
class MscChart;
class MscInstance;

namespace cmd {

class CmdCoRegionItemMove : public BaseCommand
{
    Q_OBJECT
public:
    CmdCoRegionItemMove(MscCoregion *coregionBegin, MscCoregion *coregionEnd, int newBeginPos, int newEndPos,
            MscInstance *newInsance, MscChart *chart);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<msc::MscCoregion> m_coregionBegin;
    QPointer<msc::MscCoregion> m_coregionEnd;
    int m_oldBeginIndex = -1;
    int m_oldEndIndex = -1;
    int m_newBeginIndex = -1;
    int m_newEndIndex = -1;
    QPointer<msc::MscInstance> m_oldInstance;
    QPointer<msc::MscInstance> m_newInstance;
    QPointer<msc::MscChart> m_chart;
};

}
}
