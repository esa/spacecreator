/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "asnsequencecomponent.h"

#include "sequencecomponentvisitor.h"

using namespace Asn1Acn;

AsnSequenceComponent::AsnSequenceComponent(const QString &name, const QString &cName, bool optional,
        std::optional<QString> defaultValue, const QString &presentWhen, const Presence presence,
        const SourceLocation &location, std::unique_ptr<Types::Type> type)
    : SequenceComponent(name, cName, std::move(type))
    , m_optional(optional)
    , m_defaultValue(std::move(defaultValue))
    , m_presentWhen(presentWhen)
    , m_presence(presence)
    , m_location(location)
{
}

AsnSequenceComponent::AsnSequenceComponent(const AsnSequenceComponent &other)
    : SequenceComponent(other)
    , m_optional(other.m_optional)
    , m_defaultValue(other.m_defaultValue)
    , m_presentWhen(other.m_presentWhen)
    , m_presence(other.m_presence)
    , m_location(other.m_location)
{
}

QString AsnSequenceComponent::definitionAsString() const
{
    return name();
}

QString AsnSequenceComponent::presentWhen() const
{
    return m_presentWhen;
}

AsnSequenceComponent::Presence AsnSequenceComponent::presence() const
{
    return m_presence;
}

void AsnSequenceComponent::accept(SequenceComponentVisitor &visitor)
{
    visitor.visit(*this);
}

std::unique_ptr<SequenceComponent> AsnSequenceComponent::clone() const
{
    return std::make_unique<AsnSequenceComponent>(*this);
}

const SourceLocation &AsnSequenceComponent::location() const
{
    return m_location;
}

const std::optional<QString> &AsnSequenceComponent::defaultValue() const
{
    return m_defaultValue;
}

bool AsnSequenceComponent::isOptional() const
{
    return m_optional;
}
