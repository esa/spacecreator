/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "systemchecks.h"

namespace msc {

SystemChecks::SystemChecks(QObject *parent)
    : QObject(parent)
{
}

SystemChecks::~SystemChecks() { }

QStringList SystemChecks::functionsNames() const
{
    return {};
}

QStringList SystemChecks::connectionNames() const
{
    return {};
}

bool SystemChecks::checkInstance(const MscInstance *instance) const
{
    Q_UNUSED(instance)
    return true;
}

bool SystemChecks::checkMessage(const MscMessage *message) const
{
    Q_UNUSED(message)
    return true;
}

QVector<QPair<MscChart *, MscInstance *>> SystemChecks::checkInstanceNames() const
{
    return {};
}

QVector<QPair<MscChart *, MscInstance *>> SystemChecks::checkInstanceRelations() const
{
    return {};
}

QVector<QPair<MscChart *, MscMessage *>> SystemChecks::checkMessages() const
{
    return {};
}

QStringList SystemChecks::connectionNamesFromTo(const QString &sourceName, const QString &targetName) const
{
    Q_UNUSED(sourceName)
    Q_UNUSED(targetName)
    return {};
}

bool SystemChecks::correspond(const ivm::IVObject *ivObj, const MscInstance *instance) const
{
    Q_UNUSED(ivObj)
    Q_UNUSED(instance)
    return true;
}

bool SystemChecks::correspond(const ivm::IVFunction *ivFunc, const MscInstance *instance) const
{
    Q_UNUSED(ivFunc)
    Q_UNUSED(instance)
    return true;
}

bool SystemChecks::correspond(const ivm::IVConnection *connection, const MscMessage *message) const
{
    Q_UNUSED(connection)
    Q_UNUSED(message)
    return true;
}

QVector<MscMessageDeclaration *> SystemChecks::allConnectionsAsDeclaration() const
{
    return {};
}

Qt::CaseSensitivity SystemChecks::stringSensitivity() const
{
    return Qt::CaseInsensitive;
}

bool SystemChecks::hasValidSystem() const
{
    return false;
}

} // namespace msc
