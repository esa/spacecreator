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
#include "enumerated.h"

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"

using namespace Asn1Acn;
using namespace Asn1Acn::Types;

Enumerated::Enumerated(const QString &identifier)
    : Type(identifier)
    , m_encodeValues(false)
{
}

std::unique_ptr<Type> Enumerated::clone() const
{
    return std::make_unique<Enumerated>(*this);
}

QString Enumerated::typeName() const
{
    return QLatin1String("ENUMERATED");
}

void Enumerated::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void Enumerated::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

void Enumerated::addItem(const EnumeratedItem &item)
{
    m_items.insert(item.name(), item);
}

int Enumerated::mapToValue(const QString &key) const
{
    const auto it = items().find(key);
    if (it == items().end())
        throw std::runtime_error("Unknown key in ENUMERATED");
    return encodeValues() ? it->value() : it->index();
}

Range<std::int64_t> Enumerated::mapToValue(const Range<QString> &r) const
{
    if (!r.isSingleItem())
        throw std::runtime_error("Multi-item range in ENUMERATED");
    return Range<std::int64_t>(mapToValue(r.begin()));
}
