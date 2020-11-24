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

#include "aadlconnectionchain.h"

#include "aadlobjectconnection.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"

#include <QDebug>

namespace aadl {

AADLConnectionChain::AADLConnectionChain() { }

/*!
   Checks if \p chain is already part of the list
   The caller has to take over the ownership of the objects in the list
 */
bool containsChainAlready(AADLConnectionChain *chain, const QList<AADLConnectionChain *> &chainList)
{
    for (const AADLConnectionChain *checkChain : chainList) {
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
QList<AADLConnectionChain *> AADLConnectionChain::build(const aadl::AADLObjectsModel &model)
{
    QList<AADLConnectionChain *> chains;
    QList<AADLObjectConnection *> allConnections = model.allObjectsByType<aadl::AADLObjectConnection>().toList();

    for (AADLObjectConnection *connection : allConnections) {
        QList<AADLConnectionChain *> newChains = AADLConnectionChain::build(connection, allConnections);
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
QList<AADLConnectionChain *> AADLConnectionChain::build(
        AADLObjectConnection *connection, const QList<AADLObjectConnection *> &allConnections)
{
    if (connection == nullptr) {
        return {};
    }

    QList<AADLConnectionChain *> sourceChains = findPrevious(connection, allConnections);
    QList<AADLConnectionChain *> targetChains = findNext(connection, allConnections);

    // Return chain with only this connection
    if (sourceChains.isEmpty() && targetChains.isEmpty()) {
        QList<AADLConnectionChain *> chains;
        auto chain = new AADLConnectionChain();
        chain->append(connection);
        chains.append(chain);
        return chains;
    }
    // This connection is the last of the chains
    if (targetChains.isEmpty()) {
        for (AADLConnectionChain *chain : sourceChains) {
            chain->append(connection);
        }
        return sourceChains;
    }
    // This connection is the first of the chains
    if (sourceChains.isEmpty()) {
        for (AADLConnectionChain *chain : targetChains) {
            chain->prepend(connection);
        }
        return targetChains;
    }

    // Connect all source and targets chains
    QList<AADLConnectionChain *> chains;
    for (AADLConnectionChain *schain : sourceChains) {
        for (AADLConnectionChain *tchain : targetChains) {
            auto chain = new AADLConnectionChain();
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

const QList<AADLObjectConnection *> &AADLConnectionChain::connections() const
{
    return m_chain;
}

bool AADLConnectionChain::prepend(AADLObjectConnection *connection)
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

bool AADLConnectionChain::append(AADLObjectConnection *connection)
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
bool AADLConnectionChain::append(AADLConnectionChain *chain)
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
   Return if the aadl \p connection is part of this chain
 */
bool AADLConnectionChain::contains(AADLObjectConnection *connection) const
{
    return m_chain.contains(connection);
}

/*!
   Returns if the chain does connect source \p sourceName with target \p targetName with the connection \p
   connectionName.
   Or if a part of the chain does
 */
bool AADLConnectionChain::contains(
        const QString &connectionName, const QString &sourceName, const QString &targetName) const
{
    auto it = m_chain.begin();
    while (it != m_chain.end() && (*it)->sourceName().trimmed().toLower() != sourceName.trimmed().toLower()) {
        ++it;
    }
    while (it != m_chain.end() && (*it)->targetName().trimmed().toLower() != targetName.trimmed().toLower()) {
        ++it;
    }

    if (it != m_chain.end()) {
        return (*it)->name().trimmed().toLower() == connectionName.trimmed().toLower();
    }

    return false;
}

/*!
   Returns if two chains are equal
 */
bool AADLConnectionChain::operator==(const AADLConnectionChain &other) const
{
    return m_chain == other.m_chain;
}

/*!
   Returns all chains that end at the source interface of \p connection
 */
QList<AADLConnectionChain *> AADLConnectionChain::findPrevious(
        AADLObjectConnection *connection, const QList<AADLObjectConnection *> &allConnections)
{
    AADLObjectIface *iface = connection->sourceInterface();
    if (iface == nullptr) {
        return {};
    }

    QList<AADLConnectionChain *> chains;
    for (AADLObjectConnection *c : allConnections) {
        if (c->targetInterface() == iface) {
            QList<AADLConnectionChain *> subChains = findPrevious(c, allConnections);
            if (subChains.isEmpty()) {
                auto chain = new AADLConnectionChain();
                chain->append(c);
                chains.append(chain);
            } else {
                for (AADLConnectionChain *subChain : subChains) {
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
QList<AADLConnectionChain *> AADLConnectionChain::findNext(
        AADLObjectConnection *connection, const QList<AADLObjectConnection *> &allConnections)
{
    AADLObjectIface *iface = connection->targetInterface();
    if (iface == nullptr) {
        return {};
    }

    QList<AADLConnectionChain *> chains;
    for (AADLObjectConnection *c : allConnections) {
        if (c->sourceInterface() == iface) {
            QList<AADLConnectionChain *> subChains = findNext(c, allConnections);
            if (subChains.isEmpty()) {
                auto chain = new AADLConnectionChain();
                chain->prepend(c);
                chains.append(chain);
            } else {
                for (AADLConnectionChain *subChain : subChains) {
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
QDebug operator<<(QDebug debug, const aadl::AADLConnectionChain &chain)
{
    QDebugStateSaver saver(debug);
    if (chain.connections().isEmpty()) {
        debug.nospace() << "<empty chain>";
        return debug;
    } else {
        QString out;
        out += QString("%1.%2").arg(
                chain.connections().first()->sourceName(), chain.connections().first()->sourceInterfaceName());
        for (aadl::AADLObjectConnection *c : chain.connections()) {
            out += QString("->%1.%2").arg(c->targetName(), c->targetInterfaceName());
        }
        debug.nospace() << out;
    }
    return debug;
}
