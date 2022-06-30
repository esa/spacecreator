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

#include "expression.h"
#include "variableref.h"

#include <QList>

namespace promela::model {
/**
 * @brief Representation of channel send operation in promela
 */
class ChannelSend
{
public:
    /**
     * @brief Constructor.
     *
     * TODO in future, it shall be possible to send an expression
     *
     * @param channelRef variableRef, which shall be a reference to channel
     * @param args list of expression, which are parameters of the signal
     */
    ChannelSend(VariableRef channelRef, const QList<Expression> &args);

    /**
     * @brief Getter for channel reference.
     *
     * @return reference to the channel
     */
    const VariableRef &getChannelRef() const noexcept;

    /**
     * @brief Getter for argument of channel send operation.
     *
     * @return list of arguments for send operation
     */
    const QList<Expression> &getArgs() const noexcept;

private:
    VariableRef m_channelRef;
    QList<Expression> m_args;
};
}
