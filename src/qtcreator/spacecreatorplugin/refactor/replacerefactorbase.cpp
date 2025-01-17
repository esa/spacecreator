/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "replacerefactorbase.h"

#include "common.h"
#include "implementationshandler.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "qmakefile.h"
#include "spacecreatorproject.h"

#include <QDir>
#include <QDirIterator>
#include <coreplugin/fileutils.h>
#include <editormanager/documentmodel.h>
#include <texteditor/textdocument.h>

namespace spctr {

bool ReplaceRefactorBase::isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const
{
    Q_ASSERT(func);

    const QStringList implFiles = codeFilePaths(name);
    return !implFiles.isEmpty();
}

bool ReplaceRefactorBase::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());

    const QStringList implFiles = codeFilePaths(interface->function()->title());
    const QList<QByteArray> codes = interfaceCodeNames(interface, name);
    for (const QString &implFile : implFiles) {
        QFileInfo fi(implFile);
        if (fileContainsText(fi, codes)) {
            return true;
        }
    }

    return false;
}

void ReplaceRefactorBase::onIVFunctionRenamed(
        ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    Q_ASSERT(func);

    // Update file contents
    const QStringList implFiles = codeFilePaths(oldName);
    const QList<QByteArray> oldTexts = functionsTextsToReplace(oldName);
    const QList<QByteArray> newTexts = functionsTextsToReplace(newName);

    for (const QString &implFile : implFiles) {
        QFileInfo fi(implFile);
        QByteArray fileData = fileContent(fi);
        bool updated = false;
        for (int idx = 0; idx < oldTexts.size(); ++idx) {
            if (fileData.contains(oldTexts[idx])) {
                fileData.replace(oldTexts[idx], newTexts[idx]);
                updated = true;
            }
        }
        if (updated) {
            setFileContent(fi, fileData);
        }
    }

    // rename files
    for (const QString &implFile : allImplementationFilePaths(oldName)) {
        QFileInfo fi(implFile);
        const QString newBasename = fi.baseName().replace(oldName.toLower(), newName.toLower());
        const QString newFileName = fi.path() + QDir::separator() + newBasename + "." + fi.suffix();
        Core::FileUtils::renameFile(
                Utils::FilePath::fromString(fi.absoluteFilePath()), Utils::FilePath::fromString(newFileName));
    }

    // rename file in .pro file(s)
    const QStringList oldImplFiles = implementationFileNames(oldName);
    const QStringList newImplFiles = implementationFileNames(newName);
    const QString implPath =
            m_storage->projectPath() + QDir::separator() + shared::kRootImplementationPath + QDir::separator();
    QDirIterator it(implPath, { ive::ImplementationsHandler::projectFilename(oldName) }, QDir::Files,
            QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString prFilePath = it.next();
        for (int i = 0; i < newImplFiles.size(); ++i) {
            shared::QMakeFile::renameFileName(prFilePath, oldImplFiles[i], newImplFiles[i]);
        }
    }
}

void ReplaceRefactorBase::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    Q_ASSERT(interface && interface->function());
    const QStringList implFiles = codeFilePaths(interface->function()->title());
    const QList<QByteArray> oldCodes = interfaceCodeNames(interface, oldName);
    const QList<QByteArray> newCodes = interfaceCodeNames(interface, newName);

    for (const QString &implFile : implFiles) {
        QFileInfo fi(implFile);
        updateFileText(fi, oldCodes, newCodes);
    }
}

QStringList ReplaceRefactorBase::codeFilePaths(const QString &functionName) const
{
    QStringList implFiles;

    const QStringList implementationFiles = implementationFileNames(functionName);
    QDirIterator it(m_storage->projectPath() + "/" + shared::kRootImplementationPath, implementationFiles, QDir::Files,
            QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString fileName = it.next();
        if (fileName.contains("/" + languageDir() + "/")) {
            implFiles.append(fileName);
        }
    }

    return implFiles;
}

QStringList ReplaceRefactorBase::allImplementationFilePaths(const QString &functionName) const
{
    QStringList implFiles;
    const QString fnNameLow(functionName.toLower());

    QDirIterator it(m_storage->projectPath() + QDir::separator() + shared::kRootImplementationPath + QDir::separator()
                    + fnNameLow,
            QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString filePath = it.next();
        if (filePath.contains(QString("/%1/").arg(languageDir()))) {
            QFileInfo fi(filePath);
            if (fi.baseName().contains(fnNameLow)) {
                implFiles.append(filePath);
            }
        }
    }

    return implFiles;
}

QByteArray ReplaceRefactorBase::fileContent(const QFileInfo &fileinfo) const
{
    Core::IDocument *doc =
            Core::DocumentModel::documentForFilePath(Utils::FilePath::fromString(fileinfo.absoluteFilePath()));
    if (doc) {
        auto textdoc = dynamic_cast<TextEditor::TextDocument *>(doc);
        if (textdoc) {
            return textdoc->contents();
        }
    }

    QFile file(fileinfo.absoluteFilePath());
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }

    return {};
}

void ReplaceRefactorBase::setFileContent(const QFileInfo &fileinfo, const QByteArray &content) const
{
    Core::IDocument *doc =
            Core::DocumentModel::documentForFilePath(Utils::FilePath::fromString(fileinfo.absoluteFilePath()));
    if (doc) {
        auto textdoc = dynamic_cast<TextEditor::TextDocument *>(doc);
        const bool modified = textdoc->isModified();

        textdoc->setContents(content);

        if (!modified) {
            QString error;
            textdoc->save(&error, textdoc->filePath(), false);
        }
        return;
    }

    QFile file(fileinfo.absoluteFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(content);
    }
}

bool ReplaceRefactorBase::fileContainsText(const QFileInfo &fileInfo, const QList<QByteArray> &texts) const
{
    const QByteArray fileData = fileContent(fileInfo);
    for (const QByteArray &text : texts) {
        if (fileData.contains(text)) {
            return true;
        }
    }
    return false;
}

void ReplaceRefactorBase::updateFileText(
        const QFileInfo &fileInfo, const QList<QByteArray> &oldTexts, const QList<QByteArray> &newTexts) const
{
    Q_ASSERT(oldTexts.size() == newTexts.size());
    QByteArray fileData = fileContent(fileInfo);
    bool updated = false;
    for (int i = 0; i < oldTexts.size(); ++i) {
        if (fileData.contains(oldTexts[i])) {
            fileData.replace(oldTexts[i], newTexts[i]);
            updated = true;
        }
    }
    if (updated) {
        setFileContent(fileInfo, fileData);
    }
}

} // namespace spctr
