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
    } else {
        const QList<EntityAttribute> &impls = entity->implementations();
        auto it = std::find_if(impls.cbegin(), impls.cend(),
                [defaultName = entity->defaultImplementation()](
                        const EntityAttribute &impl) { return impl.name() == defaultName; });
        if (it != impls.cend()) {
            shared::moveDefaultDirectories(
                    entity->defaultImplementation(), projectPath, entity->title().toLower(), it->value().toString());
        }
    }
}

void CmdFunctionImplementationUpdate::redo()
{
    const bool isDefault = m_function->defaultImplementation() == m_oldValues.name();
    if (m_oldValues.name() != m_newValues.name()) {
        moveDirectories(m_oldValues.name(), m_newValues.name());
    }
    m_function->setImplementation(m_idx, m_newValues);
    if (isDefault) {
        m_function->setDefaultImplementation(m_newValues.name());
        updateSymLink(m_oldValues, m_newValues, isDefault); // TODO: remove isDefault
    }
    Q_EMIT implementationChanged(m_function.data(), m_newValues.name(), m_oldValues.name(), this);
}

void CmdFunctionImplementationUpdate::undo()
{
    const bool isDefault = m_function->defaultImplementation() == m_newValues.name();
    if (m_newValues.name() != m_oldValues.name()) {
        moveDirectories(m_newValues.name(), m_oldValues.name());
    }
    m_function->setImplementation(m_idx, m_oldValues);
    if (isDefault) {
        m_function->setDefaultImplementation(m_oldValues.name());
        updateSymLink(m_newValues, m_oldValues, isDefault); // TODO: remove isDefault
    }
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

void CmdFunctionImplementationUpdate::updateSymLink(
        const EntityAttribute &oldValue, const EntityAttribute &newValue, bool isDefault)
{
    const QString currentImplPath = m_projectPath + QDir::separator() + shared::kRootImplementationPath
            + QDir::separator() + m_function->title().toLower() + QDir::separator();
    const QFileInfo oldFileInfo(currentImplPath + oldValue.value().toString());
    if (oldFileInfo.isSymLink()) {
        QFile::remove(oldFileInfo.absoluteFilePath());
    } else if (oldFileInfo.exists()) {
        const QString destPath = currentImplPath + shared::kNonCurrentImplementationPath + QDir::separator()
                + oldValue.name() + QDir::separator() + oldValue.value().toString();
        shared::copyDir(oldFileInfo.absoluteFilePath(), destPath, shared::FileCopyingMode::Overwrite);
        QDir(oldFileInfo.absoluteFilePath()).removeRecursively();
    }
    const QFileInfo newFileInfo(currentImplPath + newValue.value().toString());
    const QString destPath = currentImplPath + shared::kNonCurrentImplementationPath + QDir::separator()
            + newValue.name() + QDir::separator() + newValue.value().toString();
    if (newFileInfo.exists() && newFileInfo.isDir()) {
        shared::copyDir(newFileInfo.absoluteFilePath(), destPath, shared::FileCopyingMode::Overwrite);
        QDir(newFileInfo.absoluteFilePath()).removeRecursively();
    }
    if (isDefault) {
        shared::ensureDirExists(destPath);
        QFile::link(destPath, newFileInfo.absoluteFilePath());
    }
}

} // namespace cmd
} // namespace ive
