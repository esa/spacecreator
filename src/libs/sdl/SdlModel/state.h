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

#include "input.h"
#include "node.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

class ContinuousSignal
{
    // TODO: move to a separate file
    // TODO: this class shall inherit from Transition
    // TODO: this class shall contain an Expression
};

class State final : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name              name of this state
     * @param   inputs            a reference to a vector of pointers to inputs (inputs will be moved after call to this
     *                            function)
     * @param   continuousSignals a reference to a vector of pointers to continuous signals (continuous signals will be
     *                            moved after call to this function)
     */
    State(QString name, std::vector<std::unique_ptr<Input>> inputs,
            std::vector<std::unique_ptr<ContinuousSignal>> continuousSignals);

    /**
     * @brief   Default constructor
     */
    State() = default;

    /**
     * @brief   Deleted copy constructor
     */
    State(const State &) = delete;

    /**
     * @brief   Default move constructor
     */
    State(State &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    State &operator=(const State &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    State &operator=(State &&) = default;

    /**
     * @brief   Getter for the inputs (input signals)
     *
     * @return  a const reference to a vector of pointers to input signals
     */
    auto inputs() const -> const std::vector<std::unique_ptr<Input>> &;

    /**
     * @brief   Add the input signal
     *
     * @param   inputs a pointer to input signal
     */
    auto addInput(std::unique_ptr<Input> input) -> void;

    /**
     * @brief   Getter for the continuous signals
     *
     * @return  a const reference to a vector of pointers to continuous signals
     */
    auto continuousSignals() const -> const std::vector<std::unique_ptr<ContinuousSignal>> &;

    /**
     * @brief   Add a continuous signal
     *
     * @param   continuousSignal a pointer to continuous signal
     */
    auto addContinuousSignal(std::unique_ptr<ContinuousSignal> continuousSignal) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::vector<std::unique_ptr<Input>> m_inputs;
    std::vector<std::unique_ptr<ContinuousSignal>> m_continuousSignals;
};

} // namespace sdl
