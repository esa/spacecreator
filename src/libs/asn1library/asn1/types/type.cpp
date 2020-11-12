/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "type.h"

using namespace Asn1Acn::Types;

Type::Type(const QString &identifier)
    : m_identifier(identifier)
{
}

/*!
   Returns the identifier/name of this type
   \note This is the same as the Asn1Acn::TypeAssignment name()
 */
const QString &Type::identifier() const
{
    return m_identifier;
}

void Type::setIdentifier(const QString &name)
{
    m_identifier = name;
}

/*!
   Data values of this ASN1 type (like min/max for numbers)
 */
const QVariantMap &Type::parameters() const
{
    return m_parameters;
}

/*!
   Set parameters
   Example for numbers: { { "min", 5 }, { "max", 15 } }
 */
void Type::setParameters(const QVariantMap &parameters)
{
    m_parameters = parameters;
}

/*!
   Returns children. Children are possible for the types Choice and Sequence
 */
const std::vector<std::unique_ptr<Type>> &Type::children() const
{
    return m_children;
}

/*!
   Adds a child to this type.
   \note This type takes over ownership of the \p child
 */
void Type::addChild(std::unique_ptr<Type> child)
{
    m_children.push_back(std::move(child));
}
