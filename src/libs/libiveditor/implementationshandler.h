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

#pragma once

#include <QPointer>
#include <QString>

namespace ivm {
class IVFunctionType;
}

namespace ive {

/**
 * Class to copy, move, link function implemenations (generated by external scripts)
 * It handles the implementation of one function
 *
 * Implementations are stored in: <project path>/work/<function name>/implem/<impl. name>/<language>
 * The default implementation is a link in <project path>/work/<function name>/ to the corresponding implementation
 */
class ImplementationsHandler
{
public:
    ImplementationsHandler(const QString &projectPath, ivm::IVFunctionType *entity);

    void checkOldImplementation();

    void renameImplementationDirectory(const QString &currentImplName, const QString &nextImplName);
    void updateDefaultImplementation(const QString &oldName, const QString &oldLanguage, const QString &newName,
            const QString &newLanguage, bool isDefault);

    QString functionBasePath(const QString &functionName = "") const;
    QString functionImplementaionsBasePath(const QString &functionName = "") const;
    QString implementationBasePath(const QString &name, const QString &functionName = "") const;
    QString implementationPath(const QString &name, const QString &language, const QString &functionName = "") const;

    /**
     * Returns the filename of the project file for the functions implementation. Does not contains the path.
     */
    static QString projectFilename(const QString &functionName);

    /**
     * Returns the directory name for the functions implementation. Does not contains the path.
     */
    static QString implementationDirectory(const QString &functionName);

private:
    bool moveDefaultDirectories(const QString &currentImplName, const QString &language);

    QString m_projectPath;
    QPointer<ivm::IVFunctionType> m_function;
};

}
