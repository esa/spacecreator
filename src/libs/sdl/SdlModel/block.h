/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "node.h"
#include "process.h"

namespace sdl {

/**
 * @brief   Represents an SDL system
 */
class Block final : public Node
{
public:
    /**
     * @brief   Default constructor
     */
    Block() = default;
    /**
     * @brief   Deleted copy constructor
     */
    Block(const Block &system) = delete;
    /**
     * @brief   Default move constructor
     */
    Block(Block &&system) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Block &operator=(const Block &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Block &operator=(Block &&) = default;

public:
    /**
     * @brief   Getter for the process
     *
     * @return  Process
     */
    auto process() const -> const Process &;
    /**
     * @brier   Setter for the process
     *
     * @param   process     Process
     */
    auto setProcess(Process process) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    Process m_process;
};

} // namespace sdl
