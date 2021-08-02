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

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"
#include "userdefinedtype.h"

using namespace Asn1Acn::Types;

UserdefinedType::UserdefinedType(const QString &name, const QString &module, const Asn1Acn::TypeAssignment *referencedType)
    : m_name(name)
    , m_module(module)
    , m_referencedType(referencedType)
    , m_type(nullptr)
{
}

UserdefinedType::UserdefinedType(const UserdefinedType &other)
    : Type(other)
    , m_name(other.m_name)
    , m_module(other.m_module)
    , m_referencedType(other.m_referencedType)
{
    m_type = (other.m_type != nullptr) ? other.m_type->clone() : nullptr;

    for (const auto &item : other.m_arguments)
        addArgument(std::make_unique<AcnArgument>(*item));
}

QString UserdefinedType::typeName() const
{
    return m_name;
}

QString UserdefinedType::label() const
{
    return ": " + m_module + "." + typeName();
}

Type::ASN1Type UserdefinedType::typeEnum() const
{
    return USERDEFINED;
}

const QString &UserdefinedType::module() const
{
    return m_module;
}

void UserdefinedType::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void UserdefinedType::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

std::unique_ptr<Type> UserdefinedType::clone() const
{
    return std::make_unique<UserdefinedType>(*this);
}

QString UserdefinedType::baseIconFile() const
{
    return QStringLiteral(":/asn1acn/images/outline/userdefined.png");
}

/*!
   Returns the type that this one is based on (references to)
 */
const Asn1Acn::TypeAssignment *UserdefinedType::referencedType() const
{
    return m_referencedType;
}

void UserdefinedType::setType(std::unique_ptr<Type> type)
{
    m_type = std::move(type);
}

void UserdefinedType::addArgument(AcnArgumentPtr argument)
{
    m_arguments.push_back(std::move(argument));
}
