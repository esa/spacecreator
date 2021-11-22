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

#pragma once

#include "ivobject.h"

#include <QList>
#include <QString>

namespace ivm {

class IVConnectionLayerType : public IVObject
{
    Q_OBJECT

public:
    IVConnectionLayerType(const QString &name, QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    ~IVConnectionLayerType();
    inline QString name() const { return m_name; };
    auto rename(const QString &name) -> void;
    auto moveObjectToLayer(IVObject *obj) -> bool;
    auto removeObjectFromLayer(IVObject *obj) -> bool;
    auto getObjectsForLayer() -> QList<IVObject *>;

    static const QString DefaultLayerName;
    static QVector<IVConnectionLayerType *> connectionLayers;

    static auto getConnectionLayerByName(const QString &name) -> IVConnectionLayerType *;
    static auto renameConnectionLayer(const QString &oldName, const QString &newName) -> IVConnectionLayerType *;

    static auto addConnectionLayer(IVConnectionLayerType *layer) -> IVConnectionLayerType *;
    static auto addConnectionLayer(const QString &name, IVObject *parent) -> IVConnectionLayerType *;

    static auto removeConnectionLayer(const QString &name) -> bool;

    static auto addDefaultConnectionLayer(IVObject *parent) -> IVConnectionLayerType *;
    static auto getDefaultConnectionLayer() -> IVConnectionLayerType *;

private:
    QString m_name;
    QList<IVObject *> m_objects;
};

} // namespace ivm
