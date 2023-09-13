/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "componentimporthelper.h"

#include "abstractproject.h"
#include "asn1reader.h"
#include "asn1systemchecks.h"
#include "componentmodel.h"
#include "diskutils.h"
#include "errorhub.h"
#include "implementationshandler.h"
#include "ivobject.h"

#include <QApplication>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTemporaryDir>

namespace ive {
namespace cmd {

ComponentImportHelper::ComponentImportHelper(
        shared::ComponentModel *componentModel, Asn1Acn::Asn1SystemChecks *asn1Checks, QObject *parent)
    : QObject(parent)
    , m_model(componentModel)
    , m_asn1Checks(asn1Checks)
    , m_destPath(m_asn1Checks ? m_asn1Checks->projectPath() : QString())
{
}

ComponentImportHelper::ComponentImportHelper(
        shared::ComponentModel *componentModel, const QString &projectPath, QObject *parent)
    : QObject(parent)
    , m_model(componentModel)
    , m_asn1Checks(nullptr)
    , m_destPath(projectPath)
{
}

ComponentImportHelper::~ComponentImportHelper() { }

void ComponentImportHelper::setComponentModel(shared::ComponentModel *componentModel)
{
    m_model = componentModel;
}

void ComponentImportHelper::setAsn1SystemChecks(Asn1Acn::Asn1SystemChecks *asn1Checks)
{
    m_asn1Checks = asn1Checks;
    m_destPath = m_asn1Checks->projectPath();
}

bool ComponentImportHelper::isValid() const
{
    return m_model && !m_destPath.isEmpty();
}

void ComponentImportHelper::redoSourcesCloning(const QList<ivm::IVObject *> &objects)
{
    if (!isValid() || objects.isEmpty())
        return;

    struct EntryData {
        ivm::IVObject *object;
        QString sourcePath;
        QString destPath;
    };

    QList<EntryData> objectsForSourceImport;
    QList<EntryData> existingSources;

    for (ivm::IVObject *obj : objects) {
        if (obj->type() != ivm::IVObject::Type::Function && obj->type() != ivm::IVObject::Type::FunctionType)
            continue;

        const shared::VEObject *rootObj = obj->rootObject();
        const QString componentPath = m_model->componentPath(rootObj->id());
        if (componentPath.isEmpty())
            continue;

        const QDir sourceDir = QFileInfo(componentPath).absoluteDir();
        if (!sourceDir.exists())
            continue;

        const QString subPath = relativePathForObject(obj);
        const QString sourcePath = m_tempDir.isNull() ? sourceDir.filePath(subPath) : m_tempDir->path();
        if (!QDir(sourcePath).exists())
            continue;

        const QString destPath = QDir(m_destPath).filePath(subPath);
        const QDir destDir { destPath };
        if (destDir.exists() && destDir.count() > 0) /// TODO: check if we need deeper check of implementation
            existingSources << EntryData { obj, sourcePath, destPath };
        else
            objectsForSourceImport << EntryData { obj, sourcePath, destPath };
    }

    if (!existingSources.isEmpty()) {
        QStringList names;
        std::for_each(existingSources.cbegin(), existingSources.cend(), [&names](const EntryData &data) {
            names.append(QLatin1Char('\'') + data.object->titleUI() + QLatin1Char('\''));
        });
        const auto res = QMessageBox::question(qApp->activeWindow(), tr("Implementations import"),
                tr("There are existing implementations in the project for objects: %1.\nDo you want to overwrite?")
                        .arg(names.join(QLatin1String(", "))),
                QMessageBox::Abort | QMessageBox::YesToAll | QMessageBox::No);
        if (res == QMessageBox::Abort) {
            return;
        } else if (res == QMessageBox::YesToAll) {
            objectsForSourceImport << existingSources;
        }
    }

    QStringList failedObjects;
    for (const EntryData &entryData : qAsConst(objectsForSourceImport)) {
        if (!shared::copyDir(entryData.sourcePath, entryData.destPath, shared::Overwrite)) {
            failedObjects << entryData.object->titleUI();
        } else {
            m_importedSources << entryData.object;
        }
    }
    if (!failedObjects.isEmpty()) {
        QMessageBox::critical(qApp->activeWindow(), tr("Implementations import"),
                tr("Issues during implementations import for objects: %1")
                        .arg(failedObjects.join(QLatin1String(", "))));
    }
}

void ComponentImportHelper::redoSourcesCloning(const ivm::IVObject *object)
{
    if (!isValid() || !object)
        return;

    if (object->type() != ivm::IVObject::Type::Function && object->type() != ivm::IVObject::Type::FunctionType)
        return;

    const shared::VEObject *rootObj = object->rootObject();
    const QString componentPath = m_model->componentPath(rootObj->id());
    if (componentPath.isEmpty())
        return;

    const QDir sourceDir = QFileInfo(componentPath).absoluteDir();
    if (!sourceDir.exists())
        return;

    const QString subPath = relativePathForObject(object);
    const QString sourcePath = m_tempDir.isNull() ? sourceDir.filePath(subPath) : m_tempDir->path();
    if (!QDir(sourcePath).exists())
        return;

    if (!shared::copyDir(sourcePath, QDir(m_destPath).filePath(subPath))) {
        QMessageBox::critical(qApp->activeWindow(), tr("Implementations import"),
                tr("Issues during implementations import for object: %1").arg(object->titleUI()));
    } else {
        m_importedSources << object;
    }
}

void ComponentImportHelper::undoSourcesCloning()
{
    if (m_importedSources.isEmpty())
        return;

    QStringList objNames;
    for (auto obj : qAsConst(m_importedSources)) {
        objNames.append(QLatin1Char('\'') + obj->titleUI() + QLatin1Char('\''));
    }
    if (QMessageBox::No
            == QMessageBox::question(qApp->activeWindow(), tr("Implementations import undo"),
                    tr("Do you want to remove/restore implementations files for objects: %1?")
                            .arg(objNames.join(QLatin1String(", "))))) {
        return;
    }

    objNames.clear();
    for (auto obj : qAsConst(m_importedSources)) {
        const QString subPath = relativePathForObject(obj);
        const QString destPath = m_tempDir->path() + QDir::separator() + subPath;
        const QString sourcePath { m_destPath + QDir::separator() + subPath };
        if (!shared::moveDir(sourcePath, destPath, shared::Overwrite)) {
            objNames.append(QLatin1Char('\'') + obj->titleUI() + QLatin1Char('\''));
        }
    }

    if (!objNames.isEmpty()) {
        QMessageBox::critical(qApp->activeWindow(), tr("Implementations import undo"),
                tr("Issues during implementations import undo for objects: %1")
                        .arg(objNames.join(QLatin1String(", "))));
    }
}

void ComponentImportHelper::redoAsnFilesImport(const ivm::IVObject *object)
{
    if (!isValid() || !object)
        return;

    if (object->type() != ivm::IVObject::Type::Function && object->type() != ivm::IVObject::Type::FunctionType)
        return;

    const QFileInfo componentFileInfo { m_model->componentPath(object->id()) };
    const QDir componentDir = componentFileInfo.absoluteDir();
    const QStringList asnFiles = m_model->asn1Files(object->id());
    if (asnFiles.isEmpty())
        return;

    const QDir targetDir { m_destPath };
    QVector<QFileInfo> fileInfos;
    QHash<QString, QString> fileInfosMapping;
    const QProcessEnvironment prEnv = QProcessEnvironment::systemEnvironment();
    std::for_each(asnFiles.cbegin(), asnFiles.cend(), [&](const QString &fileName) {
        static const QRegularExpression rx { QLatin1String("\\$\\(([a-zA-Z_]{1,}[a-zA-Z0-9_]*)\\)") };
        QString asn1File(fileName);
        const QRegularExpressionMatch match = rx.match(asn1File);
        if (match.hasMatch()) {
            asn1File.replace(match.captured(0), prEnv.value(match.captured(1)));
        }

        QFileInfo fi(asn1File);
        if (!fi.isAbsolute()) {
            fi.setFile(componentDir.filePath(asn1File));
        }

        const QString destFilePath { targetDir.filePath(fi.fileName()) };
        if (!shared::isSame(destFilePath, fi.absoluteFilePath()) && !asn1File.startsWith(QLatin1String("work/"))) {
            fileInfos.append(fi);
            if (match.hasMatch()) {
                fileInfosMapping.insert(fi.absoluteFilePath(), fileName);
            }
        }
    });

    auto alreadyExistingModules = asn1ModuleDuplication(m_asn1Checks, fileInfos);
    if (!alreadyExistingModules.isEmpty()) {
        auto res = QMessageBox::question(qApp->activeWindow(), tr("Import ASN1 files"),
                tr("%1 module(s) already exist(s) in project or have(s) same type assignment(s), "
                   "do you want add it/them anyway?")
                        .arg(alreadyExistingModules.join(", ")));
        if (res == QMessageBox::No) {
            return; // CHECK: probably skip only files with module duplication
        }
    }

    QStringList importedAsnFiles;
    for (const QFileInfo &file : qAsConst(fileInfos)) {
        if (!file.absoluteFilePath().startsWith(componentDir.absolutePath())) {
            /// external ref
            importedAsnFiles.append(fileInfosMapping.value(file.absoluteFilePath()));
            continue;
        }

        QString destFilePath { targetDir.filePath(file.fileName()) };
        if (QFile::exists(destFilePath)) {
            auto res = QMessageBox::question(qApp->activeWindow(), tr("Import ASN1 files"),
                    tr("%1 already exists in project directory, do you want to rename importing one")
                            .arg(file.fileName()));
            if (res == QMessageBox::No) {
                continue;
            }
            bool ok;
            const QString text = QInputDialog::getText(qApp->activeWindow(), tr("Rename importing ASN1 file"),
                    tr("File name:"), QLineEdit::Normal, file.fileName(), &ok);
            if (!ok || !text.isEmpty())
                destFilePath = targetDir.filePath(text);
        }
        if (shared::copyFile(file.absoluteFilePath(), destFilePath)) {
            importedAsnFiles.append(destFilePath);
        } else {
            if (QFile::exists(destFilePath))
                importedAsnFiles.append(destFilePath);
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("%1 wasn't imported").arg(file.fileName()));
        }
    }
    if (!importedAsnFiles.isEmpty()) {
        m_importedAsnFiles << importedAsnFiles;
        Q_EMIT asn1FilesImported(importedAsnFiles);
    }
}

void ComponentImportHelper::undoAsnFilesImport()
{
    if (!isValid() || m_importedAsnFiles.isEmpty())
        return;

    const auto res = QMessageBox::question(qApp->activeWindow(), tr("Import ASN1 files undo"),
            tr("Do you want to remove ASN1 files: %1?").arg(m_importedAsnFiles.join(QLatin1String(", "))));
    if (res == QMessageBox::No) {
        return;
    }

    for (const QString &filePath : qAsConst(m_importedAsnFiles)) {
        QFile::remove(filePath);
    }

    Q_EMIT asn1FilesRemoved(m_importedAsnFiles);
}

QString ComponentImportHelper::relativePathForObject(const ivm::IVObject *object) const
{
    return ive::kRootImplementationPath + QDir::separator() + object->title().toLower();
}

QStringList ComponentImportHelper::asn1ModuleDuplication(
        Asn1Acn::Asn1SystemChecks *asn1Checks, QVector<QFileInfo> &asn1FileInfos)
{
    if (asn1FileInfos.empty()) {
        return {};
    }
    Asn1Acn::Asn1Reader parser;
    QStringList errorMessages;
    QStringList modulesNames;
    const std::map<QString, std::unique_ptr<Asn1Acn::File>> asn1Data =
            parser.parseAsn1Files(asn1FileInfos, &errorMessages);
    if (asn1Data.empty()) {
        QStringList filenames;
        for (const QFileInfo &fi : qAsConst(asn1FileInfos)) {
            filenames.append(fi.fileName());
        }
        shared::ErrorHub::addError(shared::ErrorItem::Error,
                QObject::tr("%1 was/were not imported: ").arg(filenames.join(", ")), errorMessages.join(", "));
        return modulesNames;
    }

    QStringList projectDefinitionsNames;
    QStringList projectTypeAssignmentsNames;
    const std::vector<Asn1Acn::Definitions *> &projectDefinitions = asn1Checks->definitionsList();
    for_each(projectDefinitions.cbegin(), projectDefinitions.cend(), [&](Asn1Acn::Definitions *defs) {
        projectDefinitionsNames << defs->name();
        projectTypeAssignmentsNames << defs->typeAssignmentNames();
    });
    const QStringList files = asn1Checks->project() ? asn1Checks->project()->allAsn1Files() : QStringList();
    for (auto it = asn1Data.cbegin(); it != asn1Data.cend(); ++it) {
        for (const auto &defs : it->second->definitionsList()) {
            if (files.contains(it->first)) {
                auto infoIt = std::find_if(asn1FileInfos.begin(), asn1FileInfos.end(),
                        [filePath = it->first](const QFileInfo &fi) { return fi.filePath() == filePath; });
                if (infoIt != asn1FileInfos.end())
                    asn1FileInfos.erase(infoIt);
                continue;
            }

            if (projectDefinitionsNames.contains(defs->name())) {
                modulesNames.append(defs->name());
            } else {
                for (const QString &typeAssignment : defs->typeAssignmentNames()) {
                    if (projectTypeAssignmentsNames.contains(typeAssignment)) {
                        modulesNames.append(defs->name());
                    }
                }
            }
        }
    }
    return modulesNames;
}

} // namespace ive
} // namespace cmd
