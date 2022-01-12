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

#include "variableref.h"

#include <QList>

namespace promela::model {
/**
 * @brief Representation of channel receive operation in promela
 */
class ChannelRecv
{
public:
    /**
     * @brief Constructor.
     *
     * TODO in future, it shall be possible to receive a constant - for condition
     *
     * @param channelRef variableRef, which shall be a reference to channel
     * @param args list of variable references, where received signal parameters shall be stored
     */
    ChannelRecv(VariableRef channelRef, const QList<VariableRef> &args);

    /**
     * @brief Getter for channel reference.
     *
     * @return reference to the channel
     */
    const VariableRef &getChannelRef() const noexcept;

    /**
     * @brief Getter for argument of channel receive operation.
     *
     * @return list of arguments for receive operation
     */
    const QList<VariableRef> &getArgs() const noexcept;

private:
    VariableRef m_channelRef;
    QList<VariableRef> m_args;
};
}
