/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "implementationshandler.h"

#include "diskutils.h"
#include "entityattribute.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivpropertytemplateconfig.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <algorithm>

namespace ive {

static const QString projectSuffix { "pro" };

ImplementationsHandler::ImplementationsHandler(const QString &projectPath, ivm::IVFunctionType *entity)
    : m_projectPath(projectPath)
    , m_function(entity)
{
}

/**
 * Old implementations had the implemenations directly in the subdirectory ive::kRootImplementationPath ("work").
 * With the introductions of multiple implementations the implementations are moved to the subdirectory
 * ive::kNonCurrentImplementationPath ("implem"). And the default implementation is just a link to that implementation.
 *
 * This function "converts" old versions to new ones (moving directories, setting the link)
 */
void ImplementationsHandler::checkOldImplementation()
{
    if (!m_function || m_function->type() != ivm::IVObject::Type::Function) {
        return;
    }

    auto func = static_cast<ivm::IVFunction *>(m_function.data());
    const QList<EntityAttribute> &impls = func->implementations();
    auto it = std::find_if(
            impls.cbegin(), impls.cend(), [defaultName = func->defaultImplementation()](const EntityAttribute &impl) {
                return impl.name() == defaultName;
            });
    if (it != impls.cend()) {
        moveDefaultDirectories(func->defaultImplementation(), it->value().toString());
    }
}

void ImplementationsHandler::renameImplementationDirectory(const QString &currentImplName, const QString &nextImplName)
{
    if (currentImplName == nextImplName) {
        return;
    }

    QDir dir(functionImplementaionsBasePath());
    dir.rename(currentImplName, nextImplName);
}

void ImplementationsHandler::updateDefaultImplementation(const QString &oldName, const QString &oldLanguage,
        const QString &newName, const QString &newLanguage, bool isDefault)
{
    const QString currentImplPath = functionBasePath() + QDir::separator();
    const QString oldLanguageDir = ivm::IVPropertyTemplateConfig::instance()->languageDirectory(oldLanguage);
    const QFileInfo oldFileInfo(currentImplPath + oldLanguageDir);
    if (oldFileInfo.isSymLink()) {
        QFile::remove(oldFileInfo.absoluteFilePath());
    } else if (oldFileInfo.exists()) {
        const QString destPath = implementationPath(oldName, oldLanguage);
        if (!shared::moveDir(oldFileInfo.absoluteFilePath(), destPath, shared::FileCopyingMode::Overwrite)) {
            /// TODO: ROLLBACK
        }
    }
    const QString newLanguageDir = ivm::IVPropertyTemplateConfig::instance()->languageDirectory(newLanguage);
    const QFileInfo newFileInfo(currentImplPath + newLanguageDir);
    const QString destPath = implementationPath(newName, newLanguageDir);
    if (newFileInfo.exists() && newFileInfo.isDir()) {
        if (!shared::moveDir(newFileInfo.absoluteFilePath(), destPath, shared::FileCopyingMode::Overwrite)) {
            /// TODO: ROLLBACK
        }
    }
    if (isDefault) {
        shared::ensureDirExists(destPath);
        QFile::link(newFileInfo.dir().relativeFilePath(destPath), newFileInfo.absoluteFilePath());
    }
}

/**
 * Returns directory path where all implementations of the functions are located.
 * The default implementation is a link. All code is in a subdirectory
 * Example: <project path>/work/<function name>
 * @param functionName Use this name as function name. If not set or empty, the name of the function entity is used
 */
QString ImplementationsHandler::functionBasePath(const QString &functionName) const
{
    QString name = functionName.isEmpty() ? m_function->title().toLower() : functionName.toLower();
    name = name.toLower();
    QStringList pathTemplate { m_projectPath, shared::kRootImplementationPath, name };
    return pathTemplate.join(QDir::separator());
}

/**
 * Returns directory path where all implementations of the functions are located.
 * The implementations are in subdiretories of the implementation's names.
 * Example: <project path>/work/<function name>/implem
 * @param functionName Use this name as function name. If not set or empty, the name of the function entity is used
 */
QString ImplementationsHandler::functionImplementaionsBasePath(const QString &functionName) const
{
    return functionBasePath(functionName) + QDir::separator() + shared::kNonCurrentImplementationPath;
}

/**
 * Returns the directory path of one specific implementation. Not including the source directory.
 * @param name The implementation's name
 * Example: <project path>/work/<function name>/implem/<impl. name>
 * @param functionName Use this name as function name. If not set or empty, the name of the function entity is used
 */
QString ImplementationsHandler::implementationBasePath(const QString &name, const QString &functionName) const
{
    return functionImplementaionsBasePath(functionName) + QDir::separator() + name;
}

/**
 * Returns the directory path of one specific implementation.
 * @param name The implementation's name
 * @param language The programming language
 * @param functionName Use this name as function name. If not set or empty, the name of the function entity is used
 * Example: <project path>/work/<function name>/implem/<impl. name>/<language>
 */
QString ImplementationsHandler::implementationPath(
        const QString &name, const QString &language, const QString &functionName) const
{
    const QString languageDir = ivm::IVPropertyTemplateConfig::instance()->languageDirectory(language);
    return implementationBasePath(name, functionName) + QDir::separator() + languageDir;
}

QString ImplementationsHandler::projectFilename(const QString &functionName)
{
    return QString("%1.%2").arg(functionName.toLower(), projectSuffix);
}

QString ImplementationsHandler::implementationDirectory(const QString &functionName)
{
    return functionName.toLower();
}

bool ImplementationsHandler::moveDefaultDirectories(const QString &currentImplName, const QString &language)
{
    const QString defaultImplPath { functionBasePath() };
    const QString commonImplPath { functionImplementaionsBasePath() + QDir::separator() + currentImplName };

    bool result = true;
    if (shared::ensureDirExists(commonImplPath)) {
        QDir dir { defaultImplPath };
        const QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (const QString &dirName : subfolders) {
            if (dirName != shared::kNonCurrentImplementationPath) {
                const QString subfolderPath = dir.filePath(dirName);
                const QString destPath = commonImplPath + QDir::separator() + dirName;
                const bool res = shared::copyDir(subfolderPath, destPath, shared::FileCopyingMode::Overwrite);
                result &= res;
                if (res)
                    result &= QDir(subfolderPath).removeRecursively();
            }
        }
    }

    const QString languageDir = ivm::IVPropertyTemplateConfig::instance()->languageDirectory(language);

    const QFileInfo link { defaultImplPath + QDir::separator() + languageDir };
    const QString linkTargetPath = commonImplPath + QDir::separator() + languageDir;
    shared::ensureDirExists(linkTargetPath);
    if (link.isSymLink()) {
        result &= link.symLinkTarget() == linkTargetPath;
    } else {
        result &= QFile::link(link.dir().relativeFilePath(linkTargetPath), link.absoluteFilePath());
    }
    return result;
}

}
