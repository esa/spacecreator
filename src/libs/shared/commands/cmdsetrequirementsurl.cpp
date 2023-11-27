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

#include "cmdsetrequirementsurl.h"

#include "commandids.h"

namespace shared {
namespace cmd {

/*!
 * \param mscCore The model to store the URL information in
 * \param url The new URL for the requirements (gitlab project)
 */
CmdSetRequirementsUrl::CmdSetRequirementsUrl(shared::DataModel *model, const QUrl &url, QObject *parent)
    : shared::UndoCommand(parent)
    , m_model(model)
    , m_oldUrl(m_model->requirementsURL())
    , m_newUrl(url)
{
}

void CmdSetRequirementsUrl::redo()
{
    m_model->setRequirementsURL(m_newUrl);
}

void CmdSetRequirementsUrl::undo()
{
    m_model->setRequirementsURL(m_oldUrl);
}

bool CmdSetRequirementsUrl::mergeWith(const QUndoCommand *command)
{
    if (id() == command->id()) {
        auto other = static_cast<const CmdSetRequirementsUrl *>(command);
        m_oldUrl = other->m_oldUrl;
        return true;
    }

    return false;
}

int CmdSetRequirementsUrl::id() const
{
    return Id::SetRequirementsUrl;
}

} // namespace cmd
} // namespace shared
