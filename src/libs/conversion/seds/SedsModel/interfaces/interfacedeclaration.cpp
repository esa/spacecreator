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

#include "interfaces/interfacedeclaration.h"

namespace seds::model {

InterfaceDeclaration::InterfaceDeclaration() noexcept
    : m_abstract(false)
    , m_level(InterfaceLevel::APPLICATION)
{
}

bool
InterfaceDeclaration::isAbstract() const
{
    return m_abstract;
}

void
InterfaceDeclaration::setAbstract(bool abstract)
{
    m_abstract = abstract;
}

InterfaceLevel
InterfaceDeclaration::level() const
{
    return m_level;
}

void
InterfaceDeclaration::setLevel(InterfaceLevel level)
{
    m_level = level;
}

const InterfaceDeclaration::BaseInterfaceSet&
InterfaceDeclaration::baseInterfaces() const
{
    return m_baseInterfaces;
}

void
InterfaceDeclaration::addBaseInterface(InterfaceRef baseInterface)
{
    m_baseInterfaces.push_back(std::move(baseInterface));
}

const InterfaceDeclaration::GenericTypeSet&
InterfaceDeclaration::genericTypes() const
{
    return m_genericTypes;
}

void
InterfaceDeclaration::addGenericType(GenericType genericType)
{
    m_genericTypes.push_back(std::move(genericType));
}

const InterfaceDeclaration::ParameterSet&
InterfaceDeclaration::parameters() const
{
    return m_parameters;
}

void
InterfaceDeclaration::addParameter(InterfaceParameter parameter)
{
    m_parameters.push_back(std::move(parameter));
}

const InterfaceDeclaration::CommandSet&
InterfaceDeclaration::commands() const
{
    return m_commands;
}

void
InterfaceDeclaration::addCommand(InterfaceCommand command)
{
    m_commands.push_back(std::move(command));
}

} // namespace seds::model
