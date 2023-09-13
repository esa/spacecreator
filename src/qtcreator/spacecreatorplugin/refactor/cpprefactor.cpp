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

QList<QByteArray> CppRefactor::interfaceCodeNames(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());
    QList<QByteArray> codes;

    codes << QString("%1::%2").arg(interface->function()->title(), interface->title()).toLower().toUtf8();

    const QString type = interface->isProvided() ? "_PI_" : "_RI_";
    codes << (interface->function()->title().toLower() + type + name.toLower()).toUtf8();

    return codes;
}

QStringList CppRefactor::implementationFileNames(const QString &functionName) const
{
    const QString fnNameLow(functionName.toLower());
    return { fnNameLow + ".cc", fnNameLow + ".h", fnNameLow + "_state.h" };
}

} // namespace spctr
