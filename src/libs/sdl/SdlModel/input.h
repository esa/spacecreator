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

#include "signal.h"
#include "transition.h"
#include "variableliteral.h"
#include "variablereference.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

/**
 * @brief   Represents an SDL input signal.
 *
 * Stores a pointer to the transition which should be fired upon reception of this signal.
 */
class Input final : public Signal
{
public:
    /**
     * @brief   Constructor
     *
     * @param name        signal name
     * @param transition  a pointer to the transition which should be triggered upon reception of this signal
     */
    Input(QString name, Transition *transition);

    /**
     * @brief   Default constructor
     */
    Input() = default;

    /**
     * @brief   Deleted copy constructor
     */
    Input(const Input &) = delete;

    /**
     * @brief   Default move constructor
     */
    Input(Input &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Input &operator=(const Input &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Input &operator=(Input &&) = default;

    /**
     * @brief   Getter for the transition
     *
     * @return  a pointer to transition which should be triggered upon reception of this signal
     */
    auto transition() const -> Transition *;

    /**
     * @brief   Setter for the transition
     *
     * @param   transition a pointer to transition which should be triggered upon reception of this signal
     */
    auto setTransition(Transition *transition) -> void;

    /**
     * @brief   Getter for the parameters
     *
     * @return  a reference to vector of pointers to input parameters
     */
    auto parameters() const -> const std::vector<std::unique_ptr<VariableReference>> &;

    /**
     * @brief   Setter for the transition
     *
     * @param   parameter  a pointer to input parameter
     */
    auto addParameter(std::unique_ptr<VariableReference> parameter) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    Transition *m_transition;

    std::vector<std::unique_ptr<VariableReference>> m_parameters;
};

} // namespace sdl
