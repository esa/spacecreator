/*
  Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "sdlrefactor.h"

#include "ivfunctiontype.h"
#include "ivinterface.h"

namespace spctr {

QString SdlRefactor::language() const
{
    return tr("SDL");
}
QString SdlRefactor::languageDir() const
{
    return "SDL";
}
QString SdlRefactor::filename(const QString &funcName) const
{
    return QString("/src/%1.pr").arg(funcName.toLower());
}

QList<QByteArray> SdlRefactor::interfaceCodeNames(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface && interface->function());
    QList<QByteArray> codes;
    const QString type = interface->isProvided() ? "input" : "output";
    codes << QString("%1 %2(").arg(type, name).toUtf8(); // used in "name".pr file
    codes << QString("signal %1 (").arg(name).toUtf8(); // used in systen_structure.pr
    codes << QString(" %1, ").arg(name).toUtf8(); // used in systen_structure.pr
    codes << QString(" %1;").arg(name).toUtf8(); // used in systen_structure.pr
    return codes;
}

QStringList SdlRefactor::implementationFileNames(const QString &functionName) const
{
    const QString fnNameLow(functionName.toLower());
    return { fnNameLow + ".pr", "system_structure.pr" };
}

QList<QByteArray> SdlRefactor::functionsTextsToReplace(const QString &functionName) const
{
    QString tmpFnName = functionName.toLower();
    tmpFnName[0] = tmpFnName[0].toUpper();
    const QByteArray fnName = tmpFnName.toUtf8();

    QList<QByteArray> texts { " " + fnName + ";", " " + fnName + " " };

    return texts;
}

} // namespace spctr
