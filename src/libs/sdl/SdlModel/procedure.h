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

#include "node.h"
#include "transition.h"

#include <QString>
#include <memory>

namespace sdl {

/**
 * @brief   Represents an SDL procedure
 */
class Procedure : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name   name of the element
     */
    Procedure(QString name = "");

    /**
     * @brief   Deleted copy constructor
     */
    Procedure(const Procedure &) = delete;

    /**
     * @brief   Default move constructor
     */
    Procedure(Procedure &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Procedure &operator=(const Procedure &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Procedure &operator=(Procedure &&) = default;

    /**
     * @brief   Getter for the transition
     *
     * @return  a pointer to transition which should be triggered when this procedure is called
     */
    auto transition() const -> Transition *;

    /**
     * @brief   Setter for the transition
     *
     * @param   transition a pointer to transition which should be triggered when this procedure is called
     */
    auto setTransition(std::unique_ptr<Transition> transition) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::unique_ptr<Transition> m_implementation;
};

} // namespace sdl
