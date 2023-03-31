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

#include "creatorrefactorbase.h"

#include "commands/implementationshandler.h"
#include "errorhub.h"
#include "ivfunction.h"
#include "spacecreatorproject.h"

#include <QDir>
#include <QMessageBox>

namespace spctr {

void CreatorRefactorBase::setStorage(scs::SpaceCreatorProject *storage)
{
    m_storage = storage;
}

QDir CreatorRefactorBase::implementationDir(
        ivm::IVFunction *func, const QString &name, const QString &languageDir) const
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

void CreatorRefactorBase::reportWarning(const QString &message, const QString &filePath) const
{
    shared::ErrorHub::addError(shared::ErrorItem::Warning, message, filePath);
    QMessageBox::information(nullptr, QObject::tr("Python refactor"), message + " " + filePath);
}

} // namespace spctr
