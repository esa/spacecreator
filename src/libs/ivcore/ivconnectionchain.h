/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QList>

namespace ivm {
class IVConnection;
class IVModel;

/*!
   Represents a chain of connections
   Calls can not only be done using one single connection,but is going through a whole chain of connections
 */
class IVConnectionChain
{
public:
    IVConnectionChain();

    static QList<IVConnectionChain *> build(const ivm::IVModel &model);
    static QList<IVConnectionChain *> build(
            IVConnection *connection, const QList<IVConnection *> &allConnections);

    const QList<IVConnection *> &connections() const;

    bool prepend(IVConnection *connection);
    bool append(IVConnection *connection);
    bool append(IVConnectionChain *chain);

    bool contains(IVConnection *connection) const;
    bool contains(const QString &connectionName, const QString &sourceName, const QString &targetName) const;

    QStringList connectionNames(const QString &sourceName, const QString &targetName) const;

    bool operator==(const IVConnectionChain &other) const;

private:
    static QList<IVConnectionChain *> findPrevious(
            IVConnection *connection, const QList<IVConnection *> &allConnections);
    static QList<IVConnectionChain *> findNext(
            IVConnection *connection, const QList<IVConnection *> &allConnections);

    QList<IVConnection *> m_chain;
};

}

QDebug operator<<(QDebug debug, const ivm::IVConnectionChain &chain);
