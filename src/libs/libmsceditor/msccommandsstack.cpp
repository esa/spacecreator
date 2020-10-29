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

#include "msccommandsstack.h"

#include "commands/cmdentitynamechange.h"

#include <QUndoStack>

namespace msc {

MscCommandsStack::MscCommandsStack(QObject *parent)
    : QObject(parent)
    , m_undoStack(new QUndoStack)
{
    connect(m_undoStack.get(), &QUndoStack::cleanChanged, this, &msc::MscCommandsStack::cleanChanged);
    connect(m_undoStack.get(), &QUndoStack::canUndoChanged, this, &msc::MscCommandsStack::canUndoChanged);
    connect(m_undoStack.get(), &QUndoStack::canRedoChanged, this, &msc::MscCommandsStack::canRedoChanged);
    connect(m_undoStack.get(), &QUndoStack::undoTextChanged, this, &msc::MscCommandsStack::undoTextChanged);
    connect(m_undoStack.get(), &QUndoStack::redoTextChanged, this, &msc::MscCommandsStack::redoTextChanged);
    connect(m_undoStack.get(), &QUndoStack::indexChanged, this, &msc::MscCommandsStack::indexChanged);
}

MscCommandsStack::~MscCommandsStack() { }

void MscCommandsStack::clear()
{
    m_undoStack->clear();
}

void MscCommandsStack::push(QUndoCommand *command)
{
    // check connection for name connection
    if (command) {
        if (auto nameCommand = dynamic_cast<msc::cmd::CmdEntityNameChange *>(command)) {
            connect(nameCommand, &msc::cmd::CmdEntityNameChange::nameChanged, this,
                    &msc::MscCommandsStack::nameChanged);
        }
    }
    m_undoStack->push(command);
}

bool MscCommandsStack::canUndo() const
{
    return m_undoStack->canUndo();
}

bool MscCommandsStack::canRedo() const
{
    return m_undoStack->canRedo();
}

QString MscCommandsStack::undoText() const
{
    return m_undoStack->undoText();
}

QString MscCommandsStack::redoText() const
{
    return m_undoStack->redoText();
}

int MscCommandsStack::count() const
{
    return m_undoStack->count();
}

int MscCommandsStack::index() const
{
    return m_undoStack->index();
}

QString MscCommandsStack::text(int idx) const
{
    return m_undoStack->text(idx);
}

bool MscCommandsStack::isActive() const
{
    return m_undoStack->isActive();
}

bool MscCommandsStack::isClean() const
{
    return m_undoStack->isClean();
}

int MscCommandsStack::cleanIndex() const
{
    return m_undoStack->cleanIndex();
}

void MscCommandsStack::beginMacro(const QString &text)
{
    m_undoStack->beginMacro(text);
}

void MscCommandsStack::endMacro()
{
    m_undoStack->endMacro();
}

void MscCommandsStack::setUndoLimit(int limit)
{
    m_undoStack->setUndoLimit(limit);
}

int MscCommandsStack::undoLimit() const
{
    return m_undoStack->undoLimit();
}

const QUndoCommand *MscCommandsStack::command(int index) const
{
    return m_undoStack->command(index);
}

QUndoStack *MscCommandsStack::undoStack()
{
    return m_undoStack.get();
}

void MscCommandsStack::setClean()
{
    m_undoStack->setClean();
}

void MscCommandsStack::resetClean()
{
    m_undoStack->resetClean();
}

void MscCommandsStack::setIndex(int idx)
{
    m_undoStack->setIndex(idx);
}

void MscCommandsStack::undo()
{
    m_undoStack->undo();
}

void MscCommandsStack::redo()
{
    m_undoStack->redo();
}

void MscCommandsStack::setActive(bool active)
{
    m_undoStack->setActive(active);
}

} // namespace msc
