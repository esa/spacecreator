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

#include "asn1componentsimport.h"

#include "asn1reader.h"
#include "asn1systemchecks.h"
#include "errorhub.h"
#include "implementationshandler.h"
#include "ivobject.h"

#include <QApplication>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QTemporaryDir>

namespace ive {
namespace cmd {

ASN1ComponentsImport::ASN1ComponentsImport(
        Asn1Acn::Asn1SystemChecks *asn1Checks, const QString &srcPath, const QString &destPath)
    : QObject()
    , m_asn1Checks(asn1Checks)
    , m_srcPath(srcPath)
    , m_destPath(destPath)
{
}

ASN1ComponentsImport::~ASN1ComponentsImport() { }

void ASN1ComponentsImport::redoSourceCloning(const ivm::IVObject *object)
{
    if (!object
            || !(object->type() == ivm::IVObject::Type::Function
                    || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QStringList objPath = ivm::IVObject::path(object);
    const QString rootName = objPath.isEmpty() ? object->title() : objPath.front();
    const QString subPath = relativePathForObject(object);
    const QString sourcePath = m_tempDir.isNull() ? m_srcPath + QDir::separator() + rootName : m_tempDir->path();
    const QDir sourceDir { sourcePath };
    const QDir targetDir { m_destPath };
    shared::copyDir(sourceDir.filePath(subPath), targetDir.filePath(subPath));
}

void ASN1ComponentsImport::undoSourceCloning(const ivm::IVObject *object)
{
    if (!object
            || !(object->type() == ivm::IVObject::Type::Function
                    || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QString subPath = relativePathForObject(object);
    const QString destPath = m_tempDir->path() + QDir::separator() + subPath;
    const QString sourcePath { m_destPath + QDir::separator() + subPath };
    if (!shared::moveDir(sourcePath, destPath)) {
        /// TODO: ROLLBACK
    }
}

static inline bool isSame(const QString &filePath1, const QString &filePath2)
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

static inline QStringList asn1ModuleDuplication(
        Asn1Acn::Asn1SystemChecks *asn1Checks, const QVector<QFileInfo> &asn1FileInfos)
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

    for (auto it = asn1Data.cbegin(); it != asn1Data.cend(); ++it) {
        for (const auto &defs : it->second->definitionsList()) {
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

void ASN1ComponentsImport::redoAsnFileImport(const ivm::IVObject *object)
{
    if (!object
            || !(object->type() == ivm::IVObject::Type::Function
                    || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QStringList objPath = ivm::IVObject::path(object);
    const QString rootName = objPath.isEmpty() ? object->title() : objPath.front();
    const QString sourcePrefix = m_tempDir.isNull() ? m_srcPath : m_tempDir->path();
    const QDir sourceDir { sourcePrefix + QDir::separator() + rootName };
    const QDir targetDir { m_destPath };

    QList<QFileInfo> asnFiles = sourceDir.entryInfoList(
            { QLatin1String("*.asn1"), QLatin1String("*.asn"), QLatin1String("*.acn") }, QDir::Files);

    QVector<QFileInfo> fileInfos;
    std::copy_if(asnFiles.begin(), asnFiles.end(), std::back_inserter(fileInfos), [targetDir](const QFileInfo &fi) {
        const QString destFilePath { targetDir.filePath(fi.fileName()) };
        return !isSame(destFilePath, fi.absoluteFilePath());
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

    for (const QFileInfo &file : qAsConst(fileInfos)) {
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
                    tr("File name:"), QLineEdit::Normal, rootName + file.fileName(), &ok);
            if (!ok || !text.isEmpty())
                destFilePath = targetDir.filePath(text);
        }
        if (shared::copyFile(file.absoluteFilePath(), destFilePath)) {
            m_importedAsnFiles.append(destFilePath);
        } else {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("%1 wasn't imported").arg(file.fileName()));
        }
    }
}

void ASN1ComponentsImport::undoAsnFileImport()
{
    for (const QString &filePath : qAsConst(m_importedAsnFiles)) {
        QFile::remove(filePath);
    }

    Q_EMIT asn1FileRemoved(m_importedAsnFiles);
}

QString ASN1ComponentsImport::relativePathForObject(const ivm::IVObject *object) const
{
    return ive::kRootImplementationPath + QDir::separator() + object->title().toLower();
}

} // namespace ive
} // namespace cmd
