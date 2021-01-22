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

#pragma once

#include "commandsstackbase.h"

#include <QObject>

class QUndoCommand;
class QUndoStack;

namespace aadl {
class AADLObject;
}

namespace shared {
class UndoCommand;
}

namespace ive {
namespace cmd {

class CommandsStack : public shared::cmd::CommandsStackBase
{
    Q_OBJECT
public:
    static bool push(QUndoCommand *command);

Q_SIGNALS:
    void nameChanged(aadl::AADLObject *entity, const QString &oldName, shared::UndoCommand *command);
    void entityRemoved(aadl::AADLObject *entity, shared::UndoCommand *command);
};

}
}
