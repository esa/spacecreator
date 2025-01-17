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

#include "process.h"

#include <memory>
#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Process::Process(QString name, std::unique_ptr<StateMachine> stateMachine)
    : Node(std::move(name))
    , m_stateMachine(std::move(stateMachine))
{
}

Transition *Process::startTransition() const
{
    return m_startTransition.get();
}

void Process::setStartTransition(std::unique_ptr<Transition> transition)
{
    m_startTransition = std::move(transition);
}

const StateMachine *Process::stateMachine() const
{
    return m_stateMachine.get();
}

void Process::setStateMachine(std::unique_ptr<StateMachine> stateMachine)
{
    m_stateMachine = std::move(stateMachine);
}

const std::vector<std::unique_ptr<VariableDeclaration>> &Process::variables() const
{
    return m_variables;
}

void Process::setVariables(std::vector<std::unique_ptr<VariableDeclaration>> variables)
{
    m_variables = std::move(variables);
}

void Process::addVariable(std::unique_ptr<VariableDeclaration> variable)
{
    m_variables.push_back(std::move(variable));
}

const std::vector<Syntype> &Process::syntypes() const
{
    return m_syntypes;
}

void Process::addSyntype(Syntype syntype)
{
    m_syntypes.push_back(std::move(syntype));
}

const std::vector<Newtype> &Process::newtypes() const
{
    return m_newtypes;
}

void Process::addNewtype(Newtype newtype)
{
    m_newtypes.push_back(std::move(newtype));
}

const std::vector<std::unique_ptr<Procedure>> &Process::procedures() const
{
    return m_procedures;
}

void Process::addProcedure(std::unique_ptr<Procedure> procedure)
{
    m_procedures.push_back(std::move(procedure));
}

const std::vector<QString> &Process::timerNames() const
{
    return m_timerNames;
}

void Process::addTimer(QString timerName)
{
    m_timerNames.push_back(std::move(timerName));
}

const QStringList &Process::successStates() const
{
    return m_successStates;
}

void Process::addSuccessState(QString stateName)
{
    m_successStates.push_back(std::move(stateName));
}

const QStringList &Process::errorStates() const
{
    return m_errorStates;
}

void Process::addErrorState(QString stateName)
{
    m_errorStates.push_back(std::move(stateName));
}

void Process::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
