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

#include "commands/basecommand.h"

#include <QPointer>

namespace msc {

class MscCoregion;
class MscChart;
class MscInstance;

namespace cmd {

class CmdCoregionItemCreate : public BaseCommand
{
    Q_OBJECT
public:
    explicit CmdCoregionItemCreate(msc::MscCoregion *begin, msc::MscCoregion *end, msc::MscChart *chart,
                                   msc::MscInstance *instance, int eventIndex);
    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    MscCoregion *m_begin = nullptr;
    MscCoregion *m_end = nullptr;
    QPointer<MscInstance> m_instance;
    QPointer<MscChart> m_chart;
    int m_eventIndex = -1;
};

} // namespace msc
} // namespace cmd
