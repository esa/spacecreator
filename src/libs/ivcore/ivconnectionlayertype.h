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

/**
 * @brief The IVConnectionLayerType is a class to represent view layer
 * for interfaces and connections as IVObject
 */
class IVConnectionLayerType : public IVObject
{
    Q_OBJECT

public:
    /**
    * @brief The IVConnectionLayerType constructor without name set
    * @param parent                  parent of IVConnectionLayerType object
    * @param id                      id of IVConnectionLayerType object
    */
    IVConnectionLayerType(QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);

    /**
    * @brief The IVConnectionLayerType destructor
    */
    ~IVConnectionLayerType();

    /**
    * @brief Rename layer
    * @param name                    new name
    */
    auto rename(const QString &name) -> void;

    /**
    * @brief Constant for default layer name
    */
    static const QString DefaultLayerName;
};

} // namespace ivm
