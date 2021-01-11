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

#include "commandsstackbase.h"

#include "undocommand.h"

#include <QUndoStack>

namespace shared {
namespace cmd {

/*!
 * \class CommandsStack
 * \brief A wrapper around the QUndoStack, provides the convenient access to the current stack.
 * During integration of other types of documents may be replaced by QUndoGroup. Or may not.
 */

/*!
 * \brief CommandsStack::Macro::Macro
 * The helper structure to remove "macros" from the undo stack. Used in cases when a macro canceled during its
 composition.
 * \sa CreatorTool::handleConnection(), PropertiesDialog

 */
CommandsStackBase::Macro::Macro(const QString &title)
{
    CommandsStackBase::current()->beginMacro(title);
}

/*!
 * \brief CommandsStack::Macro::~Macro
 * The actual magic appears here. Uses the const_cast and particular calls order, so the magic is black.
 * To avoid this, the following refactoring is necessary:
 *  all the existing custom undo commands and methods of CommandsFactory have to receive the QUndoCommand* as a
 * parameter, so the macro would be managed as: \code
 * ...
 * QUndoCommand* root = new QUndoCommand();
 * CommandsFactory::createSubcommand(..., root); // creates the QUndoCommand as a child of root
 * CommandsFactory::createSubcommand(..., root);
 * CommandsFactory::createSubcommand(..., root);
 *
 * if(somethingGoesWrong)
 *     ... // delete all commands, including the root
 * else
 *  commandsStack->push(root);
 * \endcode
 */
CommandsStackBase::Macro::~Macro()
{
    CommandsStackBase::current()->endMacro();

    int count = 0;
    if (const auto cmd = CommandsStackBase::current()->command(CommandsStackBase::current()->index() - 1))
        count = cmd->childCount();

    if (!isComplete() || 0 == count) {

        // I found no other way to remove a macro from the stack:
        const int posOfMacro = CommandsStackBase::current()->index() - 1;
        if (auto macroCmd = const_cast<QUndoCommand *>(CommandsStackBase::current()->command(posOfMacro))) {
            macroCmd->undo(); // unperform all the stuff
            macroCmd->setObsolete(true); // to be checked in QUndoStack::undo
        }
        CommandsStackBase::current()->undo(); // just removes the history record
    }
}

/*!
 * \brief CommandsStack::Macro::push
 * Checks the \a cmd for null and places it into the current stack.
 * Returns \c false if the \a cmd is null.
 */
bool CommandsStackBase::Macro::push(QUndoCommand *cmd) const
{
    return CommandsStackBase::push(cmd);
}

/*!
 * \brief CommandsStack::Macro::setComplete
 * Pass \c true in \a complete argument to keep the created macro after destroying this instance.
 * Passing \c false (the default value when created) result in deleteion of the macro in d-tor.
 */
void CommandsStackBase::Macro::setComplete(bool complete)
{
    m_keepMacro = complete;
}

/*!
 * \brief CommandsStack::Macro::isComplete
 * If the macro will be deleted in d-tor, returns \c false, otherwise returns \c true.
 */
bool CommandsStackBase::Macro::isComplete() const
{
    return m_keepMacro;
}

CommandsStackBase *CommandsStackBase::m_instance = nullptr;

/*!
 * \brief CommandsStack::instance
 * Common accessor to the global instance.
 */
CommandsStackBase *CommandsStackBase::instance()
{
    if (!m_instance)
        m_instance = new CommandsStackBase();

    return m_instance;
}

/*!
 * \brief CommandsStack::setCurrent
 * Registers the \a stack as a current one.
 */
void CommandsStackBase::setCurrent(QUndoStack *stack)
{
    instance()->setCurrentStack(stack);
}

/*!
 * \brief CommandsStack::current
 * Returns pointer to the current (last registered) undo stack. May be null.
 */
QUndoStack *CommandsStackBase::current()
{
    return instance()->currentStack();
}

/*!
 * \brief CommandsStack::push
 * Pushes the \a command into the current stack, if both are not null.
 * Returns \c true if the \a command pushed into the stack.
 */
bool CommandsStackBase::push(QUndoCommand *command)
{
    if (command && CommandsStackBase::current()) {
        CommandsStackBase::current()->push(command);
        return true;
    }

    return false;
}

/*!
 * \brief CommandsStack::CommandsStack
 * The constructor is private to force usage of static accessors.
 */
CommandsStackBase::CommandsStackBase(QObject *parent)
    : QObject(parent)
{
}

/*!
 * \brief CommandsStack::setCurrentStack
 * If \a stack differs from the current, set it as current end emit the notification.
 * The nullptr is allowed for \a stack here.
 */
void CommandsStackBase::setCurrentStack(QUndoStack *stack)
{
    if (m_current == stack)
        return;

    m_current = stack;
    Q_EMIT currentStackChanged(m_current);
}

/*!
 * \brief CommandsStack::currentStack
 * Returns pointer to the current (the last registered) undo stack.
 */
QUndoStack *CommandsStackBase::currentStack() const
{
    return m_current;
}

}
}
