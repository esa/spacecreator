/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "undocommand.h"

#include <QObject>
#include <QUndoStack>
#include <memory>

class QUndoCommand;

namespace shared {
class UndoCommand;

namespace cmd {

class CommandsStackBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive)
    Q_PROPERTY(int undoLimit READ undoLimit WRITE setUndoLimit)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(QString undoText READ undoText NOTIFY undoTextChanged)
    Q_PROPERTY(QString redoText READ redoText NOTIFY redoTextChanged)
    Q_PROPERTY(bool clean READ isClean NOTIFY cleanChanged)

public:
    struct Macro {
        explicit Macro(CommandsStackBase *undoStack, const QString &title = QString());
        ~Macro();

        bool push(QUndoCommand *cmd) const;

        void setComplete(bool complete);
        bool isComplete() const;

    private:
        bool m_keepMacro { false };
        CommandsStackBase *m_undoStack = nullptr;
    };

    explicit CommandsStackBase(QObject *parent = nullptr);
    ~CommandsStackBase();

    virtual bool push(QUndoCommand *command);

    QUndoStack *undoStack();

    void clear() { m_undoStack->clear(); }

    bool canUndo() const { return m_undoStack->canUndo(); }
    bool canRedo() const { return m_undoStack->canRedo(); }
    QString undoText() const { return m_undoStack->undoText(); }
    QString redoText() const { return m_undoStack->redoText(); }

    int count() const { return m_undoStack->count(); }
    int index() const { return m_undoStack->index(); }
    QString text(int idx) const { return m_undoStack->text(idx); }

    bool isActive() const { return m_undoStack->isActive(); }
    bool isClean() const { return m_undoStack->isClean(); }
    int cleanIndex() const { return m_undoStack->cleanIndex(); }

    void beginMacro(const QString &text) { m_undoStack->beginMacro(text); }
    void endMacro() { m_undoStack->endMacro(); }

    void setUndoLimit(int limit) { m_undoStack->setUndoLimit(limit); }
    int undoLimit() const { return m_undoStack->undoLimit(); }

    const QUndoCommand *command(int index) const { return m_undoStack->command(index); }

public Q_SLOTS:
    void setClean() { m_undoStack->setClean(); }
    void resetClean() { m_undoStack->resetClean(); }
    void setIndex(int idx) { m_undoStack->setIndex(idx); }
    void undo() { m_undoStack->undo(); }
    void redo() { m_undoStack->redo(); }
    void setActive(bool active = true) { m_undoStack->setActive(active); }

Q_SIGNALS:
    void indexChanged(int idx);
    void cleanChanged(bool clean);
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void undoTextChanged(const QString &undoText);
    void redoTextChanged(const QString &redoText);

protected:
    std::unique_ptr<QUndoStack> m_undoStack;
};

} // cmd
} // shared
