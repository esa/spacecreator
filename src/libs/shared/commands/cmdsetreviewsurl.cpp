/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdsetreviewsurl.h"

#include "commandids.h"

namespace shared {
namespace cmd {

/*!
 * \param model The model to store the URL information in
 * \param url The new URL for the reviews (gitlab project)
 */
CmdSetReviewsUrl::CmdSetReviewsUrl(shared::DataModel *model, const QUrl &url, QObject *parent)
    : shared::UndoCommand(parent)
    , m_model(model)
    , m_oldUrl(m_model->reviewsURL())
    , m_newUrl(url)
{
}

void CmdSetReviewsUrl::redo()
{
    m_model->setReviewsURL(m_newUrl);
}

void CmdSetReviewsUrl::undo()
{
    m_model->setReviewsURL(m_oldUrl);
}

bool CmdSetReviewsUrl::mergeWith(const QUndoCommand *command)
{
    if (id() == command->id()) {
        auto other = static_cast<const CmdSetReviewsUrl *>(command);
        m_oldUrl = other->m_oldUrl;
        return true;
    }

    return false;
}

int CmdSetReviewsUrl::id() const
{
    return Id::SetReviewsUrl;
}

} // namespace cmd
} // namespace shared
