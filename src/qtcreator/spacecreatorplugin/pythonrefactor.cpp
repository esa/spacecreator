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

#include "commands/implementationshandler.h"
#include "errorhub.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "spacecreatorproject.h"

#include <QDir>
#include <QMessageBox>

namespace spctr {

const QString languageDir = "GUI";
const QString implementationFile = "/src/UserWidgets.py";

void PythonRefactor::setStorage(scs::SpaceCreatorProject *storage)
{
    m_storage = storage;
}

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
    return implementationDir(func, name).exists();
}

bool PythonRefactor::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    if (interface->function()->type() == ivm::IVObject::Type::Function) {
        auto func = static_cast<ivm::IVFunction *>(interface->function());
        return implementationDir(func, func->title()).exists();
    }
    return false;
}

void PythonRefactor::onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    const QString message =
            QObject::tr("Don't forget to update the Python implementation (%1 -> %2) in").arg(oldName, newName);
    const QString filePath = implementationDir(func, oldName).absolutePath() + implementationFile;

    shared::ErrorHub::addError(shared::ErrorItem::Warning, message, filePath);
    QMessageBox::information(nullptr, QObject::tr("Python refactor"), message + " " + filePath);
}

void PythonRefactor::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    if (interface->function()->type() == ivm::IVObject::Type::Function) {
        auto func = static_cast<ivm::IVFunction *>(interface->function());
        const QString funcName = func->title();
        const QString message = QObject::tr("Don't forget to update the Python implementation (%1.%2 -> %1.%3) in")
                                        .arg(funcName, oldName, newName);
        const QString filePath = implementationDir(func, func->title()).absolutePath() + implementationFile;

        shared::ErrorHub::addError(shared::ErrorItem::Warning, message, filePath);
        QMessageBox::information(nullptr, QObject::tr("Python refactor"), message + " " + filePath);
    }
}

QDir PythonRefactor::implementationDir(ivm::IVFunction *func, const QString &name) const
{
    const QDir dummyDir("/dummy/dir");
    if (func == nullptr) {
        return dummyDir;
    }

    ive::ImplementationsHandler implHandler(m_storage->projectPath(), func);

    for (const EntityAttribute &attribute : func->implementations()) {
        const QString language = attribute.value().toString();
        if (language.compare(languageDir, Qt::CaseInsensitive) == 0) {
            // There is a python configuration configured
            QDir implPath = implHandler.implementationPath(attribute.name(), language, name);
            if (implPath.exists()) {
                return implPath;
            }
            implPath = implHandler.functionBasePath(name) + QDir::separator() + languageDir;
            if (implPath.exists()) {
                return implPath;
            }
        }
    }
    return dummyDir;
}

} // namespace spctr
