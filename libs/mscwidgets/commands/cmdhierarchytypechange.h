
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

#ifndef CMDHIERARCHYTYPECHANGE_H
#define CMDHIERARCHYTYPECHANGE_H

#include "basecommand.h"
#include "mscdocument.h"

namespace msc {

namespace cmd {

class CmdHierarchyTypeChange : public BaseCommand
{
public:
    CmdHierarchyTypeChange(MscDocument *document, const MscDocument::HierarchyType newType);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<MscDocument> m_document;
    MscDocument::HierarchyType m_oldType;
    MscDocument::HierarchyType m_newType;
};

} // namespace cmd
} // namespace msc

#endif // CMDHIERARCHYTYPECHANGE_H
