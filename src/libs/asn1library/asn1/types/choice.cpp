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
#include "choice.h"

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"

using namespace Asn1Acn;
using namespace Asn1Acn::Types;

ChoiceAlternative::ChoiceAlternative(const QString &name, const QString &presentWhenName, const QString &adaName,
        const QString &cName, const QString &presentWhen, const SourceLocation &location, std::unique_ptr<Type> type)
    : m_name(name)
    , m_presentWhenName(presentWhenName)
    , m_adaName(adaName)
    , m_cName(cName)
    , m_presentWhen(presentWhen)
    , m_location(location)
    , m_type(std::move(type))
{
}

ChoiceAlternative::ChoiceAlternative(const ChoiceAlternative &other)
    : m_name(other.m_name)
    , m_presentWhenName(other.m_presentWhenName)
    , m_adaName(other.m_adaName)
    , m_cName(other.m_cName)
    , m_presentWhen(other.m_presentWhen)
    , m_location(other.m_location)
    , m_type(other.m_type->clone())
{
}

const std::vector<QString> &ChoiceAlternative::acnParameters() const
{
    static std::vector<QString> stub;
    return stub;
}

Choice::Choice(const Choice &other)
    : Type(other)
    , AcnParameterizableCollection<ChoiceAlternative>(other)
{
}

void Choice::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void Choice::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

std::unique_ptr<Type> Choice::clone() const
{
    return std::make_unique<Choice>(*this);
}
