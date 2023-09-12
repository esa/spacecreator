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

#include "commands/implementationshandler.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "spacecreatorproject.h"

#include <QDir>
#include <QDirIterator>
#include <editormanager/documentmodel.h>
#include <texteditor/textdocument.h>

namespace spctr {

bool ReplaceRefactorBase::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());

    QStringList implFiles = implementationFilePaths(interface->function());
    for (const QString &implFile : implFiles) {
        QFileInfo fi(implFile);
        if (fileContent(fi).contains(interfaceCodeName(interface, name))) {
            return true;
        }
    }

    return false;
}

void ReplaceRefactorBase::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    QStringList implFiles = implementationFilePaths(interface->function());
    const QByteArray oldCode = interfaceCodeName(interface, oldName);
    const QByteArray newCode = interfaceCodeName(interface, newName);

    for (const QString &implFile : implFiles) {
        QFileInfo fi(implFile);
        QByteArray fileData = fileContent(fi);
        if (fileData.contains(oldCode)) {
            fileData.replace(oldCode, newCode);
            setFileContent(fi, fileData);
        }
    }
}

QStringList ReplaceRefactorBase::implementationFilePaths(ivm::IVFunctionType *function) const
{
    QStringList implFiles;

    QStringList implementationFiles = implementationFileNames(function);
    QDirIterator it(m_storage->projectPath() + "/" + ive::kRootImplementationPath, implementationFiles, QDir::Files,
            QDirIterator::Subdirectories);
    while (it.hasNext()) {
        implFiles.append(it.next());
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
    file.open(QIODevice::ReadOnly);
    if (file.isOpen()) {
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
    file.open(QIODevice::WriteOnly);
    if (file.isOpen()) {
        file.write(content);
    }
}

} // namespace spctr
