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

#include "ivfunctiontype.h"
#include "ivinterface.h"

namespace spctr {

QString CppRefactor::language() const
{
    return tr("C++");
}

QString CppRefactor::languageDir() const
{
    return "CPP";
}

QString CppRefactor::filename(const QString &funcName) const
{
    return QString("/src/%1.cc").arg(funcName.toLower());
}

QByteArray CppRefactor::interfaceCodeName(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());
    return QString("%1::%2").arg(interface->function()->title(), interface->title()).toLower().toUtf8();
}

QStringList CppRefactor::implementationFileNames(ivm::IVFunctionType *function) const
{
    Q_ASSERT(function);
    const QString fnNameLow(function->title().toLower());
    return { fnNameLow + ".cc", fnNameLow + ".h" };
}

} // namespace spctr
