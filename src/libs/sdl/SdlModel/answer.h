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

#include "transition.h"
#include "variableliteral.h"

#include <QString>
#include <memory>

namespace sdl {

/**
 * @brief   Represents an answer used in SDL model.
 */
class Answer final : public Node
{
public:
    /**
     * @brief   Constructor
     */
    Answer();

    /**
     * @brief   Deleted copy constructor
     */
    Answer(const Answer &) = delete;

    /**
     * @brief   Default move constructor
     */
    Answer(Answer &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Answer &operator=(const Answer &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Answer &operator=(Answer &&) = default;

    /**
     * @brief   Getter for the stored transition
     *
     * @return  transition
     */
    auto transition() const -> Transition *;

    /**
     * @brief   Setter for the stored value
     *
     * @param   transition  transition to store
     */
    auto setTransition(std::unique_ptr<Transition> Transition) -> void;

    /**
     * @brief   Getter for answer literal
     *
     * @return  variable literal
     */
    auto literal() const -> const VariableLiteral *;

    /**
     * @brief   Setter for answer literal
     *
     * @param   literal  literal to store
     */
    auto setLiteral(VariableLiteral literal) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::unique_ptr<Transition> m_transition;
    VariableLiteral m_literal;
};

} // namespace sdl
