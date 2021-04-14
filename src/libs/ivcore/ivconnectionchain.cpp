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

#include "ivconnectionchain.h"

#include "ivconnection.h"
#include "ivinterface.h"
#include "ivmodel.h"

#include <QDebug>

namespace ivm {

IVConnectionChain::IVConnectionChain() { }

/*!
   Checks if \p chain is already part of the list
   The caller has to take over the ownership of the objects in the list
 */
bool containsChainAlready(IVConnectionChain *chain, const QList<IVConnectionChain *> &chainList)
{
    for (const IVConnectionChain *checkChain : chainList) {
        if (*chain == *checkChain) {
            return true;
        }
    }

    return false;
}

/*!
   Creates a list of all connection chains in that model
   The caller has to take over the ownership of the objects in the list
 */
QList<IVConnectionChain *> IVConnectionChain::build(const ivm::IVModel &model)
{
    QList<IVConnectionChain *> chains;
    QList<IVConnection *> allConnections = model.allObjectsByType<ivm::IVConnection>().toList();

    for (IVConnection *connection : allConnections) {
        QList<IVConnectionChain *> newChains = IVConnectionChain::build(connection, allConnections);
        for (auto c : newChains) {
            if (!containsChainAlready(c, chains)) {
                chains.append(c);
            }
        }
    }

    return chains;
}

/*!
   Creates a list of all connection chains in the list of given connection
 */
QList<IVConnectionChain *> IVConnectionChain::build(
        IVConnection *connection, const QList<IVConnection *> &allConnections)
{
    if (connection == nullptr) {
        return {};
    }

    QList<IVConnectionChain *> sourceChains = findPrevious(connection, allConnections);
    QList<IVConnectionChain *> targetChains = findNext(connection, allConnections);

    // Return chain with only this connection
    if (sourceChains.isEmpty() && targetChains.isEmpty()) {
        QList<IVConnectionChain *> chains;
        auto chain = new IVConnectionChain();
        chain->append(connection);
        chains.append(chain);
        return chains;
    }
    // This connection is the last of the chains
    if (targetChains.isEmpty()) {
        for (IVConnectionChain *chain : sourceChains) {
            chain->append(connection);
        }
        return sourceChains;
    }
    // This connection is the first of the chains
    if (sourceChains.isEmpty()) {
        for (IVConnectionChain *chain : targetChains) {
            chain->prepend(connection);
        }
        return targetChains;
    }

    // Connect all source and targets chains
    QList<IVConnectionChain *> chains;
    for (IVConnectionChain *schain : sourceChains) {
        for (IVConnectionChain *tchain : targetChains) {
            auto chain = new IVConnectionChain();
            chain->append(schain);
            chain->append(connection);
            chain->append(tchain);
            chains.append(chain);
        }
    }
    qDeleteAll(targetChains);
    qDeleteAll(sourceChains);
    return chains;
}

const QList<IVConnection *> &IVConnectionChain::connections() const
{
    return m_chain;
}

bool IVConnectionChain::prepend(IVConnection *connection)
{
    if (!connection) {
        return false;
    }

    if (!m_chain.isEmpty()) {
        if (m_chain.first()->sourceInterface() != connection->targetInterface()) {
            qDebug() << Q_FUNC_INFO << "connection" << *connection << "does not fit the chain start"
                     << *(m_chain.first());
            return false;
        }
    }

    m_chain.prepend(connection);
    return true;
}

bool IVConnectionChain::append(IVConnection *connection)
{
    if (!connection) {
        return false;
    }

    if (!m_chain.isEmpty()) {
        if (m_chain.last()->targetInterface() != connection->sourceInterface()) {
            qDebug() << Q_FUNC_INFO << "connection" << *connection << "does not fit the chain end"
                     << *(m_chain.first());
            return false;
        }
    }

    m_chain.append(connection);
    return true;
}

/**
   Appends a copy of \p chain
 */
bool IVConnectionChain::append(IVConnectionChain *chain)
{
    if (!chain) {
        return false;
    }

    if (!m_chain.isEmpty() && chain->connections().isEmpty()) {
        if (m_chain.last()->targetInterface() != chain->m_chain.first()->sourceInterface()) {
            qDebug() << Q_FUNC_INFO << "chain start" << *(chain->m_chain.first()) << "does not fit the chain end"
                     << *(m_chain.first());
            return false;
        }
    }

    m_chain.append(chain->m_chain);
    return true;
}

/*!
   Return if the iv \p connection is part of this chain
 */
bool IVConnectionChain::contains(IVConnection *connection) const
{
    return m_chain.contains(connection);
}

/*!
   Returns if the chain does connect source \p sourceName with target \p targetName with the connection \p
   connectionName.
   Or if a part of the chain does
 */
bool IVConnectionChain::contains(
        const QString &connectionName, const QString &sourceName, const QString &targetName) const
{
    if (sourceName.isEmpty() && targetName.isEmpty()) {
        return false;
    }

    const QString sName = sourceName.trimmed().toLower();
    const QString tName = targetName.trimmed().toLower();

    auto it = m_chain.begin();
    if (!sName.isEmpty()) {
        while (it != m_chain.end() && (*it)->sourceName().trimmed().toLower() != sName) {
            ++it;
        }
    }

    if (!tName.isEmpty()) {
        while (it != m_chain.end() && (*it)->targetName().trimmed().toLower() != tName) {
            ++it;
        }
    }

    if (!sName.isEmpty() && !tName.isEmpty()) {
        if (it != m_chain.end()) {
            return (*it)->name().trimmed().toLower() == connectionName.trimmed().toLower();
        }
    } else {
        // at least one name has to to match for a message from/to the environment
        it = m_chain.begin();
        while (it != m_chain.end()) {
            if ((*it)->name().trimmed().toLower() == connectionName.trimmed().toLower()) {
                return true;
            }
            ++it;
        }
    }

    return false;
}

/*!
   Returns the connection name for the given target (if it is part of this chain)
   If the the targetName is empty (from/to environment) all connection names after the source are returned.
 */
QStringList IVConnectionChain::connectionNames(const QString &sourceName, const QString &targetName) const
{
    if (sourceName.isEmpty() && targetName.isEmpty()) {
        return {};
    }

    const QString sName = sourceName.trimmed().toLower();
    const QString tName = targetName.trimmed().toLower();

    auto it = m_chain.begin();
    if (!sName.isEmpty()) {
        while (it != m_chain.end() && (*it)->sourceName().trimmed().toLower() != sName) {
            ++it;
        }
    }

    if (!tName.isEmpty()) {
        while (it != m_chain.end() && (*it)->targetName().trimmed().toLower() != tName) {
            ++it;
        }
        if (it != m_chain.end()) {
            return { (*it)->name() };
        }
    } else {
        QStringList result;
        while (it != m_chain.end()) {
            result << (*it)->name();
            ++it;
        }
        return result;
    }

    return {};
}

/*!
   Returns if two chains are equal
 */
bool IVConnectionChain::operator==(const IVConnectionChain &other) const
{
    return m_chain == other.m_chain;
}

/*!
   Returns all chains that end at the source interface of \p connection
 */
QList<IVConnectionChain *> IVConnectionChain::findPrevious(
        IVConnection *connection, const QList<IVConnection *> &allConnections)
{
    IVInterface *iface = connection->sourceInterface();
    if (iface == nullptr) {
        return {};
    }

    QList<IVConnectionChain *> chains;
    for (IVConnection *c : allConnections) {
        if (c->targetInterface() == iface) {
            QList<IVConnectionChain *> subChains = findPrevious(c, allConnections);
            if (subChains.isEmpty()) {
                auto chain = new IVConnectionChain();
                chain->append(c);
                chains.append(chain);
            } else {
                for (IVConnectionChain *subChain : subChains) {
                    subChain->append(c);
                    chains.append(subChain);
                }
            }
        }
    }

    return chains;
}

/*!
   Returns all chains that start at the target interface of \p connection
 */
QList<IVConnectionChain *> IVConnectionChain::findNext(
        IVConnection *connection, const QList<IVConnection *> &allConnections)
{
    IVInterface *iface = connection->targetInterface();
    if (iface == nullptr) {
        return {};
    }

    QList<IVConnectionChain *> chains;
    for (IVConnection *c : allConnections) {
        if (c->sourceInterface() == iface) {
            QList<IVConnectionChain *> subChains = findNext(c, allConnections);
            if (subChains.isEmpty()) {
                auto chain = new IVConnectionChain();
                chain->prepend(c);
                chains.append(chain);
            } else {
                for (IVConnectionChain *subChain : subChains) {
                    subChain->prepend(c);
                    chains.append(subChain);
                }
            }
        }
    }

    return chains;
}

}

/*!
   Debug output of a chain - used for debugging
 */
QDebug operator<<(QDebug debug, const ivm::IVConnectionChain &chain)
{
    QDebugStateSaver saver(debug);
    if (chain.connections().isEmpty()) {
        debug.nospace() << "<empty chain>";
        return debug;
    } else {
        QString out;
        out += QString("%1.%2").arg(
                chain.connections().first()->sourceName(), chain.connections().first()->sourceInterfaceName());
        for (ivm::IVConnection *c : chain.connections()) {
            out += QString("->%1.%2").arg(c->targetName(), c->targetInterfaceName());
        }
        debug.nospace() << out;
    }
    return debug;
}
