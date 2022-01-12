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
#include "real.h"

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"

using namespace Asn1Acn;
using namespace Asn1Acn::Types;

Real::Real(const QString &identifier)
    : Type(identifier)
    , m_encoding(RealEncoding::unspecified)
    , m_endianness(Endianness::unspecified)
{
}

void Real::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void Real::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

std::unique_ptr<Type> Real::clone() const
{
    return std::make_unique<Real>(*this);
}

RealEncoding Real::mapEncoding(StringRef in)
{
    if (in == QLatin1String("IEEE754-1985-32"))
        return RealEncoding::IEEE754_1985_32;
    if (in == QLatin1String("IEEE754-1985-64"))
        return RealEncoding::IEEE754_1985_64;
    return RealEncoding::unspecified;
}

QString Real::encodingToString(RealEncoding encoding)
{
    switch (encoding) {
    case RealEncoding::IEEE754_1985_32:
        return QStringLiteral("IEEE754-1985-32");
    case RealEncoding::IEEE754_1985_64:
        return QStringLiteral("IEEE754-1985-64");
    default:
        return {};
    }

    return {};
}
