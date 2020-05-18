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

#include "cmdcommentitemchangegeometry.h"

#include "cif/cifblockfactory.h"
#include "cif/cifline.h"
#include "cif/ciflines.h"
#include "common/commandids.h"
#include "mscchart.h"
#include "msccomment.h"

namespace msc {
namespace cmd {

CmdCommentItemChangeGeometry::CmdCommentItemChangeGeometry(
        MscChart *chart, const QRect &oldRect, const QRect &newRect, MscEntity *entity)
    : BaseCommand(entity)
    , m_chart(chart)
    , m_oldRect(oldRect)
    , m_newRect(newRect)
{
}

void CmdCommentItemChangeGeometry::redo()
{
    if (!m_modelItem)
        return;

    auto comment = m_modelItem->comment();
    if (!comment) {
        comment = m_modelItem->setCommentString(QString());
        m_chart->addInstanceEvent(comment);
    }
    comment->setRect(m_newRect);
}

void CmdCommentItemChangeGeometry::undo()
{
    if (!m_modelItem)
        return;

    auto comment = m_modelItem->comment();
    if (!comment) {
        comment = m_modelItem->setCommentString(QString());
        m_chart->addInstanceEvent(comment);
    }
    comment->setRect(m_oldRect);
}

bool CmdCommentItemChangeGeometry::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    m_newRect = static_cast<const CmdCommentItemChangeGeometry *>(command)->m_newRect;
    return true;
}

int CmdCommentItemChangeGeometry::id() const
{
    return msc::cmd::Id::ChangeCommentGeometry;
}

} // namespace cmd
} // namespace msc
