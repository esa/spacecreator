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

#include "assignment.h"
#include "channelrecv.h"
#include "channelsend.h"
#include "conditional.h"
#include "declaration.h"
#include "doloop.h"
#include "exitloop.h"
#include "expression.h"
#include "forloop.h"
#include "inlinecall.h"
#include "sequence.h"
#include "skip.h"

#include <variant>

namespace promela::model {
/**
 * @brief Representation of proctype element i.e. step in promela
 */
class ProctypeElement final
{
public:
    /**
     * @brief Variant representation of different alternatives of proctype element
     */
    using Value = std::variant<Declaration, ChannelSend, ChannelRecv, Expression, DoLoop, Assignment, InlineCall, Skip,
            Conditional, Sequence, ForLoop, ExitLoop>;

    /**
     * @brief Constructor
     *
     * @param value value of proctype element
     */
    ProctypeElement(Value value);

    /**
     * @brief Getter for value of proctype element
     *
     * @return value of proctype element
     */
    const Value &getValue() const noexcept;

private:
    Value m_value;
};
}
