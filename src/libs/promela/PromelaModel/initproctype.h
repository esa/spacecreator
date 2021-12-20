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

#include "proctypeelement.h"
#include "sequence.h"

#include <QString>
#include <cstddef>
#include <list>
#include <memory>
#include <optional>

namespace promela::model {
/**
 * @brief Representation of init proctype in promela
 */
class InitProctype final
{
public:
    /**
     * @brief Constructor
     *
     * @param sequence sequence of init proctype
     */
    InitProctype(Sequence sequence);

    /**
     * @brief Getter for sequence of init proctype
     *
     * @return Sequence of init proctype
     */
    const Sequence &getSequence() const noexcept;
    /**
     * @brief Setter for sequence of init proctype
     *
     * @param sequence Sequence to set
     */
    void setSequence(Sequence sequence);

private:
    Sequence m_sequence;
};
}
