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
CommandsStackBase::Macro::Macro(QUndoStack *undoStack, const QString &title)
    : m_undoStack(undoStack)
{
    m_undoStack->beginMacro(title);
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
    m_undoStack->endMacro();

    int count = 0;
    if (const auto cmd = m_undoStack->command(m_undoStack->index() - 1)) {
        count = cmd->childCount();
    }

    if (!isComplete() || 0 == count) {

        // I found no other way to remove a macro from the stack:
        const int posOfMacro = m_undoStack->index() - 1;
        if (auto macroCmd = const_cast<QUndoCommand *>(m_undoStack->command(posOfMacro))) {
            macroCmd->undo(); // unperform all the stuff
            macroCmd->setObsolete(true); // to be checked in QUndoStack::undo
        }
        m_undoStack->undo(); // just removes the history record
    }
}

/*!
 * \brief CommandsStack::Macro::push
 * Checks the \a cmd for null and places it into the current stack.
 * Returns \c false if the \a cmd is null.
 */
bool CommandsStackBase::Macro::push(QUndoCommand *cmd) const
{
    m_undoStack->push(cmd);
    return true;
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

/*!
 * \brief CommandsStack::CommandsStack
 * The constructor is private to force usage of static accessors.
 */
CommandsStackBase::CommandsStackBase(QObject *parent)
    : QObject(parent)
    , m_undoStack(new QUndoStack)
{
    connect(m_undoStack.get(), &QUndoStack::cleanChanged, this, &CommandsStackBase::cleanChanged);
    connect(m_undoStack.get(), &QUndoStack::canUndoChanged, this, &CommandsStackBase::canUndoChanged);
    connect(m_undoStack.get(), &QUndoStack::canRedoChanged, this, &CommandsStackBase::canRedoChanged);
    connect(m_undoStack.get(), &QUndoStack::undoTextChanged, this, &CommandsStackBase::undoTextChanged);
    connect(m_undoStack.get(), &QUndoStack::redoTextChanged, this, &CommandsStackBase::redoTextChanged);
    connect(m_undoStack.get(), &QUndoStack::indexChanged, this, &CommandsStackBase::indexChanged);
}

/*!
   \brief CommandsStackBase::~CommandsStackBase
 */
CommandsStackBase::~CommandsStackBase()
{
    m_undoStack->clear();
}

/*!
 * \brief CommandsStack::push
 * Pushes the \a command into the current stack, if both are not null.
 * Returns \c true if the \a command pushed into the stack.
 */
bool CommandsStackBase::push(QUndoCommand *command)
{
    m_undoStack->push(command);
    return true;
}

QUndoStack *CommandsStackBase::undoStack()
{
    return m_undoStack.get();
}

}
}
