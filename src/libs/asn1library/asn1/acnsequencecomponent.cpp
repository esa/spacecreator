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

#include "acnsequencecomponent.h"

#include "sequencecomponentvisitor.h"

using namespace Asn1Acn;

AcnSequenceComponent::AcnSequenceComponent(const QString &id, const QString &name, std::unique_ptr<Types::Type> type)
    : SequenceComponent(name, name, std::move(type)) // TODO CName in ACN_COMPONENT
    , m_id(id)
{
}

AcnSequenceComponent::AcnSequenceComponent(const AcnSequenceComponent &other)
    : SequenceComponent(other)
    , m_id(other.id())
{
}

std::unique_ptr<SequenceComponent> AcnSequenceComponent::clone() const
{
    return std::make_unique<AcnSequenceComponent>(*this);
}

QString AcnSequenceComponent::definitionAsString() const
{
    return name() + QChar(' ') + (type() ? type()->typeName() : QChar(' '));
}

QString AcnSequenceComponent::presentWhen() const
{
    return QString();
}

void AcnSequenceComponent::accept(SequenceComponentVisitor &visitor)
{
    visitor.visit(*this);
}

const QString &AcnSequenceComponent::id() const
{
    return m_id;
}

void AcnSequenceComponent::setReference(const QString &reference)
{
    m_reference = reference;
}

std::optional<QString> AcnSequenceComponent::reference() const
{
    return m_reference;
}
