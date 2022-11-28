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

#include "newtype.h"
#include "node.h"
#include "procedure.h"
#include "statemachine.h"
#include "syntype.h"
#include "variabledeclaration.h"

#include <QString>
#include <QStringList>
#include <memory>
#include <vector>

namespace sdl {

/**
 * @brief   Represents an SDL process
 */
class Process final : public Node
{
public:
    /**
     * @brief   Default constructor
     */
    Process() = default;
    /**
     * @brief   Constructor
     *
     * @param   name           name of the process
     * @param   stateMachine   state machine of the process
     */
    Process(QString name, std::unique_ptr<StateMachine> stateMachine);
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
     * @brief Default move assignment
     */
    Process &operator=(Process &&) = default;

public:
    /**
     * @brief Getter for START transition
     *
     * @return START transition pointer
     */
    auto startTransition() const -> Transition *;
    /**
     * @brief Setter for START transition
     *
     * @param transition  START transition
     */
    auto setStartTransition(std::unique_ptr<Transition> transition) -> void;

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
     * @brief   Add a variable declaration
     *
     * @param   variable a pointer to variable declaration
     */
    auto addVariable(std::unique_ptr<VariableDeclaration> variable) -> void;

    /**
     * @brief   Getter for the syntypes declared in this process
     *
     * @return  a const reference to a vector of syntypes declarations
     */
    auto syntypes() const -> const std::vector<Syntype> &;
    /**
     * @brief   Add a syntype declaration
     *
     * @param   syntype a syntype declaration
     */
    auto addSyntype(Syntype syntype) -> void;

    /**
     * @brief   Getter for the newtypes declared in this process
     *
     * @return  a const reference to a vector of newtypes declarations
     */
    auto newtypes() const -> const std::vector<Newtype> &;
    /**
     * @brief   Add a newtype declaration
     *
     * @param   newtype a newtype declaration
     */
    auto addNewtype(Newtype newtype) -> void;

    /**
     * @brief   Getter for the procedures declared in this process
     *
     * @return  a const reference to a vector of pointers to procedure
     */
    auto procedures() const -> const std::vector<std::unique_ptr<Procedure>> &;
    /**
     * @brief   Add a procedure declared in this process
     *
     * @param   procedure procedure to be added
     */
    auto addProcedure(std::unique_ptr<Procedure> procedure) -> void;

    /**
     * @brief   Timer names
     */
    auto timerNames() const -> const std::vector<QString> &;
    /**
     * @brief   Add timer
     *
     * @param   timerName Timer name
     */
    auto addTimer(QString timerName) -> void;

    /**
     * @brief   Getter for success states names
     *
     * @return  Success states names
     */
    auto successStates() const -> const QStringList &;
    /**
     * @brief   Adds an success state name
     *
     * @param   stateName   Success state name
     */
    auto addSuccessState(QString stateName) -> void;
    /**
     * @brief   Getter for error states names
     *
     * @return  Error states names
     */
    auto errorStates() const -> const QStringList &;
    /**
     * @brief   Adds an error state name
     *
     * @param   stateName   Error state name
     */
    auto addErrorState(QString stateName) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::unique_ptr<Transition> m_startTransition;
    std::unique_ptr<StateMachine> m_stateMachine;
    std::vector<std::unique_ptr<VariableDeclaration>> m_variables;
    std::vector<Syntype> m_syntypes;
    std::vector<Newtype> m_newtypes;
    std::vector<std::unique_ptr<Procedure>> m_procedures;
    std::vector<QString> m_timerNames;
    QStringList m_successStates;
    QStringList m_errorStates;
};
} // namespace sdl
