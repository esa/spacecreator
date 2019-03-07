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

#include "commands/common/commandids.h"

#include <QObject>
#include <QUndoStack>
#include <QVariant>

namespace msc {
namespace cmd {

class CommandsStack : public QObject
{
    Q_OBJECT
public:
    static CommandsStack *instance();

    static void setCurrent(QUndoStack *stack);
    static QUndoStack *current();

    static bool push(msc::cmd::Id id, const QVariantList &params);

Q_SIGNALS:
    void currentStackChanged(QUndoStack *to);

private:
    CommandsStack(QObject *parent = nullptr);

    void setCurrentStack(QUndoStack *stack);
    QUndoStack *currentStack() const;

    static CommandsStack *m_instance;
    QUndoStack *m_current = nullptr;
};

} // namespace cmd
} // ns msc
