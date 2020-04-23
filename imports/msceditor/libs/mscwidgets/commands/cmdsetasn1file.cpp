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

#include "cmdsetasn1file.h"

#include "commands/common/commandids.h"
#include "mscmodel.h"

namespace msc {
namespace cmd {

CmdSetAsn1File::CmdSetAsn1File(MscModel *model, const QString &filename, const QString &language)
    : QUndoCommand()
    , m_model(model)
    , m_newFilename(filename)
    , m_oldFilename(model->dataDefinitionString())
    , m_newLanguage(language)
    , m_oldLanguage(model->dataLanguage())
{
}

void CmdSetAsn1File::redo()
{
    if (m_model) {
        m_model->setDataDefinitionString(m_newFilename);
        m_model->setDataLanguage(m_newLanguage);
    }
}

void CmdSetAsn1File::undo()
{
    if (m_model) {
        m_model->setDataDefinitionString(m_oldFilename);
        m_model->setDataLanguage(m_oldLanguage);
    }
}

bool CmdSetAsn1File::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdSetAsn1File *>(command);
    if (other && other->id() == id()) {
        m_newFilename = other->m_newFilename;
        m_newLanguage = other->m_newLanguage;
        return true;
    }

    return false;
}

int CmdSetAsn1File::id() const
{
    return msc::cmd::Id::SetAsn1File;
}

} // namespace cmd
} // namespace msc
