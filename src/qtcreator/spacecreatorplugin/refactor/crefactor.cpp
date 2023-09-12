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

#include "crefactor.h"

#include "ivfunctiontype.h"
#include "ivinterface.h"

namespace spctr {

QString spctr::CRefactor::language() const
{
    return tr("C++");
}

QString spctr::CRefactor::languageDir() const
{
    return "C";
}

QString spctr::CRefactor::filename(const QString &funcName) const
{
    return QString("/src/%1.c").arg(funcName.toLower());
}

QByteArray CRefactor::interfaceCodeName(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());
    const QString type = interface->isProvided() ? "_PI_" : "_RI_";
    return (interface->function()->title().toLower() + type + name.toLower()).toUtf8();
}

QStringList CRefactor::implementationFileNames(ivm::IVFunctionType *function) const
{
    Q_ASSERT(function);

    return { function->title().toLower() + ".c", function->title().toLower() + ".cc",
        function->title().toLower() + ".h" };
}
}
