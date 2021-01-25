/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadliface.h"
#include "cmdentitygeometrychange.h"

#include <QPointer>
#include <QVector>

namespace ive {
namespace cmd {

class CmdInterfaceItemCreate : public CmdEntityGeometryChange
{
public:
    explicit CmdInterfaceItemCreate(const ivm::AADLIface::CreationInfo &creationInfo);
    ~CmdInterfaceItemCreate() override;

    void redo() override;
    void undo() override;
    int id() const override;

    ivm::AADLIface *createdInterface() const;

private:
    const ivm::AADLIface::CreationInfo m_ifaceInfo;
    QPointer<ivm::AADLIface> m_entity;
    QVector<QUndoCommand *> m_cmdClones;
};

}
}
