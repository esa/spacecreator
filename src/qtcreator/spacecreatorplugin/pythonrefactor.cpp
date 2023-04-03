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

#include "pythonrefactor.h"

#include "ivfunction.h"
#include "ivinterface.h"

#include <QDir>

namespace spctr {

const QString languageDir = "GUI";
const QString implementationFile = "/src/UserWidgets.py";

/*!
 * \see RefactorBase::isRefactorSupported
 */
bool PythonRefactor::isRefactorSupported(RefactorType type) const
{
    switch (type) {
    case scs::RefactorBase::IVFunctionRename:
    case scs::RefactorBase::IVInterfaceRename:
        return true;
    default:
        break;
    }

    return false;
}

bool PythonRefactor::isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const
{
    return implementationDir(func, name, languageDir).exists();
}

bool PythonRefactor::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    if (interface->function()->type() == ivm::IVObject::Type::Function) {
        auto func = static_cast<ivm::IVFunction *>(interface->function());
        return implementationDir(func, func->title(), languageDir).exists();
    }
    return false;
}

void PythonRefactor::onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    const QString filePath = implementationDir(func, oldName, languageDir).absolutePath() + implementationFile;
    if (!QFile::exists(filePath)) {
        return;
    }
    const QString message = tr("Don't forget to update the Python implementation (%1 -> %2) in").arg(oldName, newName);

    reportWarning(message, filePath);
}

void PythonRefactor::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    if (interface->function()->type() == ivm::IVObject::Type::Function) {
        auto func = static_cast<ivm::IVFunction *>(interface->function());
        const QString filePath =
                implementationDir(func, func->title(), languageDir).absolutePath() + implementationFile;
        if (!QFile::exists(filePath)) {
            return;
        }

        const QString funcName = func->title();
        const QString message = tr("Don't forget to update the Python implementation (%1.%2 -> %1.%3) in")
                                        .arg(funcName, oldName, newName);

        reportWarning(message, filePath);
    }
}


} // namespace spctr
