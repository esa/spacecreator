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
#include "state.h"

#include <QString>
#include <memory>

namespace sdl {

/**
 * @brief   Represents a next state (go to state) action in SDL model.
 *
 * Stores a pointer to the next state
 */
class NextState final : public Action
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name    name of the action
     */
    NextState(QString name);

    /**
     * @brief   Deleted copy constructor
     */
    NextState(const NextState &) = delete;

    /**
     * @brief   Default move constructor
     */
    NextState(NextState &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    NextState &operator=(const NextState &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    NextState &operator=(NextState &&) = default;

    /**
     * @brief   Constructor
     *
     * @param   name    name of the action
     * @param   state   a pointer to the next state
     */
    NextState(QString name, const std::shared_ptr<State> &state);

    /**
     * @brief   Getter for the next state
     *
     * @return  next state
     */
    auto state() const -> const std::shared_ptr<State> &;

    /**
     * @brief   Setter for the next state
     *
     * @param   state next state
     */
    auto setState(const std::shared_ptr<State> &state) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::shared_ptr<State> m_state;
};

} // namespace sdl
