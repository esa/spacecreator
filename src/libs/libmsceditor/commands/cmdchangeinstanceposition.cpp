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

#include "cmdchangeinstanceposition.h"

#include "cif/cifblockfactory.h"
#include "cif/cifparser.h"
#include "commandids.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdChangeInstancePosition::CmdChangeInstancePosition(MscInstance *instance, const QVector<QPoint> &newCif)
    : BaseCommand(instance)
    , m_instance(instance)
    , m_oldCif(instance->cifGeometry())
    , m_newCif(newCif)
{
}

void CmdChangeInstancePosition::redo()
{
    if (m_instance)
        m_instance->setCifGeometry(m_newCif);
}

void CmdChangeInstancePosition::undo()
{
    if (m_instance)
        m_instance->setCifGeometry(m_oldCif);
}

bool CmdChangeInstancePosition::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}
int CmdChangeInstancePosition::id() const
{
    return msc::cmd::Id::ChangeInstancePosition;
}

} // ns cmd
} // ns msc
