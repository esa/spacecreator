/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "ivconnectionlayertype.h"

#include "ivobject.h"

#include <QDebug>
#include <QString>

namespace ivm {

const QString IVConnectionLayerType::DefaultLayerName = "default";

QVector<IVConnectionLayerType *> IVConnectionLayerType::connectionLayers;

IVConnectionLayerType::IVConnectionLayerType(const QString &name, QObject *parent, const shared::Id &id)
    : IVObject(IVObject::Type::ConnectionLayer, parent, id)
    , m_name(name)
{
}

void IVConnectionLayerType::rename(const QString &name)
{
    m_name = name;
}

IVConnectionLayerType::~IVConnectionLayerType()
{
    if (!IVConnectionLayerType::connectionLayers.isEmpty()) {
        IVConnectionLayerType::connectionLayers.removeAll(this);
    }
}

IVConnectionLayerType *IVConnectionLayerType::getConnectionLayerByName(const QString &name)
{
    for (auto layer : IVConnectionLayerType::connectionLayers) {
        if (layer != nullptr && layer->name().compare(name) == 0) {
            return layer;
        }
    }
    return nullptr;
}

IVConnectionLayerType *IVConnectionLayerType::renameConnectionLayer(const QString &oldName, const QString &newName)
{
    auto *layer = IVConnectionLayerType::getConnectionLayerByName(oldName);
    if (layer != nullptr) {
        layer->rename(newName);
    }
    return nullptr;
}

IVConnectionLayerType *IVConnectionLayerType::addDefaultConnectionLayer(IVObject *parent)
{
    if (getDefaultConnectionLayer() == nullptr) {
        IVConnectionLayerType::connectionLayers.append(
                new IVConnectionLayerType(IVConnectionLayerType::DefaultLayerName, parent, shared::createId()));
    }
    return getDefaultConnectionLayer();
}

IVConnectionLayerType *IVConnectionLayerType::getDefaultConnectionLayer()
{
    return IVConnectionLayerType::getConnectionLayerByName(IVConnectionLayerType::DefaultLayerName);
}

} // namespace ivm
