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

#include "cmdfunctionimplementationdefaultchange.h"

#include "commandids.h"
#include "entityattribute.h"
#include "ivcommonprops.h"
#include "ivfunction.h"
#include "ivpropertytemplateconfig.h"

#include <QDir>

namespace ive {
namespace cmd {

static inline QString implToken()
{
    static auto token = ivm::meta::Props::token(ivm::meta::Props::Token::default_implementation);
    return token;
}

static inline QList<EntityAttribute> attributes(const QString &defaultName, const QString &language)
{
    return { EntityAttribute { implToken(), defaultName, EntityAttribute::Type::Attribute },
        EntityAttribute { ivm::meta::Props::token(ivm::meta::Props::Token::language), language,
                EntityAttribute::Type::Attribute } };
}

CmdFunctionImplementationDefaultChange::CmdFunctionImplementationDefaultChange(
        const QString &projectPath, ivm::IVFunction *function, const QString &defaultName, const QString &language)
    : CmdFunctionAttrChange(ivm::IVPropertyTemplateConfig::instance(), function, attributes(defaultName, language))
    , m_projectPath(projectPath)
{
    Q_ASSERT(defaultName != function->defaultImplementation());
    if (defaultName == function->defaultImplementation()) {
        setObsolete(true);
    }
}

void CmdFunctionImplementationDefaultChange::redo()
{
    const QString currentImplName = m_entity->entityAttributeValue<QString>(implToken());
    CmdFunctionAttrChange::redo();
    const QString implName = m_entity->entityAttributeValue<QString>(implToken());

    moveDirectories(currentImplName, implName, m_projectPath, m_entity->title().toLower());
}

void CmdFunctionImplementationDefaultChange::undo()
{
    const QString currentImplName = m_entity->entityAttributeValue<QString>(implToken());
    CmdFunctionAttrChange::undo();
    const QString implName = m_entity->entityAttributeValue<QString>(implToken());

    moveDirectories(currentImplName, implName, m_projectPath, m_entity->title().toLower());
}

int CmdFunctionImplementationDefaultChange::id() const
{
    return ChangeFunctionDefaultImplementation;
}

void CmdFunctionImplementationDefaultChange::moveDirectories(const QString &currentImplName,
        const QString &nextImplName, const QString &projectPath, const QString &functionName)
{
    const QString kDefaultImplPath { projectPath + QDir::separator() + shared::kRootImplementationPath
        + QDir::separator() + functionName };
    const QString kCommonPathTemplate { kDefaultImplPath + QDir::separator() + shared::kNonCurrentImplementationPath };

    const QString currentImplDestPath { kCommonPathTemplate + QDir::separator() + currentImplName };
    if (shared::ensureDirExists(currentImplDestPath)) {
        const QDir dir { kDefaultImplPath };
        const QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &dirName : subfolders) {
            if (dirName != shared::kNonCurrentImplementationPath) {
                const QString subfolderPath = dir.filePath(dirName);
                shared::copyDir(subfolderPath, currentImplDestPath + QDir::separator() + dirName,
                        shared::FileCopyingMode::Overwrite);
                QDir(subfolderPath).removeRecursively();
            }
        }
    }

    const QString nextImplSourcePath { kCommonPathTemplate + QDir::separator() + nextImplName };
    shared::copyDir(nextImplSourcePath, kDefaultImplPath, shared::FileCopyingMode::Overwrite);
    QDir(nextImplSourcePath).removeRecursively();
}

} // namespace cmd
} // namespace ive
