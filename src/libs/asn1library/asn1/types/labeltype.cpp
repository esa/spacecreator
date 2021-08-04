/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a program and funded by
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

#include "labeltype.h"

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"

using namespace Asn1Acn::Types;

LabelType::LabelType(const QString &name)
    : m_name(name)
{
}

QString LabelType::typeName() const
{
    return m_name;
}

QString LabelType::label() const
{
    return QString();
}

Type::ASN1Type LabelType::typeEnum() const
{
    return LABELTYPE;
}

void LabelType::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void LabelType::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

std::unique_ptr<Type> LabelType::clone() const
{
    return std::make_unique<LabelType>(*this);
}

QString LabelType::baseIconFile() const
{
    return QString();
}
