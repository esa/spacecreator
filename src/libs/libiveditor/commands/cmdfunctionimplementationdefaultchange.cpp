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
#include "implementationshandler.h"
#include "ivcommonprops.h"
#include "ivfunction.h"
#include "ivpropertytemplateconfig.h"

#include <QDir>

namespace ive {
namespace cmd {

static inline QString languageToken()
{
    static auto token = ivm::meta::Props::token(ivm::meta::Props::Token::language);
    return token;
}

static inline QString implToken()
{
    static auto token = ivm::meta::Props::token(ivm::meta::Props::Token::default_implementation);
    return token;
}

static inline QList<EntityAttribute> attributes(const QString &defaultName, const QString &language)
{
    return { EntityAttribute { implToken(), defaultName, EntityAttribute::Type::Attribute },
        EntityAttribute { languageToken(), language, EntityAttribute::Type::Attribute } };
}

CmdFunctionImplementationDefaultChange::CmdFunctionImplementationDefaultChange(
        const QString &projectPath, ivm::IVFunction *function, const QString &defaultName, const QString &language)
    : CmdFunctionAttrChange(ivm::IVPropertyTemplateConfig::instance(), function, attributes(defaultName, language))
    , m_implHandler(projectPath, function)
{
    if (defaultName == function->defaultImplementation()) {
        setObsolete(true);
    } else {
        m_implHandler.checkOldImplementation();
    }
}

void CmdFunctionImplementationDefaultChange::redo()
{
    const QFileInfo fileInfo(oldCurrentImplementationPath());
    if (fileInfo.isSymLink()) {
        QFile::remove(fileInfo.absoluteFilePath());
    } else if (fileInfo.exists()) {
        if (!shared::moveDir(
                    fileInfo.absoluteFilePath(), oldImplementationPath(), shared::FileCopyingMode::Overwrite)) {
            /// TODO: ROLLBACK;
        }
    }

    shared::ensureDirExists(newImplementationPath());
    const QFileInfo link { newCurrentImplementationPath() };
    if (QFile::link(link.dir().relativeFilePath(newImplementationPath()), link.absoluteFilePath())) {
        CmdFunctionAttrChange::redo();
    }
}

void CmdFunctionImplementationDefaultChange::undo()
{
    const QFileInfo fileInfo(newCurrentImplementationPath());
    if (fileInfo.isSymLink()) {
        QFile::remove(fileInfo.absoluteFilePath());
    }

    const QFileInfo link { oldCurrentImplementationPath() };
    if (QFile::link(link.dir().relativeFilePath(oldImplementationPath()), link.absoluteFilePath())) {
        CmdFunctionAttrChange::undo();
    }
}

int CmdFunctionImplementationDefaultChange::id() const
{
    return ChangeFunctionDefaultImplementation;
}

QString CmdFunctionImplementationDefaultChange::currentImplementationPath(const QString &language) const
{
    return m_implHandler.functionBasePath() + QDir::separator()
            + ivm::IVPropertyTemplateConfig::instance()->languageDirectory(language);
}

QString CmdFunctionImplementationDefaultChange::newCurrentImplementationPath() const
{
    return currentImplementationPath(m_newAttrs.value(languageToken()).toString());
}
QString CmdFunctionImplementationDefaultChange::oldCurrentImplementationPath() const
{
    return currentImplementationPath(m_oldAttrs.value(languageToken()).toString());
}

QString CmdFunctionImplementationDefaultChange::newImplementationPath() const
{
    return m_implHandler.implementationPath(m_newAttrs.value(implToken()).toString(), m_newAttrs.value(languageToken()).toString());
}

QString CmdFunctionImplementationDefaultChange::oldImplementationPath() const
{
    return m_implHandler.implementationPath(m_oldAttrs.value(implToken()).toString(), m_oldAttrs.value(languageToken()).toString());
}

} // namespace cmd
} // namespace ive
