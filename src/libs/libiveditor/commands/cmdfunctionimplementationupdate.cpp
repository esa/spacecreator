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
#include "implementationshandler.h"
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
    , m_implHandler(projectPath, entity)
{
    Q_ASSERT(m_function);
    Q_ASSERT(idx >= 0 && idx < m_function->implementations().size());
    if (m_oldValues == m_newValues) {
        setObsolete(true);
    } else {
        m_implHandler.checkOldImplementation();
    }
}

void CmdFunctionImplementationUpdate::redo()
{
    const bool isDefault = m_function->defaultImplementation() == oldImplementationName();
    if (oldImplementationName() != newImplementationName()) {
        m_implHandler.renameImplementationDirectory(oldImplementationName(), newImplementationName());
    }
    m_function->setImplementation(m_idx, m_newValues);
    if (isDefault) {
        m_function->setDefaultImplementation(newImplementationName());
        m_implHandler.updateDefaultImplementation(oldImplementationName(), oldLanguage(), newImplementationName(), newLanguage(), isDefault); // TODO: remove isDefault
    }
    Q_EMIT implementationChanged(m_function.data(), newImplementationName(), oldImplementationName(), this);
}

void CmdFunctionImplementationUpdate::undo()
{
    const bool isDefault = m_function->defaultImplementation() == newImplementationName();
    if (newImplementationName() != oldImplementationName()) {
        m_implHandler.renameImplementationDirectory(newImplementationName(), oldImplementationName());
    }
    m_function->setImplementation(m_idx, m_oldValues);
    if (isDefault) {
        m_function->setDefaultImplementation(oldImplementationName());
        m_implHandler.updateDefaultImplementation(newImplementationName(), newLanguage(), oldImplementationName(), oldLanguage(), isDefault); // TODO: remove isDefault
    }
    Q_EMIT implementationChanged(m_function.data(), oldImplementationName(), newImplementationName(), this);
}

int CmdFunctionImplementationUpdate::id() const
{
    return UpdateFunctionImplementation;
}

QString CmdFunctionImplementationUpdate::newImplementationName() const
{
    return m_newValues.name();
}

QString CmdFunctionImplementationUpdate::newLanguage() const
{
    return m_newValues.value().toString();
}

QString CmdFunctionImplementationUpdate::oldImplementationName() const
{
    return m_oldValues.name();
}

QString CmdFunctionImplementationUpdate::oldLanguage() const
{
    return m_oldValues.value().toString();
}

} // namespace cmd
} // namespace ive
