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

#include "cmdfunctionimplementationremove.h"

#include "commandids.h"
#include "ivfunction.h"

#include <QTemporaryDir>

namespace ive {
namespace cmd {

CmdFunctionImplementationRemove::CmdFunctionImplementationRemove(
        const QString &projectPath, ivm::IVFunction *entity, int idx)
    : m_function(entity)
    , m_idx(idx)
    , m_projectPath(projectPath)
{
    Q_ASSERT(m_function);
    Q_ASSERT(idx >= 0 && idx < m_function->implementations().size() + 1);
    m_value = m_function->implementations().value(idx);
    if (entity->defaultImplementation() == m_value.name()) {
        setObsolete(true);
    }
}

CmdFunctionImplementationRemove::~CmdFunctionImplementationRemove() {}

void CmdFunctionImplementationRemove::redo()
{
    if (m_function) {
        m_function->removeImplementation(m_idx);
        m_tempDir.reset(new QTemporaryDir);
        if (m_tempDir->isValid()) {
            QDir dir = implementationDir();
            shared::copyDir(dir.path(), m_tempDir->path(), shared::FileCopyingMode::Overwrite);
            dir.removeRecursively();
            Q_EMIT implementationListChanged(m_function.data());
        }
    }
}

void CmdFunctionImplementationRemove::undo()
{
    if (m_function) {
        m_function->insertImplementation(m_idx, m_value);
        if (m_tempDir && m_tempDir->isValid()) {
            QDir dir = implementationDir();
            dir.removeRecursively();
            shared::copyDir(m_tempDir->path(), dir.path(), shared::FileCopyingMode::Overwrite);
            m_tempDir.reset();
        }
        Q_EMIT implementationListChanged(m_function.data());
    }
}

int CmdFunctionImplementationRemove::id() const
{
    return RemoveFunctionImplementation;
}

QDir CmdFunctionImplementationRemove::implementationDir() const
{
    return QStringList { m_projectPath, shared::kRootImplementationPath, m_function->title().toLower(),
        shared::kNonCurrentImplementationPath, m_value.name() }
            .join(QDir::separator());
}

} // namespace cmd
} // namespace ive
