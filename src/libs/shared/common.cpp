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

#include "diskutils.h"
#include "settingsmanager.h"
#include "standardpaths.h"

#include <QDebug>
#include <QDir>
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

void setWidgetFontColor(QWidget *widget, const QColor &color)
{
    if (!widget || !color.isValid())
        return;

    QPalette p(widget->palette());
    p.setColor(QPalette::Text, color);
    widget->setPalette(p);
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

bool isForbiddenName(const QString &name)
{
    static const QSet<QString> &reservedWords = shared::forbiddenNamesSet();
    return reservedWords.contains(name.trimmed());
}

bool isValidName(const QString &name)
{
    if (name.isEmpty()) {
        return false;
    }

    if (isForbiddenName(name)) {
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
