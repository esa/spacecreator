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

#include "common.h"

#include "settingsmanager.h"
#include "standardpaths.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QPalette>
#include <QRegularExpressionMatch>
#include <QWidget>

#ifdef Q_OS_WIN
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

namespace shared {
namespace env {
const char *kSharedTypesLibrary = "TASTE_SHARED_TYPES";
const char *kComponentsLibrary = "TASTE_COMPONENTS_LIBRARY";
const char *kInterfaceAttrs = "TASTE_DEPLOYMENT_ATTRIBUTES_PATH";
const char *kDeploymentAttrs = "TASTE_DEFAULT_ATTRIBUTES_PATH";
const char *kDeploymentLibrary = "TASTE_DEPLOYMENT_HW_PATH";
} // namespace env

/*!
  \namespace shared

  \brief The collection of utilities shared among all submodules.
*/

/*!
 * \typedef Id
 * \brief The common Id used as internal objects referring.
 */

/*!
 * \brief InvalidId
 * Global constant representing the empty/invalid ID.
 */

Id createId()
{
    return QUuid::createUuid();
}

/*!
  \enum shared::FileCopyingMode

  This enum specifies a file overwriting policy:
    \var Keep
        Do not overwrite existing file.
    \var Overwrite
        Do overwrite existing file.
 */

/*!
 * \brief Copies the \a source file from resources to the \a target file.

 * Returns \c true if the \a source file copied successfully and
 * the QFile::WriteUser permission explicitly set for the \a target
 * (otherwise it would be read-only as any file in qrc).
 * If the \a target file already exists, this function will not overwrite
 * and return \c false;
 */
bool copyFile(const QString &source, const QString &target, FileCopyingMode replaceMode)
{
    if (source.isEmpty() || target.isEmpty()) {
        return false;
    }
    if (!QFile::exists(source)) {
        qWarning() << "Source file " << source << " does not exist";
    }

    if (QFile::exists(target)) {
        if (FileCopyingMode::Overwrite == replaceMode) {
            if (!QFile::remove(target)) {
                qWarning() << "Unable to remove old file" << target;
                return false;
            }
        } else {
            return false;
        }
    }

    bool result(false);
#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif
    try {
        ensureDirExists(QFileInfo(target).path());
        if (QFile::copy(source, target)) {
            QFile storedFile(target);
            result = storedFile.setPermissions(QFile::WriteUser | QFile::ReadUser);
            if (!result) {
                qWarning() << "Unable to set permissions for " << target;
            }
        } else {
            qWarning() << "Can't copy resource file " << source << "-->" << target;
        }
    } catch (...) {
        qWarning() << "Failed to copy resource file " << source << "-->" << target;
    }
#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup--;
#endif
    return result;
}

void setWidgetFontColor(QWidget *widget, const QColor &color)
{
    if (!widget || !color.isValid())
        return;

    QPalette p(widget->palette());
    p.setColor(QPalette::Text, color);
    widget->setPalette(p);
}

bool ensureDirExists(const QString &path)
{
    QDir dir(path);
    if (!dir.exists(path)) {
        if (!dir.mkpath(path)) {
            qWarning() << "Failed to create path:" << path;
            return false;
        }
    }

    return true;
}

bool ensureFileExists(const QString &filePath, const QString &defaultFilePath)
{
    if (!QFileInfo::exists(filePath) && !copyFile(defaultFilePath, filePath)) {
        qWarning() << "Can't create default file path:" << filePath << "from:" << defaultFilePath;
        return false;
    }
    return true;
}

bool copyDir(const QString &source, const QString &dest, FileCopyingMode replaceMode)
{
    static const QDir::Filters filters = QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files;
    QDir sourceExportDir { source };
    if (sourceExportDir.isEmpty(filters)) {
        return true;
    }
    sourceExportDir.setFilter(filters);
    const QDir targetExportDir { dest };
    targetExportDir.mkpath(QLatin1String("."));
    QDirIterator it { sourceExportDir, QDirIterator::Subdirectories };
    bool success = true;
    while (it.hasNext()) {
        const QString filePath = it.next();
        const QFileInfo fileInfo = it.fileInfo();
        const QString relPath = sourceExportDir.relativeFilePath(fileInfo.absoluteFilePath());
        if (fileInfo.isSymLink()) {
            const QString linkTarget = fileInfo.dir().relativeFilePath(fileInfo.symLinkTarget());
            const bool result = QFile::link(linkTarget, targetExportDir.absoluteFilePath(relPath));
            if (!result) {
                qWarning() << "Error during symlink copying:" << filePath << fileInfo.symLinkTarget()
                           << targetExportDir.absoluteFilePath(relPath);
            }
            success &= result;
        } else if (fileInfo.isDir()) {
            if (!QDir(targetExportDir.filePath(relPath)).exists()) {
                const bool result = targetExportDir.mkpath(relPath);
                success &= result;
            }
        } else {
            const bool result =
                    copyFile(fileInfo.absoluteFilePath(), targetExportDir.absoluteFilePath(relPath), replaceMode);
            if (!result) {
                qWarning() << "Error during source file copying:" << filePath
                           << targetExportDir.absoluteFilePath(relPath);
            }
            success &= result;
        }
    }
    return success;
}

QString archetypesFileStartingString()
{
    static const QString kArchetypesStartingString = QString("archetype_library_");
    return kArchetypesStartingString;
}

QString interfaceCustomArchetypesDirectoryPath()
{
    static const QString kArchetypesPath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("archetypes");

    return kArchetypesPath;
}

QString interfaceCustomAttributesFilePath()
{
    static const QString kDefaultPath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("default_attributes.xml");

    return qEnvironmentVariable(env::kInterfaceAttrs, kDefaultPath);
}

QString componentsLibraryPath()
{
    static const QString kDefaultPath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("components_library") + QDir::separator();
    const QString componentsFile =
            shared::SettingsManager::load<QString>(shared::SettingsManager::IVE::ComponentsPath, kDefaultPath);

    return qEnvironmentVariable(env::kComponentsLibrary, componentsFile);
}

QString sharedTypesPath()
{
    static const QString kDefaultPath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("shared_types") + QDir::separator();
    const QString sharedTypesFile =
            shared::SettingsManager::load<QString>(shared::SettingsManager::IVE::SharedTypesPath, kDefaultPath);

    return qEnvironmentVariable(env::kSharedTypesLibrary, sharedTypesFile);
}

QString deploymentCustomAttributesFilePath()
{
    static const QString kDefaultPath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("deployment_attributes.xml");

    return qEnvironmentVariable(env::kDeploymentAttrs, kDefaultPath);
}

QString hwLibraryPath()
{
    static const QString kDefaultPath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("HWlibrary");
    QString hwFile = shared::SettingsManager::load<QString>(shared::SettingsManager::DVE::HwLibraryFile, kDefaultPath);

    return qEnvironmentVariable(env::kDeploymentLibrary, hwFile);
}

QSet<QString> forbiddenNamesSet()
{
    static const QString kFilePath = shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("forbidden_names.txt");
    static const QString kDefaultPath = QLatin1String(":/defaults/forbidden_names.txt");
    if (shared::ensureFileExists(kFilePath, kDefaultPath)) {
        QFile f(kFilePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Can't open file:" << kFilePath << f.errorString();
            return {};
        }
        QSet<QString> names;
        QTextStream stream(&f);
        QString line;
        while (stream.readLineInto(&line)) {
            names << line.trimmed();
        }
        return names;
    }
    return {};
}

bool isValidName(const QString &name)
{
    if (name.isEmpty()) {
        return false;
    }

    static const QSet<QString> reservedWords = shared::forbiddenNamesSet();
    if (reservedWords.contains(name.trimmed())) {
        return false;
    }

    static const QRegularExpression re(shared::namePatternUI);
    const QRegularExpressionMatch match = re.match(name);
    return match.hasMatch();
}

QString joinNonEmpty(const QStringList &values, const QString &lineBreak)
{
    QStringList filtered(values);
    filtered.removeAll(QString());
    return filtered.join(lineBreak);
}

bool moveDir(const QString &source, const QString &dest, FileCopyingMode replaceMode)
{
    if (copyDir(source, dest, replaceMode))
        return QDir(source).removeRecursively();

    return false;
}

bool isSame(const QString &filePath1, const QString &filePath2)
{
    QFile file1(filePath1);
    QFile file2(filePath2);

    if (file1.size() != file2.size())
        return false;

    if (!file1.open(QIODevice::ReadOnly)) {
        qWarning() << file1.fileName() << file1.errorString();
        return false;
    }
    if (!file2.open(QIODevice::ReadOnly)) {
        qWarning() << file2.fileName() << file2.errorString();
        return false;
    }

    /// TODO: Implement additional instance for ASN1 specific needs
    /// with loading ASN1 model and comparing objects instead
    /// For other types add functionality to skip spaces/empty lines/etc
    while (!file1.atEnd() && !file2.atEnd()) {
        if (file1.readLine() != file2.readLine())
            return false;
    }
    return true;
}

}
