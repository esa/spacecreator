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
     * @param name    signal name
     */
    Input(QString name);

    /**
     * @brief   Constructor
     *
     * @param name        signal name
     * @param transition  a pointer to the transition which should be triggered upon reception of this signal
     */
    Input(QString name, const std::shared_ptr<Transition> &transition);

    /**
     * @brief   Copy constructor
     */
    Input(const Input &input);

    /**
     * @brief   Getter for the transition
     *
     * @return  a const reference to the pointer to transition which should be triggered upon reception of this signal
     */
    auto transition() const -> const std::shared_ptr<Transition> &;

    /**
     * @brief   Setter for the transition
     *
     * @param   transition a const reference to the pointer to transition which should be triggered upon reception of
     *                     this signal
     */
    auto setTransition(const std::shared_ptr<Transition> &transition) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    auto accept(Visitor &visitor) const -> void override;

private:
    std::shared_ptr<Transition> m_transition;

    // TODO: list of input parameters (variants: variable references/literals)
};

} // namespace sdl
