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

#include "cpprefactor.h"

#include "ivfunction.h"
#include "ivinterface.h"

#include <QDir>

namespace spctr {

const QString languageDir = "CPP";

/*!
 * \see RefactorBase::isRefactorSupported
 */
bool CppRefactor::isRefactorSupported(RefactorType type) const
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

/*!
 * \see RefactorBase::isIVFunctionUsed
 */
bool CppRefactor::isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const
{
    return implementationDir(func, name, languageDir).exists();
}

/*!
 * \see RefactorBase::isIVInterfaceUsed
 */
bool CppRefactor::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    if (interface->function()->type() == ivm::IVObject::Type::Function) {
        auto func = static_cast<ivm::IVFunction *>(interface->function());
        return implementationDir(func, func->title(), languageDir).exists();
    }
    return false;
}

/*!
 * \see RefactorBase::onIVFunctionRenamed
 */
void CppRefactor::onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    const QString filePath = implementationDir(func, oldName, languageDir).absolutePath() + filename(oldName);
    if (!QFile::exists(filePath)) {
        return;
    }

    const QString message = tr("Don't forget to update the C++ implementation (%1 -> %2) in").arg(oldName, newName);

    reportWarning(message, filePath);
}

/*!
 * \see RefactorBase::onIVInterfaceRenamed
 */
void CppRefactor::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    if (interface->function()->type() == ivm::IVObject::Type::Function) {
        auto func = static_cast<ivm::IVFunction *>(interface->function());
        const QString funcName = func->title();
        const QString filePath =
                implementationDir(func, func->title(), languageDir).absolutePath() + filename(funcName);
        if (!QFile::exists(filePath)) {
            return;
        }

        const QString message =
                tr("Don't forget to update the C++ implementation (%1.%2 -> %1.%3) in").arg(funcName, oldName, newName);

        reportWarning(message, filePath);
    }
}

QString CppRefactor::filename(const QString &funcName) const
{
    return QString("/src/%1.cc").arg(funcName.toLower());
}

} // namespace spctr
