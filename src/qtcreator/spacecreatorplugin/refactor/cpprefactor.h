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

#include "replacerefactorbase.h"

namespace spctr {

/*!
 * \brief The CppRefactor class handles refactorings  affecting C++ implementations.
 */
class CppRefactor : public ReplaceRefactorBase
{
public:
    QString language() const override;

protected:
    virtual QString languageDir() const override;
    QString filename(const QString &funcName) const override;

    QList<QByteArray> interfaceCodeNames(ivm::IVInterface *interface, const QString &name) const override;
    QStringList implementationFileNames(const QString &functionName) const override;
};

} // namespace spctr
