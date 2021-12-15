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

#include "basictypes.h"
#include "mtyperef.h"
#include "utyperef.h"

#include <QList>
#include <variant>

namespace promela::model {
/**
 * @brief Representation of channel initialization in promela.
 *
 * The channel initialization consists of a size of the channel,
 * and the list of possible datatypes for channel elements.
 * The channel in promela might contain only basic types or user defined structures;
 */
class ChannelInit final
{
public:
    /**
     * @brief The type allowed for channel, which can be only BasicType or reference to Utype or Mtype
     */
    using Type = std::variant<BasicType, UtypeRef, MtypeRef>;

    /**
     * @brief Constructor.
     *
     * @param size - the size of the channel
     * @param type - the type of element in the channel
     */
    ChannelInit(size_t size, const Type &type);

    /**
     * @brief Getter for size of the channel
     *
     * @return size of the channel.
     */
    size_t getSize() const noexcept;

    /**
     * @brief Adds another type to the channel
     *
     * @param type a type which will be added as an alternative type
     */
    void addType(const Type &type);

    /**
     * @brief Getter for types of channel.
     *
     * @return the list of all types of channel
     */
    const QList<Type> &getTypes() const noexcept;

private:
    size_t m_size;
    QList<Type> m_types;
};
}
