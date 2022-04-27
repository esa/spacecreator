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

#include "procedure.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Procedure::Procedure(QString name)
    : Node(std::move(name))
{
}

Transition *Procedure::transition() const
{
    return m_implementation.get();
}

void Procedure::setTransition(std::unique_ptr<Transition> transition)
{
    m_implementation = std::move(transition);
}

const std::vector<std::unique_ptr<VariableDeclaration>> &Procedure::variables() const
{
    return m_variables;
}

void Procedure::addVariable(std::unique_ptr<VariableDeclaration> variable)
{
    m_variables.push_back(std::move(variable));
}

const std::vector<std::unique_ptr<ProcedureParameter>> &Procedure::parameters() const
{
    return m_parameters;
}

void Procedure::addParameter(std::unique_ptr<ProcedureParameter> parameter)
{
    m_parameters.push_back(std::move(parameter));
}

const QString &Procedure::returnType() const
{
    return m_returnType;
}

void Procedure::setReturnType(QString returnType)
{
    m_returnType = std::move(returnType);
}

void Procedure::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
