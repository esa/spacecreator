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

#include "cmdsetmessagedeclarations.h"

#include "mscdocument.h"
#include "mscmessagedeclarationlist.h"

namespace msc {
namespace cmd {

CmdSetMessageDeclarations::CmdSetMessageDeclarations(msc::MscDocument *document,
                                                     msc::MscMessageDeclarationList *declarations)
    : BaseCommand(document)
    , m_document(document)
    , m_newDeclarations(declarations->clone())
    , m_oldDeclarations(new MscMessageDeclarationList(this))
{
    Q_ASSERT(m_document);
    setText(QObject::tr("Set message declarations"));

    m_newDeclarations->setParent(this);
}

void CmdSetMessageDeclarations::redo()
{
    m_oldDeclarations->setObjectList(m_document->messageDeclarations()->objectList());
    m_document->messageDeclarations()->clear(false);
    m_document->messageDeclarations()->setObjectList(m_newDeclarations->objectList());
    m_newDeclarations->clear(false);
}

void CmdSetMessageDeclarations::undo()
{
    m_newDeclarations->setObjectList(m_document->messageDeclarations()->objectList());
    m_document->messageDeclarations()->clear(false);
    m_document->messageDeclarations()->setObjectList(m_oldDeclarations->objectList());
    m_oldDeclarations->clear(false);
}

bool CmdSetMessageDeclarations::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdSetMessageDeclarations::id() const
{
    return msc::cmd::Id::SetMessageDeclarations;
}

} // namespace cmd
} // namespace msc
