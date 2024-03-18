/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdupdateentityreviews.h"

#include "commandids.h"
#include "mscentity.h"

namespace msc {
namespace cmd {

/*!
 * \param item The entity the reviews are linked to
 * \param review A list of IDs of the reviews to be linked to the entity
 */
CmdUpdateEntityReviews::CmdUpdateEntityReviews(MscEntity *item, const QStringList &reviews)
    : msc::cmd::BaseCommand(item)
    , m_newReviews(reviews)
    , m_oldReviews(item->reviews())
{
}

void CmdUpdateEntityReviews::redo()
{
    m_modelItem->setReviews(m_newReviews);
}

void CmdUpdateEntityReviews::undo()
{
    m_modelItem->setReviews(m_oldReviews);
}

bool CmdUpdateEntityReviews::mergeWith(const QUndoCommand *command)
{
    if (id() == command->id()) {
        auto other = static_cast<const CmdUpdateEntityReviews *>(command);
        m_oldReviews = other->m_oldReviews;
        return true;
    }

    return false;
}

int CmdUpdateEntityReviews::id() const
{
    return Id::UpdateEntityReviews;
}

} // namespace cmd
} // namespace msc
