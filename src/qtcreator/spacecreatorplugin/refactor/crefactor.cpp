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
    return tr("C");
}

QString spctr::CRefactor::languageDir() const
{
    return "C";
}

QString spctr::CRefactor::filename(const QString &funcName) const
{
    return QString("/src/%1.c").arg(funcName.toLower());
}

QList<QByteArray> CRefactor::interfaceCodeNames(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());
    QList<QByteArray> codes;
    const QString type = interface->isProvided() ? "_PI_" : "_RI_";
    codes << (interface->function()->title().toLower() + type + name.toLower()).toUtf8();
    return codes;
}

QStringList CRefactor::implementationFileNames(const QString &functionName) const
{
    const QString fnNameLow(functionName.toLower());
    return { fnNameLow + ".c", fnNameLow + ".cc", fnNameLow + ".h", fnNameLow + "_state.h" };
}

QList<QByteArray> CRefactor::functionsTextsToReplace(const QString &functionName) const
{
    const QByteArray fnNameLow = functionName.toLower().toUtf8();

    QList<QByteArray> texts { fnNameLow + ".h", fnNameLow + "_state", "void " + fnNameLow + "_", "ctxt_" + fnNameLow,
        functionName.toUtf8() };

    return texts;
}
}
