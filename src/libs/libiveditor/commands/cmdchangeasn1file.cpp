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

#include "cmdchangeasn1file.h"

#include "commandids.h"
#include "interfacedocument.h"

#include <QDir>

namespace ive {
namespace cmd {

CmdChangeAsn1File::CmdChangeAsn1File(
        InterfaceDocument *document, const QString &oldFileName, const QString &newFileName)
    : QUndoCommand(QObject::tr("Set ASN1 file"))
    , m_oldFileName(oldFileName)
    , m_newFileName(newFileName)
    , m_document(document)
{
    Q_ASSERT(document);
}

void CmdChangeAsn1File::redo()
{
    if (m_document)
        m_document->setAsn1FileName(m_newFileName, m_oldFileName);
}

void CmdChangeAsn1File::undo()
{
    if (m_document)
        m_document->setAsn1FileName(m_oldFileName, m_newFileName);
}

int CmdChangeAsn1File::id() const
{
    return ChangeAsn1File;
}

} // namespace cmd
} // namespace ive
