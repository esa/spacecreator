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

#include "action.h"
#include "label.h"

#include <QString>

namespace sdl {

/**
 * @brief   Represents a join action in SDL model.
 *
 */
class Join final : public Action
{
public:
    /**
     * @brief   Constructor
     */
    Join() = default;

    /**
     * @brief   Deleted copy constructor
     */
    Join(const Join &) = delete;

    /**
     * @brief   Default move constructor
     */
    Join(Join &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Join &operator=(const Join &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Join &operator=(Join &&) = default;

    /**
     * @brief   Getter for label
     *
     * @return  label
     */
    auto label() const -> Label *;

    /**
     * @brief   Setter for label
     *
     * @param   label  label
     */
    auto setLabel(Label *label) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    Label *m_label;
};

} // namespace sdl
