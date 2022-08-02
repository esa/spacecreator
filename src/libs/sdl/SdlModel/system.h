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

#include "block.h"
#include "node.h"

#include <QStringList>

namespace sdl {

/**
 * @brief   Represents an SDL system
 */
class System final : public Node
{
public:
    /**
     * @brief   Default constructor
     */
    System() = default;
    /**
     * @brief   Deleted copy constructor
     */
    System(const System &system) = delete;
    /**
     * @brief   Default move constructor
     */
    System(System &&system) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    System &operator=(const System &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    System &operator=(System &&) = default;

public:
    /**
     * @brief   Get list of freeform texts
     *
     * @return  Freeform texts
     */
    auto freeformTexts() const -> const QStringList &;
    /**
     * @brief   Add a freeform text
     *
     * @param   text    Text to add
     */
    auto addFreeformText(QString text) -> void;

    /**
     * @brief   Getter for the block
     *
     * @return  Block
     */
    auto block() const -> const Block &;
    /**
     * @brier   Setter for the block
     *
     * @param   block       Block
     */
    auto setBlock(Block block) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    QStringList m_freeformTexts;
    Block m_block;
};

} // namespace sdl
