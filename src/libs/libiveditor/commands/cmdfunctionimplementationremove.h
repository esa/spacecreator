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
 along with this program. If not, see
 <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "entityattribute.h"

#include <QPointer>
#include <QScopedPointer>
#include <undocommand.h>

class QDir;
class QTemporaryDir;

namespace ivm {
class IVFunction;
}

namespace ive {
namespace cmd {

class CmdFunctionImplementationRemove : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdFunctionImplementationRemove(const QString &projectPath, ivm::IVFunction *entity, int idx);
    ~CmdFunctionImplementationRemove() override;

    void redo() override;
    void undo() override;
    int id() const override;

Q_SIGNALS:
    void implementationListChanged(ivm::IVFunction *entity);

private:
    QDir implementationDir() const;

private:
    QPointer<ivm::IVFunction> m_function;
    EntityAttribute m_value;
    const int m_idx = -1;
    QScopedPointer<QTemporaryDir> m_tempDir;
    const QString m_projectPath;
};

} // namespace cmd
} // namespace ive
