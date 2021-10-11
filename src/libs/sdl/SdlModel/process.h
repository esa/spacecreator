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
#include "statemachine.h"
#include "variabledeclaration.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

class Procedure
{
    // TODO: extract to a separate file

    /**
     * @brief   Default constructor
     */
    Procedure() = default;

    /**
     * @brief   Deleted move constructor
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
};

/**
 * @brief   Represents a next state (go to state) action in SDL model.
 *
 * Stores a pointer to the next state
 */
class Process final : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   stream   output stream (where the serialized values are put)
     */
    Process(QString name, std::unique_ptr<StateMachine> &stateMachine);

    /**
     * @brief   Default constructor
     */
    Process() = default;

    /**
     * @brief Deleted copy constructor
     */
    Process(const Process &) = delete;

    /**
     * @brief Default move constructor
     */
    Process(Process &&) = default;

    /**
     * @brief Deleted copy assignment
     */
    Process &operator=(const Process &) = delete;

    /**
     * @brief Deleted move assignment
     */
    Process &operator=(Process &&) = delete;

    /**
     * @brief   Getter for the state machine
     *
     * @return  a const reference to a pointer to the state machine
     */
    auto stateMachine() const -> const StateMachine *;

    /**
     * @brief   Setter for the state machine
     *
     * @param   stateMachine a pointer to the state machine
     */
    auto setStateMachine(std::unique_ptr<StateMachine> stateMachine) -> void;

    /**
     * @brief   Getter for the variables declared in this process
     *
     * @return  a const reference to a vector of pointers to variable declarations
     */
    auto variables() const -> const std::vector<std::unique_ptr<VariableDeclaration>> &;

    /**
     * @brief   Setter for the variables declared in this process
     *
     * @param   variables a vector of pointers to variable declarations
     */
    auto setVariables(std::vector<std::unique_ptr<VariableDeclaration>> variables) -> void;

    /**
     * @brief   Getter for the procedures declared in this process
     *
     * @return  a const reference to a vector of pointers to procedure
     */
    auto procedures() const -> const std::vector<std::unique_ptr<Procedure>> &;

    /**
     * @brief   Setter for the procedures declared in this process
     *
     * @param   procedures a vector of pointers to procedure
     */
    auto setProcedures(std::vector<std::unique_ptr<Procedure>> procedures) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::unique_ptr<StateMachine> m_stateMachine;
    std::vector<std::unique_ptr<VariableDeclaration>> m_variables;
    std::vector<std::unique_ptr<Procedure>> m_procedures;
};
} // namespace sdl
