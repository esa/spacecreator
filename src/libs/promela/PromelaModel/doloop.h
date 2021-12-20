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

#include "sequence.h"

#include <list>
#include <memory>

namespace promela::model {
/**
 * @brief Representation of do loop in promela
 */
class DoLoop final
{
public:
    /**
     * @brief Getter for sequences of do loop
     *
     * @return list of sequences
     */
    const std::list<std::unique_ptr<Sequence>> &getSequences() const noexcept;

    /**
     * @brief Setter for sequences
     *
     * @param sequences list of sequences
     */
    void setSequences(std::list<std::unique_ptr<Sequence>> sequences);
    /**
     * @brief Add sequence to the do loop
     *
     * @param element sequence to add
     */
    void appendSequence(std::unique_ptr<Sequence> element);

private:
    std::list<std::unique_ptr<Sequence>> m_sequences;
};
}
