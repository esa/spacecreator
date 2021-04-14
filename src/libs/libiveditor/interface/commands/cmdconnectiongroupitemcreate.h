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

#pragma once

#include "ivconnectiongroup.h"
#include "cmdentitygeometrychange.h"

namespace ive {
namespace cmd {

class CmdConnectionGroupItemCreate : public CmdEntityGeometryChange
{
public:
    explicit CmdConnectionGroupItemCreate(const ivm::IVConnectionGroup::CreationInfo &creationInfo);
    ~CmdConnectionGroupItemCreate() override;
    void redo() override;
    void undo() override;
    int id() const override;

private:
    QString m_groupName;
    QPointer<ivm::IVModel> m_model;
    QPointer<ivm::IVConnectionGroup> m_entity;
    QPointer<ivm::IVFunction> m_parent;
    QPointer<ivm::IVInterfaceGroup> m_sourceIface;
    QPointer<ivm::IVFunction> m_sourceIfaceParent;
    QPointer<ivm::IVInterfaceGroup> m_targetIface;
    QPointer<ivm::IVFunction> m_targetIfaceParent;
    QList<QUndoCommand *> m_subCommands;
};

} // namespace ive
} // namespace cmd
