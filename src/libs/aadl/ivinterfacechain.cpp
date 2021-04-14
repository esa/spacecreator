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

#include "ivinterfacechain.h"

#include "ivconnection.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"

#include <QSet>

namespace ivm {

QList<IVInterfaceChain> IVInterfaceChain::getNextChunk(
        const IVInterfaceChain &currentChain, const IVInterface *iface)
{
    QList<IVInterfaceChain> chains;
    for (auto connection : iface->model()->getConnectionsForIface(iface->id())) {
        IVInterfaceChain chain(currentChain);
        if (connection->targetInterface()->id() == iface->id()) {
            continue;
        }
        chain.ifaces.append(connection->targetInterface());
        chains << getNextChunk(chain, connection->targetInterface());
    }

    return chains.isEmpty() ? QList<IVInterfaceChain> { currentChain } : chains;
}

QList<IVInterfaceChain> IVInterfaceChain::getPreviousChunk(
        const IVInterfaceChain &currentChain, const IVInterface *iface)
{
    QList<IVInterfaceChain> chains;
    for (auto connection : iface->model()->getConnectionsForIface(iface->id())) {
        IVInterfaceChain chain(currentChain);
        if (connection->sourceInterface()->id() == iface->id()) {
            continue;
        }
        chain.ifaces.prepend(connection->sourceInterface());
        chains << getPreviousChunk(chain, connection->sourceInterface());
    }

    return chains.isEmpty() ? QList<IVInterfaceChain> { currentChain } : chains;
}

QList<IVInterfaceChain> IVInterfaceChain::build(const IVInterface *iface)
{
    if (!iface) {
        return {};
    }

    QList<IVInterfaceChain> result;
    const QList<IVInterfaceChain> prevItems = getPreviousChunk({}, iface);
    const QList<IVInterfaceChain> nextItems = getNextChunk({}, iface);
    for (auto prevChain : prevItems) {
        for (auto nextChain : nextItems) {
            IVInterfaceChain chain;
            std::copy(prevChain.ifaces.cbegin(), prevChain.ifaces.cend(), std::back_inserter(chain.ifaces));
            chain.ifaces.append(iface);
            std::copy(nextChain.ifaces.cbegin(), nextChain.ifaces.cend(), std::back_inserter(chain.ifaces));
            result << chain;
        }
    }
    return result;
}

QVector<QList<QString>> IVInterfaceChain::linkedFunctions(const IVFunctionType *function)
{
    QVector<QList<QString>> result;
    for (IVInterface *iface : function->interfaces()) {
        const QList<IVInterfaceChain> chains = build(iface);
        for (auto chain : chains) {
            if (!chain.ifaces.isEmpty()) {
                result.append(chain.ifaces.front()->function()->path());
                result.append(chain.ifaces.back()->function()->path());
            }
        }
    }

    for (auto childFn : function->functions()) {
        result.append(childFn->path());
    }
    for (auto childFn : function->functionTypes()) {
        result.append(childFn->path());
    }
    result.removeAll(function->path());
    std::sort(result.begin(), result.end());
    const auto it = std::unique(result.begin(), result.end());
    result.resize(std::distance(result.begin(), it));
    return result;
}

QString IVInterfaceChain::sourceEndPointName() const
{
    if (ifaces.isEmpty()) {
        return {};
    }
    if (auto iface = ifaces.front()) {
        return iface->title();
    }
    return {};
}

QString IVInterfaceChain::targetEndPointName() const
{
    if (ifaces.isEmpty()) {
        return {};
    }
    if (auto iface = ifaces.back()) {
        return iface->title();
    }
    return {};
}

QStringList IVInterfaceChain::sourceEndPointPath() const
{
    if (ifaces.isEmpty()) {
        return {};
    }
    if (auto iface = ifaces.front()) {
        return iface->path();
    }
    return {};
}

QStringList IVInterfaceChain::targetEndPointPath() const
{
    if (ifaces.isEmpty()) {
        return {};
    }
    if (auto iface = ifaces.back()) {
        return iface->path();
    }
    return {};
}

} // namespace ivm
