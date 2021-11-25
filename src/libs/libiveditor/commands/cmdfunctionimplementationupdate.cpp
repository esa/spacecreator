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

#include "cmdfunctionimplementationupdate.h"

#include "commandids.h"
#include "ivfunction.h"

#include <QDir>

namespace ive {
namespace cmd {

CmdFunctionImplementationUpdate::CmdFunctionImplementationUpdate(
        const QString &projectPath, ivm::IVFunction *entity, int idx, const EntityAttribute &values)
    : m_function(entity)
    , m_idx(idx)
    , m_newValues(values)
    , m_oldValues(m_function->implementations().value(m_idx))
    , m_projectPath(projectPath)
{
    Q_ASSERT(m_function);
    Q_ASSERT(idx >= 0 && idx < m_function->implementations().size());
    if (m_oldValues == m_newValues) {
        setObsolete(true);
    }
}

void CmdFunctionImplementationUpdate::redo()
{
    const bool isDefault = m_function->defaultImplementation() == m_oldValues.name();
    if (!isDefault) {
        if (m_oldValues.name() != m_newValues.name()) {
            moveDirectories(m_oldValues.name(), m_newValues.name());
        }
    } else {
        m_function->setDefaultImplementation(m_newValues.name());
    }
    m_function->setImplementation(m_idx, m_newValues);
    Q_EMIT implementationChanged(m_function.data(), m_newValues.name(), m_oldValues.name(), this);
}

void CmdFunctionImplementationUpdate::undo()
{
    const bool isDefault = m_function->defaultImplementation() == m_newValues.name();
    if (!isDefault) {
        if (m_newValues.name() != m_oldValues.name()) {
            moveDirectories(m_newValues.name(), m_oldValues.name());
        }
    } else {
        m_function->setDefaultImplementation(m_oldValues.name());
    }
    m_function->setImplementation(m_idx, m_oldValues);
    Q_EMIT implementationChanged(m_function.data(), m_oldValues.name(), m_newValues.name(), this);
}

int CmdFunctionImplementationUpdate::id() const
{
    return UpdateFunctionImplementation;
}

void CmdFunctionImplementationUpdate::moveDirectories(const QString &currentImplName, const QString &nextImplName)
{
    if (currentImplName == nextImplName) {
        return;
    }

    const QStringList pathTemplate { m_projectPath, shared::kRootImplementationPath, m_function->title().toLower(),
        shared::kNonCurrentImplementationPath };

    QDir dir(pathTemplate.join(QDir::separator()));
    dir.rename(currentImplName, nextImplName);
}

} // namespace cmd
} // namespace ive
