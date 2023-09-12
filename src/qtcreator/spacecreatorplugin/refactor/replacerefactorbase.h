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

#pragma once

#include "creatorrefactorbase.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QFileInfo>
#include <QPointer>

namespace ivm {
class IVFunctionType;
}
namespace spctr {

/**
 * @brief The ReplaceRefactorBase class
 * Performs the refactoring operations based on plain text search & replace
 */
class ReplaceRefactorBase : public CreatorRefactorBase
{
public:
    bool isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const override;

    void onIVInterfaceRenamed(
            ivm::IVInterface *interface, const QString &oldName, const QString &newName) const override;

protected:
    /**
     * Returns the interface name as used in the implementation code
     * @param interface The interface
     * @param name the name of the interface. That is used, as the name stored in @p interface might be changed already
     */
    virtual QByteArray interfaceCodeName(ivm::IVInterface *interface, const QString &name) const = 0;

    /**
     * Returns a list of plain file names (without path) that are used by the generator. Those file might exist or not.
     */
    virtual QStringList implementationFileNames(ivm::IVFunctionType *function) const = 0;

    /**
     * Returns the list of existing files of the implementation including their full path
     */
    virtual QStringList implementationFilePaths(ivm::IVFunctionType *function) const;

    /**
     * Returns the content of the given file.
     * If the file is open in QtCreator, the data is read from the editor's BaseTextDocument
     * If the file is not open, it's written to the disk
     */
    virtual QByteArray fileContent(const QFileInfo &fileinfo) const;
    /**
     * Stores the data to the given file
     * If the file is open in QtCreator, the data is written to the editor's BaseTextDocument
     * If the file is not open, it's written to the disk
     */
    virtual void setFileContent(const QFileInfo &fileinfo, const QByteArray &content) const;
};

} // namespace spctr
