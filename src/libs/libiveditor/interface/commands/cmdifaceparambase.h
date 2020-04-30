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

#include "aadlobjectiface.h"

#include <QPointer>
#include <QUndoCommand>

namespace aadl {
class AADLObjectIface;
class AADLObjectConnection;
}

namespace aadlinterface {
namespace cmd {

class CmdIfaceParamBase : public QUndoCommand
{
public:
    ~CmdIfaceParamBase() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;

protected:
    // NOT to be instantiated directly!
    explicit CmdIfaceParamBase(aadl::AADLObjectIface *iface);
    int id() const override = 0;

protected:
    QPointer<aadl::AADLObjectIface> m_iface;
    QVector<aadl::AADLObjectConnection *> m_connections;
    QVector<QUndoCommand *> m_cmdRmConnections;

private:
    bool m_rmCommandsPrepared { false };
    void prepareRmCommands();
};

}
}
