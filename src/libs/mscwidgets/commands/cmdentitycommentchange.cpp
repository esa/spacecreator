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

#include "cmdentitycommentchange.h"

#include "mscchart.h"
#include "msccomment.h"

namespace msc {
namespace cmd {

CmdEntityCommentChange::CmdEntityCommentChange(MscChart *chart, MscEntity *item, const QString &newComment)
    : BaseCommand(item)
    , m_chart(chart)
    , m_oldComment(item && item->comment() ? item->comment()->text() : QString())
    , m_newComment(newComment)
{
}

void CmdEntityCommentChange::redo()
{
    if (!m_modelItem)
        return;

    MscComment *comment = m_modelItem->comment();
    if (m_newComment.isEmpty()) {
        if (comment) {
            comment->setCommentString(m_newComment);
            m_chart->removeInstanceEvent(comment);

            // this command takes over ownership
            comment->setParent(this);
        }
    } else {
        if (!comment) {
            comment = m_modelItem->setCommentString(m_newComment);
        }
        m_chart->addInstanceEvent(comment);
        comment->setText(m_newComment);
    }
}

void CmdEntityCommentChange::undo()
{
    if (!m_modelItem)
        return;

    MscComment *comment = m_modelItem->comment();
    if (m_oldComment.isEmpty()) {
        if (comment) {
            comment->setText(m_oldComment);
            m_chart->removeInstanceEvent(comment);
            comment->attachTo(nullptr);

            // this command takes over ownership
            comment->setParent(this);
        }
    } else {
        if (!comment) {
            comment = m_modelItem->setCommentString(m_oldComment);
        }
        m_chart->addInstanceEvent(comment);
        comment->setText(m_oldComment);
    }
}

bool CmdEntityCommentChange::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdEntityCommentChange *>(command);
    if (canMergeWith(other)) {
        m_newComment = other->m_newComment;
        return true;
    }

    return false;
}

int CmdEntityCommentChange::id() const
{
    return msc::cmd::Id::ChangeComment;
}

} // namespace cmd
} // namespace msc
