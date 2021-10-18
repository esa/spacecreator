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

#include "type.h"

using namespace Asn1Acn::Types;

Type::Type(const QString &identifier)
    : m_identifier(identifier)
    , m_alignment(AlignToNext::unspecified)
{
}

Type::Type(const Type &other)
    : m_identifier(other.m_identifier)
    , m_alignment(other.m_alignment)
{
}

QString Type::label() const
{
    return QLatin1String(": ") + typeName();
}

/*!
   Returns the identifier/name of this type
   \note This is the same as the Asn1Acn::TypeAssignment name()
 */
const QString &Type::identifier() const
{
    return m_identifier;
}

void Type::setIdentifier(QString name)
{
    m_identifier = std::move(name);
}

AlignToNext Type::mapAlignToNext(QStringRef in)
{
    if (in == "byte")
        return AlignToNext::byte;
    if (in == "word")
        return AlignToNext::word;
    if (in == "dword")
        return AlignToNext::dword;
    return AlignToNext::unspecified;
}

Endianness Type::mapEndianess(QStringRef in)
{
    if (in == "big")
        return Endianness::big;
    if (in == "little")
        return Endianness::little;
    return Endianness::unspecified;
}

QString Type::alignToNextToString(AlignToNext param)
{
    switch (param) {
    case AlignToNext::byte:
        return QStringLiteral("byte");
    case AlignToNext::word:
        return QStringLiteral("word");
    case AlignToNext::dword:
        return QStringLiteral("dword");
    case AlignToNext::unspecified:
        return {};
    }

    return {};
}
QString Type::endiannessToString(Endianness param)
{
    switch (param) {
    case Endianness::big:
        return QStringLiteral("big");
    case Endianness::little:
        return QStringLiteral("little");
    case Endianness::unspecified:
        return {};
    }

    return {};
}
