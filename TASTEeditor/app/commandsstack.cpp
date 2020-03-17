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

#include "commandsstack.h"

namespace taste3 {
namespace cmd {

CommandsStack::Macro::Macro(const QString &title)
{
    taste3::cmd::CommandsStack::current()->beginMacro(title);
}

CommandsStack::Macro::~Macro()
{
    taste3::cmd::CommandsStack::current()->endMacro();

    if (!isComplete()) {

        // I found no other way to remove a macro from the stack:
        const int posOfMacro = taste3::cmd::CommandsStack::current()->index() - 1;
        if (auto macroCmd = const_cast<QUndoCommand *>(taste3::cmd::CommandsStack::current()->command(posOfMacro))) {
            macroCmd->undo(); // unperform all the stuff
            macroCmd->setObsolete(true); // to be checked in QUndoStack::undo
        }
        taste3::cmd::CommandsStack::current()->undo(); // just removes the history record
    }
}

bool CommandsStack::Macro::push(QUndoCommand *cmd) const
{
    if (!cmd)
        return false;
    taste3::cmd::CommandsStack::current()->push(cmd);
    return true;
}

void CommandsStack::Macro::setComplete(bool complete)
{
    m_keepMacro = complete;
}

bool CommandsStack::Macro::isComplete() const
{
    return m_keepMacro;
}

CommandsStack *CommandsStack::m_instance = nullptr;

CommandsStack *CommandsStack::instance()
{
    if (!m_instance)
        m_instance = new CommandsStack();

    return m_instance;
}

void CommandsStack::setCurrent(QUndoStack *stack)
{
    instance()->setCurrentStack(stack);
}

QUndoStack *CommandsStack::current()
{
    return instance()->currentStack();
}

bool CommandsStack::push(QUndoCommand *command)
{
    if (command && CommandsStack::current()) {
        CommandsStack::current()->push(command);
        return true;
    }

    return false;
}

CommandsStack::CommandsStack(QObject *parent)
    : QObject(parent)
{
}

void CommandsStack::setCurrentStack(QUndoStack *stack)
{
    if (m_current == stack)
        return;

    m_current = stack;
    Q_EMIT currentStackChanged(m_current);
}

QUndoStack *CommandsStack::currentStack() const
{
    return m_current;
}

} // namespace cmd
} // ns taste3
